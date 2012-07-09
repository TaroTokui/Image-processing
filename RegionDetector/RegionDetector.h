/*****************************************
	RegionDetectorÅv
		輪郭線検出を使った高速なラベリングを行う
		2009/08/24	by Taro Tokui

	abstract:
        get a binaly image
	  óÃàÊï™äÑÇçsÇ¡Çƒ
		äeóÃàÊÇéläpå`Ç≈àÕÇÒÇæâÊëúÇï‘Ç∑

 *****************************************/

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

#define REGION_MIN 20
#define REGION_MAX 500

/* ÉâÉxÉäÉìÉOópóÃàÊèÓïÒÇÃêÈåæ */
typedef struct{
	CvSeq *contourPtr;					// óÃàÊÇÃó÷äsèÓïÒ
	CvPoint2D32f centroid;			// óÃàÊÇÃèdêS
	CvRect boundingBox;					// óÃàÊÇÃÉoÉEÉìÉfÉBÉìÉOÉ{ÉbÉNÉX
	float size;									// óÃàÊÇÃñ êœ
} Region;

class RegionDetector{

public:

	/* ÉRÉìÉXÉgÉâÉNÉ^ */
	RegionDetector(IplImage* srcImage, int numMaxRegion);
	
	/* ÉfÉXÉgÉâÉNÉ^ */
	~RegionDetector();

	/* âÊëúÇéÊìæÇµÅAÉâÉxÉãïtÇØÇµÇƒï‘Ç∑ä÷êî */
	void label(IplImage *srcBinarizedImage);

	/* MemStorageÇâï˙ÅAãyÇ—Regionç\ë¢ëÃÇèâä˙âªÇ∑ÇÈÅB
		 ñàÉãÅ[ÉvÇÃç≈å„Ç…Ç±ÇÍÇ≈MemStorageÇâï˙Ç∑ÇÈ */
	void cleanUpRegions();

	/* ÉâÉxÉãïtÇØÇµÇΩóÃàÊÇÃIDÇÃêî,ñ êœ,à íu,Ç¢Ç≠Ç¬Ç†ÇÈÇ©ÇéÊìæÇ∑ÇÈ */
	//void getRegions(Region* regions, int numRegions);
	Region* getRegions();
	int getNumRegions();

private:

	/* ÉâÉxÉäÉìÉOópïœêî */
	Region *regions;							// RegionîzóÒ
	CvMemStorage *storage;
	IplImage *tmpLabeledImage;	// ÉâÉxÉãïtÇØÇÃâﬂíˆÇ≈égÇ§
	int numMaxRegion;						// regionÇÃç≈ëÂå¬êî
	int numRegions;							// regionÇÃå¬êîÇÉJÉEÉìÉgÇ∑ÇÈ

	/* Regionç\ë¢ëÃÇÃîzóÒÇçÏÇÈ */
	Region* createRegions();

	/* Regionç\ë¢ëÃÇÃîzóÒÇâï˙Ç∑ÇÈ */
	void releaseRegions();

	/* ó÷äsê¸åüèoÇégÇ¡ÇƒÉâÉxÉäÉìÉOÇçsÇ§ä÷êîÅB
		 ëÊ1à¯êî: 2ílâÊëúÅB
		 ëÊ2ÅAëÊ3à¯êî: åüèoÇ∑ÇÈóÃàÊÇÃñ êœËáílÅB
		 ëÊ4à¯êî: åãâ ÇèëÇ´çûÇﬁRegionç\ë¢ëÃÇÃîzóÒÅB
		 ëÊ5à¯êî: Regionç\ë¢ëÃÇÃîzóÒÇÃÉTÉCÉYÅB
		 ñﬂÇËílÇ≈åüèoÇµÇΩóÃàÊÇÃêîÇï‘Ç∑ÅB */
	int detectRegions(IplImage *srcBinarizedImageGRAY, float maxSize, float minSize, Region *resultRegions, int NUM_MAX_REGIONS);

};