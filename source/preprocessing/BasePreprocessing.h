/**
* @file
* Defines the BasePreprocessing abstract class.
*/

#ifndef BASE_PREPROCESSING_H__
#define BASE_PREPROCESSING_H__

// OpenCV
#include <cv.h>
#include "TraceMessage.h"

namespace kifas
{

/**
 The Preprocessing Abstract class. It defines how to get frames.
 @ingroup preprocessing
*/
class BasePreprocessing {
public:

  BasePreprocessing() {}
	
	/**
	 perform Preprocessing
	 @return Mat image after preprocessing.	
	*/
	//virtual void RunPreprocessing(cv::Mat &input) = 0;
    virtual void RunPreprocessing(std::vector<cv::Mat> &input) = 0;
};

}

#endif /* BASE_PREPROCESSING_H__ */