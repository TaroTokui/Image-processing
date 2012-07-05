/********************************************************************
	��̊��N���X
	�����ł́A��N���X���`���Ă��܂��B
	2009/08/25	by �Ƃ���
 ********************************************************************/
#pragma once

#include <stdlib.h>

#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

#define DELAY 20	// ���傤�ǂ悩�����̂�10�t���[��
				// DELAY�t���[�������Ώۂ����X�g���Ă��ǐՂ��s��

class Hand{

public:

	/* �R���X�g���N�^ */
	Hand();
	Hand( CvPoint2D32f position, float size, unsigned int id );

	/* �f�X�g���N�^ */
	virtual ~Hand();

	/* get�֐� */
	CvPoint2D32f getPosition();
	float getSize();
	unsigned int getId();
	bool getLostFlag();

	/* set�֐� */
	void setPosition(CvPoint2D32f position);
	void setSize(float size);

	/* �X�V���s�� */
	void update(bool type);	// false:�X�V�Ȃ� true:�X�V����

	/* update����Ȃ�������lost���� */
	//void setLostFlag(bool b);		// false:�X�V�Ȃ� true:�X�V����

protected:

	// �ʒu�A�T�C�Y�AID
	CvPoint2D32f position;
	float size;
	unsigned int id;
	bool lostFlag;
	int delay;	// �X�V����Ȃ������ꍇ��ID���������܂ł̎���

};