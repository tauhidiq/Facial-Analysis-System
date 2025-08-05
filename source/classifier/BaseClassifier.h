/**
  @file BaseClassifier.h
  Defines the abstract class of the Classifiers.
*/
#ifndef BASE_CLASSIFIER_H__
#define BASE_CLASSIFIER_H__

// OpenCV
#include <cv.h>
#include <vector>
#include "TraceMessage.h"

namespace kifas
{

/**
  The Classifier Abstract class, it defines how to classify the test images given some train files.
  @ingroup classifier
*/
class BaseClassifier {
public:

  BaseClassifier() {}
  virtual ~BaseClassifier(){}
	/**
	  Performs the classification
      @param trainningData feature set for training(Width = size of a feature vector, Height = the number of feature vectors)
      @param trainningTruthTable Truth table of train data for classification classes
      @param testData feature set for test(Width = size of a feature vector, Height = the number of feature vectors)
      @param testResultTable the table of test result
	*/
	virtual void runClassification(cv::Mat trainningData,
						                       std::vector<int> &trainningTruthTable,
						                       cv::Mat testData,
						                       std::vector<int> &testResultTable) = 0;

    virtual float runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,
						                       cv::Mat testData)=0;

	//virtual void Test() =0;
};

}

#endif /* BASE_CLASSIFIER_H__ */