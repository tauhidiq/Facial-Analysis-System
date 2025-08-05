/**
* @file 
* Defines the BaseFeatureRepresentation abstract class.
*/

#ifndef BASE_FEATURE_REPRESENTATION_H__
#define BASE_FEATURE_REPRESENTATION_H__

// OpenCV
#include <cv.h>
#include "TraceMessage.h"

namespace kifas
{

/**
 The Preprocessing Abstract class. It defines how to get frames.
 @ingroup featureRepresentation
*/
class BaseFeatureRepresentation {
public:

  BaseFeatureRepresentation() {
	mCodeDiversity= 0;
  }
	
  /**
  * Virtual function that extracts the feature vector from the input image.
  * @param input the image to extract the feature vector from.
  * @return the feature vector length.
  */
  virtual cv::Mat runFeatureVectors(std::vector<cv::Mat> &input) = 0;
  
  /**
  * Gets the length of the feature vector.
  * @return the length of the feature vector.
  */
  virtual int getFeatureLength() = 0;

  /**
  * Gets the type of the feature vector. 
  * The types that will return are the same as constants used in OpenCV, for example, CV_32S, CV_32F, CV_8U etc. 
   */
  virtual int getFeatureType() =0;

  void setCodeSize(int size){
	  mCodeDiversity = size;
  }

  void setSequenceSize(int size){
      mSequenceSize  = size;
  }

protected:
	int mCodeDiversity;
    int mSequenceSize;
};

}

#endif /* BASE_FEATURE_REPRESENTATION_H__ */