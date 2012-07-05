#include "RegionDetector.h"

/* コンストラクタ */
RegionDetector::RegionDetector(IplImage* srcImage, int numMaxRegion){

	printf("RegionDetectorのコンストラクタが呼び出されました。\n");
	
	storage = NULL;

	this->numMaxRegion = numMaxRegion;

	regions = createRegions();

	tmpLabeledImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);		// ラベリング用

	numRegions = 0;

} // コンストラクタ

/* デストラクタ */
RegionDetector::~RegionDetector(){

	printf("RegionDetectorのデストラクタが呼び出されました。\n");

	releaseRegions();
	
	cvReleaseImage( &tmpLabeledImage );

} // デストラクタ

/* 画像を取得し、ラベル付けして返す関数 */
void RegionDetector::label(IplImage *srcBinarizedImage){

	if( srcBinarizedImage->nChannels == 3 ){
		cvCvtColor(srcBinarizedImage, tmpLabeledImage, CV_BGR2GRAY);
	}else if( srcBinarizedImage->nChannels == 1 ){	
		cvCopy(srcBinarizedImage, tmpLabeledImage);			// 次の処理は入力画像が書き換わるのでコピーをとる
	}else{
		return;
	}
	numRegions = detectRegions(tmpLabeledImage, REGION_MAX, REGION_MIN, regions, numMaxRegion);

//	printf( "%d\n", numMaxRegion );

	/* 結果を表示 */
	for (int i = 0; i < numRegions; i++){
		cvCircle(srcBinarizedImage, cvPoint((int)regions[i].centroid.x, (int)regions[i].centroid.y), 2, CV_RGB(128, 128, 128), 1);
		cvRectangle(srcBinarizedImage, cvPoint((int)regions[i].boundingBox.x, (int)regions[i].boundingBox.y), cvPoint((int)(regions[i].boundingBox.x + regions[i].boundingBox.width), 
			(int)(regions[i].boundingBox.y + regions[i].boundingBox.height)), CV_RGB(128, 128, 128), 1);
	}

}

/* MemStorageを解放、及びRegion構造体を初期化する。
	 毎ループの最後にこれでMemStorageを解放する */
void RegionDetector::cleanUpRegions(){
	cvReleaseMemStorage(&storage);
	memset(regions, 0, sizeof(Region) * numRegions);		// 0で初期化
} // cleanUpRegions

/* ラベル付けした領域のIDの数,面積,位置,いくつあるか等を取得する */
//void RegionDetector::getRegions(Region* regions, int numRegions){
//	regions = this->regions;
//	numRegions = this->numRegions;
//}
Region *RegionDetector::getRegions(){
	return regions;
}

int RegionDetector::getNumRegions(){
	return numRegions;
}

/*********************************************
						 ラベリング用関数
 *********************************************/

/* numMaxRegion個のRegion構造体配列を作る。
	 最初にこの関数でRegion配列のメモリを確保しておく */
Region *RegionDetector::createRegions(){
	regions = new Region[numMaxRegion];
	memset(regions, 0, sizeof(Region) * numMaxRegion);		// 0で初期化
	return regions;
} // createRegions

/* Region構造体を解放する。
	 最後にこの関数で、Region配列のメモリを解放する */
void RegionDetector::releaseRegions(){
	delete [] regions;
} // releaseRegions

/* 輪郭線検出を使ってラベリングを行う関数。
	 第1引数: 2値画像。
	 第2、第3引数: 検出する領域の面積閾値。
	 第4引数: 結果を書き込むRegion構造体の配列。
	 第5引数: Region構造体の配列のサイズ。
	 戻り値で検出した領域の数を返す。 */
int RegionDetector::detectRegions(IplImage *srcBinarizedImageGRAY, float maxSize, float minSize, Region *resultRegions, int numMaxRegion){

	CvSeq* contour = 0;
	int regionCounter = 0;
	double regionSize;

	/* メモリ領域確保 */
	storage = cvCreateMemStorage(0);		// 確保したstorageは毎ループ終了時に解放する

	cvFindContours( srcBinarizedImageGRAY, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );			// 領域を見つけcontourに格納

	/* 領域があれば */
	if( contour != 0 ){

		/* 領域リストをチェック */
		while ( contour && regionCounter < numMaxRegion ){
			
			/* 面積が条件を満たしているか */
			regionSize = fabs( cvContourArea(contour, CV_WHOLE_SEQ) );									// 領域面積を計算
			if( !(regionSize > minSize && regionSize < maxSize) ){				// 指定された面積の範囲外の領域は無視
				contour = contour->h_next;
				continue;
			}

			/* 輪郭情報を保存 */
			regions[regionCounter].contourPtr = contour;
			regions[regionCounter].size = (float)regionSize;
			CvMoments moments;
			cvMoments(contour, &moments, 1);
			if( moments.m00 == 0 ) continue;
			regions[regionCounter].centroid.x = (float)(moments.m10 / moments.m00);
			regions[regionCounter].centroid.y = (float)(moments.m01 / moments.m00);
			regions[regionCounter].boundingBox = cvBoundingRect(contour, 0);

			regionCounter++;
			contour = contour->h_next;
		} // while
	}

	cvReleaseMemStorage (&storage);	// 確保したメモリを解放する

	return regionCounter;		// 検出した数を返す
} // detectRegions