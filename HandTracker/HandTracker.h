/********************************************************************
	��ǐՃN���X
	�����ł́A��ǐՃN���X���`���Ă��܂��B
	2009/08/25	by �Ƃ���

	�T�v�F
		�O�̃t���[���̈ʒu�ƌ��݂̃t���[���̈ʒu���r����
		�Y������(臒l���߂�)�Ƃ��͓���̗̈悪�ړ��������̂Ƃ���

 ********************************************************************/
#pragma once

#include <stdlib.h>
#include <list>
#include <string>

#include "hand.h"
#include "RegionDetector.h"

using namespace std;

static const int MAX_DISTANCE = 20;
static const int MAX_HAND = 20;

class HandTracker{

public:

	/* �R���X�g���N�^ */
	HandTracker(IplImage *srcImage, int numMaxRegion);

	/* �f�X�g���N�^ */
	~HandTracker();

	/* �摜���󂯎��AID�A�ʐρA���W��\��t���ĕԂ� */
	void showHandData(IplImage *srcImage, Region *regions, int numRegions);

	/* id = 1, ID���ŏ��̃I�u�W�F�N�g���W�Aid = 2, �ʐύő�̃I�u�W�F�N�g���W��Ԃ� */
	CvPoint2D32f getPosition(int id);

	/* Region���󂯎��A�ł��߂�ID��Ԃ� */
	int getID(Region region);

private:

	/* ���x�����O���i�[�p�ϐ� */
	Region *regions;				// ���݂�region���
	//Region *preRegions;			// �P�t���[���O��regions���
	int numRegions;					// ���݂̃t���[����region�̐�
	//int preNumRegions;			// �O�̃t���[����region�̐�
	//Hand *hand[MAX_HAND];							// ��N���X
	list<Hand*> handList;
	//Hand *preHand;
	int id;									// ���݂�ID�̐�
	int numMaxRegion;
	int x, y;

	IplImage *tmpImage;			// ���������pIplImage

	/* region���X�V����
		���̊֐����Ăяo���O��regions���󂯂Ƃ�
		���ݎ����Ă���Hand�ɋ߂����̂������Hand���X�V����
		�Ȃ���ΐV�K�ɍ쐬����
		�X�V����Ȃ�����Hand�͏������� */
	void update();

	/* �����̌v�Z */
	float calcDistance(float x, float y);

	/* �����Ƃ��߂��_��������
		���݂�region����͂�
		hand���ɂ��Ē��ׂ�
		�ł��߂�ID��Ԃ�
		�Ȃ����-1��Ԃ� */
	int checkNearestPoint(Region region);

};