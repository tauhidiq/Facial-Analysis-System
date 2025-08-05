#ifndef SUPPORT_VECTOR_MACHINE_H
#define SUPPORT_VECTOR_MACHINE_H

// OpenCV
#include <cv.h>
#include "BaseClassifier.h"

/**
 The Preprocessing Abstract class. It defines how to get frames.
 @ingroup core
*/
class SVM : public kifas::BaseClassifier {
public:

	SVM(); 
	~SVM(); 

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

	double findMaxValue(cv::Mat data);
protected:
	std::string mConfigFile;

	int m_svm_type;
	int m_kernel_type;
	double m_degree;
	double m_gamma;
	double m_coef0;
	double m_cvalue;
	double m_nu;
	double m_p;


	//int cfmatrix[8][8];

   //virtual void Test(){
	// int a;
	// a = 10;
 //}
};

#endif /* SMOOTHING_PREPROCESSING_H__ */