#include "RegionDetector.h"

/* �R���X�g���N�^ */
RegionDetector::RegionDetector(IplImage* srcImage, int numMaxRegion){

	printf("RegionDetector�̃R���X�g���N�^���Ăяo����܂����B\n");
	
	storage = NULL;

	this->numMaxRegion = numMaxRegion;

	regions = createRegions();

	tmpLabeledImage = cvCreateImage(cvGetSize(srcImage), IPL_DEPTH_8U, 1);		// ���x�����O�p

	numRegions = 0;

} // �R���X�g���N�^

/* �f�X�g���N�^ */
RegionDetector::~RegionDetector(){

	printf("RegionDetector�̃f�X�g���N�^���Ăяo����܂����B\n");

	releaseRegions();
	
	cvReleaseImage( &tmpLabeledImage );

} // �f�X�g���N�^

/* �摜���擾���A���x���t�����ĕԂ��֐� */
void RegionDetector::label(IplImage *srcBinarizedImage){

	if( srcBinarizedImage->nChannels == 3 ){
		cvCvtColor(srcBinarizedImage, tmpLabeledImage, CV_BGR2GRAY);
	}else if( srcBinarizedImage->nChannels == 1 ){	
		cvCopy(srcBinarizedImage, tmpLabeledImage);			// ���̏����͓��͉摜�����������̂ŃR�s�[���Ƃ�
	}else{
		return;
	}
	numRegions = detectRegions(tmpLabeledImage, REGION_MAX, REGION_MIN, regions, numMaxRegion);

//	printf( "%d\n", numMaxRegion );

	/* ���ʂ�\�� */
	for (int i = 0; i < numRegions; i++){
		cvCircle(srcBinarizedImage, cvPoint((int)regions[i].centroid.x, (int)regions[i].centroid.y), 2, CV_RGB(128, 128, 128), 1);
		cvRectangle(srcBinarizedImage, cvPoint((int)regions[i].boundingBox.x, (int)regions[i].boundingBox.y), cvPoint((int)(regions[i].boundingBox.x + regions[i].boundingBox.width), 
			(int)(regions[i].boundingBox.y + regions[i].boundingBox.height)), CV_RGB(128, 128, 128), 1);
	}

}

/* MemStorage������A�y��Region�\���̂�����������B
	 �����[�v�̍Ō�ɂ����MemStorage��������� */
void RegionDetector::cleanUpRegions(){
	cvReleaseMemStorage(&storage);
	memset(regions, 0, sizeof(Region) * numRegions);		// 0�ŏ�����
} // cleanUpRegions

/* ���x���t�������̈��ID�̐�,�ʐ�,�ʒu,�������邩�����擾���� */
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
						 ���x�����O�p�֐�
 *********************************************/

/* numMaxRegion��Region�\���̔z������B
	 �ŏ��ɂ��̊֐���Region�z��̃��������m�ۂ��Ă��� */
Region *RegionDetector::createRegions(){
	regions = new Region[numMaxRegion];
	memset(regions, 0, sizeof(Region) * numMaxRegion);		// 0�ŏ�����
	return regions;
} // createRegions

/* Region�\���̂��������B
	 �Ō�ɂ��̊֐��ŁARegion�z��̃�������������� */
void RegionDetector::releaseRegions(){
	delete [] regions;
} // releaseRegions

/* �֊s�����o���g���ă��x�����O���s���֐��B
	 ��1����: 2�l�摜�B
	 ��2�A��3����: ���o����̈�̖ʐ�臒l�B
	 ��4����: ���ʂ���������Region�\���̂̔z��B
	 ��5����: Region�\���̂̔z��̃T�C�Y�B
	 �߂�l�Ō��o�����̈�̐���Ԃ��B */
int RegionDetector::detectRegions(IplImage *srcBinarizedImageGRAY, float maxSize, float minSize, Region *resultRegions, int numMaxRegion){

	CvSeq* contour = 0;
	int regionCounter = 0;
	double regionSize;

	/* �������̈�m�� */
	storage = cvCreateMemStorage(0);		// �m�ۂ���storage�͖����[�v�I�����ɉ������

	cvFindContours( srcBinarizedImageGRAY, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );			// �̈������contour�Ɋi�[

	/* �̈悪����� */
	if( contour != 0 ){

		/* �̈惊�X�g���`�F�b�N */
		while ( contour && regionCounter < numMaxRegion ){
			
			/* �ʐς������𖞂����Ă��邩 */
			regionSize = fabs( cvContourArea(contour, CV_WHOLE_SEQ) );									// �̈�ʐς��v�Z
			if( !(regionSize > minSize && regionSize < maxSize) ){				// �w�肳�ꂽ�ʐς͈̔͊O�̗̈�͖���
				contour = contour->h_next;
				continue;
			}

			/* �֊s����ۑ� */
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

	cvReleaseMemStorage (&storage);	// �m�ۂ������������������

	return regionCounter;		// ���o��������Ԃ�
} // detectRegions