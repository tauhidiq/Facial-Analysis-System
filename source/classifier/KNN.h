#ifndef K_NEAREST_NEIGHBOR_H_H
#define K_NEAREST_NEIGHBOR_H_H

// OpenCV
#include <cv.h>
#include "BaseClassifier.h"

/**
 The Preprocessing Abstract class. It defines how to get frames.
 @ingroup core
*/
class KNN : public kifas::BaseClassifier {
public:

  KNN();
  ~KNN();

  /**
    perform Preprocessing
	@return Mat image after preprocessing.	
  */
virtual void runClassification(cv::Mat trainningData,
						 std::vector<int> &trainningTruthTable,
						 cv::Mat testData,
						 std::vector<int> &testResultTable);

virtual float runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,
						                       cv::Mat testData);

protected:
	unsigned int mK;
};

#endif /* SMOOTHING_PREPROCESSING_H__ */