#ifndef NEDPU_APPEARANCECODE_H__
#define NEDPU_APPEARANCECODE_H__

// OpenCV
#include <cv.h>
#include "BaseAppearanceCode.h"

#include <map>


/**
The Preprocessing Abstract class. It defines how to get frames.
@ingroup appearanceCode
*/
class NEDPU : public kifas::BaseAppearanceCode {
public:
	enum MaskType { KIRSCH, SOBEL };


	NEDPU();
	~NEDPU() {}


	//methods		
	std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &input);

	int ComputeMagThreshold(cv::Mat gradMag, float mT1Percent);
	int calcSIGN(int position, float angle);
	double* gaussianTable(int radius);
	int angleDiff(int position, float angle);

	int getCodeSize();




private:


	//file.conf
	std::string mConfigFile;

	//internal variables from file

	int magThresh;
	int scorethresh;
	float edgepercent;
	float sigma;
	int padding;
	uchar ignorecode;


};

#endif /* LDP_APPEARANCECODE_H__ */