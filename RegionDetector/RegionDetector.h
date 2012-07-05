/*****************************************
	背景差分クラス「RegionDetector」
		輪郭線検出を使った高速なラベリングを行うクラス
		2009/08/24	by とくい

	概要：
	　バイナリ画像を受け取り
	  領域分割を行って
		各領域を四角形で囲んだ画像を返す

 *****************************************/

#pragma once

#include <stdlib.h>
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

#define REGION_MIN 20
#define REGION_MAX 500

/* ラベリング用領域情報の宣言 */
typedef struct{
	CvSeq *contourPtr;					// 領域の輪郭情報
	CvPoint2D32f centroid;			// 領域の重心
	CvRect boundingBox;					// 領域のバウンディングボックス
	float size;									// 領域の面積
} Region;

class RegionDetector{

public:

	/* コンストラクタ */
	RegionDetector(IplImage* srcImage, int numMaxRegion);
	
	/* デストラクタ */
	~RegionDetector();

	/* 画像を取得し、ラベル付けして返す関数 */
	void label(IplImage *srcBinarizedImage);

	/* MemStorageを解放、及びRegion構造体を初期化する。
		 毎ループの最後にこれでMemStorageを解放する */
	void cleanUpRegions();

	/* ラベル付けした領域のIDの数,面積,位置,いくつあるかを取得する */
	//void getRegions(Region* regions, int numRegions);
	Region* getRegions();
	int getNumRegions();

private:

	/* ラベリング用変数 */
	Region *regions;							// Region配列
	CvMemStorage *storage;
	IplImage *tmpLabeledImage;	// ラベル付けの過程で使う
	int numMaxRegion;						// regionの最大個数
	int numRegions;							// regionの個数をカウントする

	/* Region構造体の配列を作る */
	Region* createRegions();

	/* Region構造体の配列を解放する */
	void releaseRegions();

	/* 輪郭線検出を使ってラベリングを行う関数。
		 第1引数: 2値画像。
		 第2、第3引数: 検出する領域の面積閾値。
		 第4引数: 結果を書き込むRegion構造体の配列。
		 第5引数: Region構造体の配列のサイズ。
		 戻り値で検出した領域の数を返す。 */
	int detectRegions(IplImage *srcBinarizedImageGRAY, float maxSize, float minSize, Region *resultRegions, int NUM_MAX_REGIONS);

};