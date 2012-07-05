/*****************************************
	�w�i�����N���X�uRegionDetector�v
		�֊s�����o���g���������ȃ��x�����O���s���N���X
		2009/08/24	by �Ƃ���

	�T�v�F
	�@�o�C�i���摜���󂯎��
	  �̈敪�����s����
		�e�̈���l�p�`�ň͂񂾉摜��Ԃ�

 *****************************************/

#pragma once

#include <stdlib.h>
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

#define REGION_MIN 20
#define REGION_MAX 500

/* ���x�����O�p�̈���̐錾 */
typedef struct{
	CvSeq *contourPtr;					// �̈�̗֊s���
	CvPoint2D32f centroid;			// �̈�̏d�S
	CvRect boundingBox;					// �̈�̃o�E���f�B���O�{�b�N�X
	float size;									// �̈�̖ʐ�
} Region;

class RegionDetector{

public:

	/* �R���X�g���N�^ */
	RegionDetector(IplImage* srcImage, int numMaxRegion);
	
	/* �f�X�g���N�^ */
	~RegionDetector();

	/* �摜���擾���A���x���t�����ĕԂ��֐� */
	void label(IplImage *srcBinarizedImage);

	/* MemStorage������A�y��Region�\���̂�����������B
		 �����[�v�̍Ō�ɂ����MemStorage��������� */
	void cleanUpRegions();

	/* ���x���t�������̈��ID�̐�,�ʐ�,�ʒu,�������邩���擾���� */
	//void getRegions(Region* regions, int numRegions);
	Region* getRegions();
	int getNumRegions();

private:

	/* ���x�����O�p�ϐ� */
	Region *regions;							// Region�z��
	CvMemStorage *storage;
	IplImage *tmpLabeledImage;	// ���x���t���̉ߒ��Ŏg��
	int numMaxRegion;						// region�̍ő��
	int numRegions;							// region�̌����J�E���g����

	/* Region�\���̂̔z������ */
	Region* createRegions();

	/* Region�\���̂̔z���������� */
	void releaseRegions();

	/* �֊s�����o���g���ă��x�����O���s���֐��B
		 ��1����: 2�l�摜�B
		 ��2�A��3����: ���o����̈�̖ʐ�臒l�B
		 ��4����: ���ʂ���������Region�\���̂̔z��B
		 ��5����: Region�\���̂̔z��̃T�C�Y�B
		 �߂�l�Ō��o�����̈�̐���Ԃ��B */
	int detectRegions(IplImage *srcBinarizedImageGRAY, float maxSize, float minSize, Region *resultRegions, int NUM_MAX_REGIONS);

};