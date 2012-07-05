#include "Hand.h"

/* �R���X�g���N�^ */
Hand::Hand(){
	delay = DELAY;
	printf("Hand�N���X�̃R���X�g���N�^���Ăяo����܂����B\n");
} // �R���X�g���N�^


Hand::Hand( CvPoint2D32f position, float size, unsigned int id ){
	printf("Hand�N���X�̃R���X�g���N�^���Ăяo����܂����B\n");
	this->position = position;
	this->size = size;
	this->id = id;
	lostFlag = false;
	delay = DELAY;
}

/* �f�X�g���N�^ */
Hand::~Hand(){
	printf("Hand�N���X�̃f�X�g���N�^�Ăяo����܂����B\n");
} // �f�X�g���N�^

/* get�֐� */
CvPoint2D32f Hand::getPosition(){
	return position;
}// getPosition

float Hand::getSize(){
	return size;
}//getSize

unsigned int Hand::getId(){
	return id;
}//getId

bool Hand::getLostFlag(){
	return lostFlag;
}//getLostFlag

/* set�֐� */
void Hand::setPosition(CvPoint2D32f position){
	this->position = position;
}//setPosition

void Hand::setSize(float size){
	this->size = size;
}//setSize

/* update����Ȃ�������lost���� */
//void Hand::setLostFlag(bool b){
//	lostFlag = b;
//}//lost

/* �X�V���s�� */
void Hand::update(bool type){
	if( type ){
		delay = DELAY;
		lostFlag = false;
	}else{
		delay--;
	}
	if(delay<0) lostFlag = true;
}//update