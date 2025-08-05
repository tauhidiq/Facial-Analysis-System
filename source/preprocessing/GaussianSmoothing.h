/**
* @file
* Defines the GaussianSmoothing class.
*/

#ifndef GAUSSIAN_SMOOTHING_PREPROCESSING_H__
#define GAUSSIAN_SMOOTHING_PREPROCESSING_H__

// OpenCV
#include <cv.h>
#include "BasePreprocessing.h"


/**
 The Preprocessing Abstract class. It defines how to get frames.
 @ingroup preprocessing
*/
class GaussianSmoothing : public kifas::BasePreprocessing {
public:

  GaussianSmoothing() {}
  ~GaussianSmoothing() {}

  /**
    perform Preprocessing
	@return Mat image after preprocessing.	
  */
  //void RunPreprocessing(cv::Mat &input);
  virtual void RunPreprocessing(std::vector<cv::Mat> &input);
protected:

};

#endif /* SMOOTHING_PREPROCESSING_H__ */