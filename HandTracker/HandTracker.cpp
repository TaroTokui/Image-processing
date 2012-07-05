#include "HandTracker.h"

/* �R���X�g���N�^ */
HandTracker::HandTracker(IplImage *srcImage, int numMaxRegion){

	printf("HandTracker�N���X�̃R���X�g���N�^���Ăяo����܂����B\n");

	tmpImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);		// ���������p

	id = 0;

	this->numMaxRegion = numMaxRegion;

	x = 0;
	y = 0;

} // �R���X�g���N�^

/* �f�X�g���N�^ */
HandTracker::~HandTracker(){

	printf("HandTracker�N���X�̃f�X�g���N�^�Ăяo����܂����B\n");

} // �f�X�g���N�^

/* �摜���󂯎��AID�A�ʐρA���W��\��t���ĕԂ� */
void HandTracker::showHandData(IplImage *srcImage, Region *regions, int numRegions){

	this->regions = regions;
	this->numRegions = numRegions;
	
	if( srcImage->nChannels == 3 ){
		cvCvtColor(srcImage, tmpImage, CV_BGR2GRAY);
	}else if( srcImage->nChannels == 1 ){	
		cvCopy(srcImage, tmpImage);			// ���̏����͓��͉摜�����������̂ŃR�s�[���Ƃ�
	}else{
		return;
	}

//	cvCopy(srcImage, tmpImage);

	update();
	
	if( srcImage->nChannels == 3 ){
		cvCvtColor(tmpImage, srcImage, CV_GRAY2BGR);
	}else if( srcImage->nChannels == 1 ){	
		cvCopy(tmpImage, srcImage);			// ���̏����͓��͉摜�����������̂ŃR�s�[���Ƃ�
	}else{
		return;
	}

//	cvCopy(tmpImage, srcImage);
	
	/* ���W�A�ʐρAID���������� */
	//CvFont font = cvFont(1,1);

	///* ��������摜�ɏ������� */
	//char pos[2*sizeof(int)];//charX[sizeof(int)], charY[sizeof(int)];
	//for(int i=0; i<numRegions; i++){
	//	sprintf( pos, "x:%d y:%d", (int)regions[i].centroid.x, (int)regions[i].centroid.y );
	//	cvPutText( srcImage, pos, cvPoint((int)regions[i].centroid.x, (int)regions[i].centroid.y), &font, CV_RGB(255,255,255) );
	//}

}

/*********************************************
						 �̈�ǐ՗p�֐�
 *********************************************/

/* hand���X�V����
	���̊֐����Ăяo���O��regions���󂯎��
	���ݎ����Ă���Hand�ɋ߂����̂������Hand���X�V����
	�Ȃ���ΐV�K�ɍ쐬����
	�X�V����Ȃ�����Hand�͏������� */
void HandTracker::update(){

	list<Hand*>::iterator itr = handList.begin(); // �C�e���[�^

	/* �S�Ă�lostFlag��true�ɂ��� */
	while( itr != handList.end() )  // list�̖����܂�
	{	
//		(*itr)->setLostFlag(true);
		(*itr)->update(false);	// true:delay�������� false:delay-1, delay<0�Ȃ�flag=true
		++itr;  // �C�e���[�^���P�i�߂�
	}

	for(int i=0; i<numRegions; i++){

		/* �V�����f�[�^�ƁA�X�V�O��hand�f�[�^���ׂ�
			�߂��f�[�^������΍X�V����*/ 
		if( checkNearestPoint(regions[i]) < 0 ){

			/* ��ԋ߂��_���Ȃ���ΐV�K�ɍ쐬���� */
			int j = handList.size();
			if( j < numMaxRegion ){
				Hand *newHandData = new Hand(regions[i].centroid, regions[i].size, id);
				handList.push_back(newHandData);
				//printf("%d\n",id)+
				id++;
			}else{
				printf("�V���ȗ̈��ǉ��ł��܂���B\n");
			}

		}
	}
	/* ���΂炭�X�V����Ȃ�����hand��delete���� */
	itr = handList.begin();
	while( itr != handList.end() )  // list�̖����܂�
	{	
		if( (*itr)->getLostFlag() ){
			delete (*itr);
			itr = handList.erase( itr );
		}else{
			++itr;  // �C�e���[�^���P�i�߂�
		}
	}

}

/* �ł��߂��_��������
	���݂�region����͂�
	hand���ɂ��Ē��ׂ�
	�ł��߂�ID��Ԃ�
	�Ȃ���� -1 ��Ԃ� */
int HandTracker::checkNearestPoint(Region region){

	float distance = MAX_DISTANCE, tmpDistance;
	float x, y, dx, dy;
	int nearestID = -1;

	x = region.centroid.x;
	y = region.centroid.y;

	/* ���W�A�ʐρAID���������� */
	CvFont font = cvFont(1,1);
	char pos[100];//charX[sizeof(int)], charY[sizeof(int)];

	list<Hand*>::iterator itr = handList.begin(); // �C�e���[�^

	/* ��ԋ߂��_��T�� */
	while( itr != handList.end() )  // list�̖����܂�
	{
		
		/* �����̌v�Z���s�� */
		dx = (*itr)->getPosition().x - x;
		dy = (*itr)->getPosition().y - y;

		tmpDistance = calcDistance(dx, dy);

		/* ��ԋ����̋߂�Id��T�� */
		if( distance > tmpDistance ){
			distance = tmpDistance;
			nearestID = (*itr)->getId();
		}

		++itr;  // �C�e���[�^���P�i�߂�

	}

	/* ��ԋ߂��_������΍X�V���� */
	// ���łɎg�p����Ă���ID�͏����K�v������
	if(nearestID > 0){
		itr = handList.begin();
		while( itr != handList.end() )  // list�̖����܂�
		{
			/* ����ID��������΂���ID��Ԃ� */
			if( (*itr)->getId() == nearestID ){
				(*itr)->setPosition(region.centroid);
				(*itr)->setSize(region.size);

				/* ��������摜�ɏ������� */
				sprintf( pos, "ID:%d x:%d y:%d", (*itr)->getId(), (int)(*itr)->getPosition().x, (int)(*itr)->getPosition().y );
				cvPutText( tmpImage, pos, cvPoint( (int)(*itr)->getPosition().x, (int)(*itr)->getPosition().y ), &font, CV_RGB(255,255,255) );
				x = (int)(*itr)->getPosition().x;
				y = (int)(*itr)->getPosition().y;
				//(*itr)->setLostFlag(false);
				(*itr)->update(true);	// true:delay�������� false:delay-1
				return nearestID;
			}
			++itr;  // �C�e���[�^���P�i�߂�
		}
	}

	return nearestID;
}

/* �����̌v�Z */
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

	list<Hand*>::iterator itr = handList.begin(); // �C�e���[�^

	/* 1:id����ԏ����ȗ̈�̍��W���A��
		 2:�ʐς���ԑ傫�ȗ̈�̍��W��Ԃ�*/
	if( type == 1 ){

		while( itr != handList.end() )
		{
			/* ��ԏ�����Id��T�� */
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
			++itr;  // �C�e���[�^���P�i�߂�
		}//while
	}

	if( type == 2 ){

		while( itr != handList.end() )
		{
			/* ��ԑ傫���ʐς�T�� */
			if( tmpSize < (*itr)->getSize() ){
				tmpSize = (*itr)->getSize();
				position.x = (*itr)->getPosition().x;
				position.y = (*itr)->getPosition().y;
			}
			++itr;  // �C�e���[�^���P�i�߂�
		}//while
	}

	return position;
}

/* Region���󂯎��A�ł��߂�ID��Ԃ� */
int HandTracker::getID(Region region){
	return checkNearestPoint(region);
}

