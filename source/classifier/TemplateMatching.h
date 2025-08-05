#ifndef TEMPLETE_MATCHING_PREPROCESSING_H__
#define TEMPLETE_MATCHING_PREPROCESSING_H__

// OpenCV
#include <cv.h>
#include "BaseClassifier.h"
#include "BaseDistance.h"

/**
  The Preprocessing Abstract class. It defines how to get frames.
  @ingroup classifier 
*/
class TemplateMatching : public kifas::BaseClassifier {
public:

  TemplateMatching(); 
  ~TemplateMatching();

 virtual void runClassification(cv::Mat trainningData,
						 std::vector<int> &trainningTruthTable,
						 cv::Mat testData,
						 std::vector<int> &testResultTable);

 virtual float runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData);

private:
  std::string mConfigFile;// = "TemplateMatching.conf";
  cv::Ptr<kifas::BaseDistance> mDistance;
};

#endif
