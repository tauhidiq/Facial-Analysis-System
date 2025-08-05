/**
* @file
* Defines the BaseFeatureReduction abstract class.
*/

#ifndef BASE_FEATURE_REDUCTION_H__
#define BASE_FEATURE_REDUCTION_H__

// OpenCV
#include <cv.h>
#include "TraceMessage.h"

namespace kifas
{

/**
 The Appreance code Abstract class. It defines how to get frames.
 @ingroup featureReduction
*/
class BaseFeatureReduction {
public:

  BaseFeatureReduction() {}
	
	/**
	 perform Preprocessing
	 @return Mat image after preprocessing.	
	*/
  virtual void cv::Mat RunReduction(cv::Mat &input) = 0;

};
}

#endif /* BASE_FEATURE_REDUCTION_H__ */