/********************************************************************
	手追跡クラス
	ここでは、手追跡クラスを定義しています。
	2009/08/25	by とくい

	概要：
		前のフレームの位置と現在のフレームの位置を比較する
		該当する(閾値より近い)ときは同一の領域が移動したものとする

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

	/* コンストラクタ */
	HandTracker(IplImage *srcImage, int numMaxRegion);

	/* デストラクタ */
	~HandTracker();

	/* 画像を受け取り、ID、面積、座標を貼り付けて返す */
	void showHandData(IplImage *srcImage, Region *regions, int numRegions);

	/* id = 1, IDが最小のオブジェクト座標、id = 2, 面積最大のオブジェクト座標を返す */
	CvPoint2D32f getPosition(int id);

	/* Regionを受け取り、最も近いIDを返す */
	int getID(Region region);

private:

	/* ラベリング情報格納用変数 */
	Region *regions;				// 現在のregion情報
	//Region *preRegions;			// １フレーム前のregions情報
	int numRegions;					// 現在のフレームのregionの数
	//int preNumRegions;			// 前のフレームのregionの数
	//Hand *hand[MAX_HAND];							// 手クラス
	list<Hand*> handList;
	//Hand *preHand;
	int id;									// 現在のIDの数
	int numMaxRegion;
	int x, y;

	IplImage *tmpImage;			// 内部処理用IplImage

	/* regionを更新する
		この関数を呼び出す前にregionsを受けとる
		現在持っているHandに近いものがあればHandを更新する
		なければ新規に作成する
		更新されなかったHandは消去する */
	void update();

	/* 距離の計算 */
	float calcDistance(float x, float y);

	/* もっとも近い点を見つける
		現在のregionを入力し
		hand一つ一つについて調べる
		最も近いIDを返す
		なければ-1を返す */
	int checkNearestPoint(Region region);

};