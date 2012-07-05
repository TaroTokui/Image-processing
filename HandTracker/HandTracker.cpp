#include "HandTracker.h"

/* コンストラクタ */
HandTracker::HandTracker(IplImage *srcImage, int numMaxRegion){

	printf("HandTrackerクラスのコンストラクタが呼び出されました。\n");

	tmpImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);		// 内部処理用

	id = 0;

	this->numMaxRegion = numMaxRegion;

	x = 0;
	y = 0;

} // コンストラクタ

/* デストラクタ */
HandTracker::~HandTracker(){

	printf("HandTrackerクラスのデストラクタ呼び出されました。\n");

} // デストラクタ

/* 画像を受け取り、ID、面積、座標を貼り付けて返す */
void HandTracker::showHandData(IplImage *srcImage, Region *regions, int numRegions){

	this->regions = regions;
	this->numRegions = numRegions;
	
	if( srcImage->nChannels == 3 ){
		cvCvtColor(srcImage, tmpImage, CV_BGR2GRAY);
	}else if( srcImage->nChannels == 1 ){	
		cvCopy(srcImage, tmpImage);			// 次の処理は入力画像が書き換わるのでコピーをとる
	}else{
		return;
	}

//	cvCopy(srcImage, tmpImage);

	update();
	
	if( srcImage->nChannels == 3 ){
		cvCvtColor(tmpImage, srcImage, CV_GRAY2BGR);
	}else if( srcImage->nChannels == 1 ){	
		cvCopy(tmpImage, srcImage);			// 次の処理は入力画像が書き換わるのでコピーをとる
	}else{
		return;
	}

//	cvCopy(tmpImage, srcImage);
	
	/* 座標、面積、IDを書き込む */
	//CvFont font = cvFont(1,1);

	///* 文字列を画像に書き込み */
	//char pos[2*sizeof(int)];//charX[sizeof(int)], charY[sizeof(int)];
	//for(int i=0; i<numRegions; i++){
	//	sprintf( pos, "x:%d y:%d", (int)regions[i].centroid.x, (int)regions[i].centroid.y );
	//	cvPutText( srcImage, pos, cvPoint((int)regions[i].centroid.x, (int)regions[i].centroid.y), &font, CV_RGB(255,255,255) );
	//}

}

/*********************************************
						 領域追跡用関数
 *********************************************/

/* handを更新する
	この関数を呼び出す前にregionsを受け取る
	現在持っているHandに近いものがあればHandを更新する
	なければ新規に作成する
	更新されなかったHandは消去する */
void HandTracker::update(){

	list<Hand*>::iterator itr = handList.begin(); // イテレータ

	/* 全てのlostFlagをtrueにする */
	while( itr != handList.end() )  // listの末尾まで
	{	
//		(*itr)->setLostFlag(true);
		(*itr)->update(false);	// true:delayを初期化 false:delay-1, delay<0ならflag=true
		++itr;  // イテレータを１つ進める
	}

	for(int i=0; i<numRegions; i++){

		/* 新しいデータと、更新前のhandデータを比べる
			近いデータがあれば更新する*/ 
		if( checkNearestPoint(regions[i]) < 0 ){

			/* 一番近い点がなければ新規に作成する */
			int j = handList.size();
			if( j < numMaxRegion ){
				Hand *newHandData = new Hand(regions[i].centroid, regions[i].size, id);
				handList.push_back(newHandData);
				//printf("%d\n",id)+
				id++;
			}else{
				printf("新たな領域を追加できません。\n");
			}

		}
	}
	/* しばらく更新されなかったhandはdeleteする */
	itr = handList.begin();
	while( itr != handList.end() )  // listの末尾まで
	{	
		if( (*itr)->getLostFlag() ){
			delete (*itr);
			itr = handList.erase( itr );
		}else{
			++itr;  // イテレータを１つ進める
		}
	}

}

/* 最も近い点を見つける
	現在のregionを入力し
	hand一つ一つについて調べる
	最も近いIDを返す
	なければ -1 を返す */
int HandTracker::checkNearestPoint(Region region){

	float distance = MAX_DISTANCE, tmpDistance;
	float x, y, dx, dy;
	int nearestID = -1;

	x = region.centroid.x;
	y = region.centroid.y;

	/* 座標、面積、IDを書き込む */
	CvFont font = cvFont(1,1);
	char pos[100];//charX[sizeof(int)], charY[sizeof(int)];

	list<Hand*>::iterator itr = handList.begin(); // イテレータ

	/* 一番近い点を探す */
	while( itr != handList.end() )  // listの末尾まで
	{
		
		/* 距離の計算を行う */
		dx = (*itr)->getPosition().x - x;
		dy = (*itr)->getPosition().y - y;

		tmpDistance = calcDistance(dx, dy);

		/* 一番距離の近いIdを探す */
		if( distance > tmpDistance ){
			distance = tmpDistance;
			nearestID = (*itr)->getId();
		}

		++itr;  // イテレータを１つ進める

	}

	/* 一番近い点があれば更新する */
	// すでに使用されているIDは除く必要がある
	if(nearestID > 0){
		itr = handList.begin();
		while( itr != handList.end() )  // listの末尾まで
		{
			/* 同じIDが見つかればそのIDを返す */
			if( (*itr)->getId() == nearestID ){
				(*itr)->setPosition(region.centroid);
				(*itr)->setSize(region.size);

				/* 文字列を画像に書き込み */
				sprintf( pos, "ID:%d x:%d y:%d", (*itr)->getId(), (int)(*itr)->getPosition().x, (int)(*itr)->getPosition().y );
				cvPutText( tmpImage, pos, cvPoint( (int)(*itr)->getPosition().x, (int)(*itr)->getPosition().y ), &font, CV_RGB(255,255,255) );
				x = (int)(*itr)->getPosition().x;
				y = (int)(*itr)->getPosition().y;
				//(*itr)->setLostFlag(false);
				(*itr)->update(true);	// true:delayを初期化 false:delay-1
				return nearestID;
			}
			++itr;  // イテレータを１つ進める
		}
	}

	return nearestID;
}

/* 距離の計算 */
float HandTracker::calcDistance(float x, float y){

	float distance;

	/* d = ( x^2 + y^2 )^(1/2) */
	distance = sqrt( x*x + y*y );

	return distance;
}


CvPoint2D32f HandTracker::getPosition(int type){

	CvPoint2D32f position;
	position.x = 0;
	position.y = 0;
	int tmpID = -1;
	int tmpSize = 0;

	list<Hand*>::iterator itr = handList.begin(); // イテレータ

	/* 1:idが一番小さな領域の座標を帰す
		 2:面積が一番大きな領域の座標を返す*/
	if( type == 1 ){

		while( itr != handList.end() )
		{
			/* 一番小さいIdを探す */
			if( tmpID < 0 ){
				tmpID = (*itr)->getId();
				tmpID = (*itr)->getId();
				position.x = (*itr)->getPosition().x;
				position.y = (*itr)->getPosition().y;
			}
			if( tmpID > (*itr)->getId() ){
				tmpID = (*itr)->getId();
				position.x = (*itr)->getPosition().x;
				position.y = (*itr)->getPosition().y;
			}
			++itr;  // イテレータを１つ進める
		}//while
	}

	if( type == 2 ){

		while( itr != handList.end() )
		{
			/* 一番大きい面積を探す */
			if( tmpSize < (*itr)->getSize() ){
				tmpSize = (*itr)->getSize();
				position.x = (*itr)->getPosition().x;
				position.y = (*itr)->getPosition().y;
			}
			++itr;  // イテレータを１つ進める
		}//while
	}

	return position;
}

/* Regionを受け取り、最も近いIDを返す */
int HandTracker::getID(Region region){
	return checkNearestPoint(region);
}

