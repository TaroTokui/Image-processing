/********************************************************************
	手の基底クラス
	ここでは、手クラスを定義しています。
	2009/08/25	by とくい
 ********************************************************************/
#pragma once

#include <stdlib.h>

#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

#define DELAY 20	// ちょうどよかったのは10フレーム
				// DELAYフレームだけ対象をロストしても追跡を行う

class Hand{

public:

	/* コンストラクタ */
	Hand();
	Hand( CvPoint2D32f position, float size, unsigned int id );

	/* デストラクタ */
	virtual ~Hand();

	/* get関数 */
	CvPoint2D32f getPosition();
	float getSize();
	unsigned int getId();
	bool getLostFlag();

	/* set関数 */
	void setPosition(CvPoint2D32f position);
	void setSize(float size);

	/* 更新を行う */
	void update(bool type);	// false:更新なし true:更新あり

	/* updateされなかったらlostする */
	//void setLostFlag(bool b);		// false:更新なし true:更新あり

protected:

	// 位置、サイズ、ID
	CvPoint2D32f position;
	float size;
	unsigned int id;
	bool lostFlag;
	int delay;	// 更新されなかった場合にIDが消されるまでの時間

};