#include "TemplateMatching.h"
#include "Parser.h"
#include "ConfigFileManager.h"
#include "Definitions.h"

TemplateMatching::TemplateMatching()
{ 

  mConfigFile = "TemplateMatching.conf";
  // Defaults values from config file
  std::map<std::string, kifas::NumStr> defaults;
  defaults["distance"] = "ChiSquare";
  
  // load the configuration file
  //kifas::ConfigFileManager cf(mConfigFile, defaults);
  // set the values
//  mDistance = kifas::DistanceFactory::instance().create(cf.getValue("distance"));
	
}

TemplateMatching::~TemplateMatching()
{

}


/**
  Performs the classification using template matching. In this case it test each test vector against all the training data; and picks the best match.
  @return classification score of the training and testing data.
*/
void TemplateMatching::runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData,std::vector<int> &testResultTable)
{
	//return 0.0;
}

float TemplateMatching::runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData)
{
    return 0.0;
}
