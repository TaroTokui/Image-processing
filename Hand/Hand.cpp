#include "Hand.h"

/* コンストラクタ */
Hand::Hand(){
	delay = DELAY;
	printf("Handクラスのコンストラクタが呼び出されました。\n");
} // コンストラクタ


Hand::Hand( CvPoint2D32f position, float size, unsigned int id ){
	printf("Handクラスのコンストラクタが呼び出されました。\n");
	this->position = position;
	this->size = size;
	this->id = id;
	lostFlag = false;
	delay = DELAY;
}

/* デストラクタ */
Hand::~Hand(){
	printf("Handクラスのデストラクタ呼び出されました。\n");
} // デストラクタ

/* get関数 */
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

/* set関数 */
void Hand::setPosition(CvPoint2D32f position){
	this->position = position;
}//setPosition

void Hand::setSize(float size){
	this->size = size;
}//setSize

/* updateされなかったらlostする */
//void Hand::setLostFlag(bool b){
//	lostFlag = b;
//}//lost

/* 更新を行う */
void Hand::update(bool type){
	if( type ){
		delay = DELAY;
		lostFlag = false;
	}else{
		delay--;
	}
	if(delay<0) lostFlag = true;
}//update