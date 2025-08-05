#ifndef BASE_APPEARANCE_CODE_H__
#define BASE_APPEARANCE_CODE_H__

// OpenCV
#include <cv.h>
#include "TraceMessage.h"

namespace kifas
{
/**
  The Appreance code Abstract class. It defines how to get frames.
  @ingroup appearanceCode
*/
class BaseAppearanceCode {
public:

  BaseAppearanceCode() {}
	
	/**
	  Creates the appearance code of the class.
	  @param input the image to generate the micro features from.
	  @return an image after making the appearance code.	
	*/
	//virtual cv::Mat runCodeGenerate(const cv::Mat input) = 0;
    virtual std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &input) = 0;
	/**
	  Calculate the size of the appearance code of the class
	  @return the size of the code
	*/
	virtual int getCodeSize() =0;
};

}

#endif /* BASE_APPEARANCE_CODE_H__ */