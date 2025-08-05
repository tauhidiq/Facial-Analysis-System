#include "SVM.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "Definitions.h"
#include "ConfigFileManager.h"
#include "Parser.h"

#include "opencv2/core/types_c.h"

#include <string>

SVM::SVM()
{
	mConfigFile = "SVM.conf";
  
	// Defaults values from config file
	std::map<std::string, kifas::NumStr> svmconfig;
	svmconfig["svm_type"] = "c_svc";
	svmconfig["cvalue"] = 10.0;
	svmconfig["nuvalue"] = 0.5;
	svmconfig["pvalue"] = 0.1;

	svmconfig["kernelfunction"] = "rbf";
	svmconfig["degree"] = 10.0;
	svmconfig["gamma"] = 8.0;
	svmconfig["coef0"] = 1.0;

	// load the configuration file
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, svmconfig);

	// get svm_type
	std::string attrbutevalue = svmconfig["svm_type"];
	// convert capitals to lower
	std::transform(attrbutevalue.begin(), attrbutevalue.end(), attrbutevalue.begin(), ::tolower);
	if(attrbutevalue.compare("c_svc") == 0)
		m_svm_type = CvSVM::C_SVC;
	else if(attrbutevalue.compare("nu_svc") == 0)
		m_svm_type = CvSVM::NU_SVC;
	else if(attrbutevalue.compare("one_class") == 0)
		m_svm_type = CvSVM::ONE_CLASS;
	else if(attrbutevalue.compare("eps_svr") == 0)
		m_svm_type = CvSVM::EPS_SVR;
	else if(attrbutevalue.compare("nu_svr") == 0)
		m_svm_type = CvSVM::NU_SVR;
	
	// get kernelfunction
	attrbutevalue = std::string(svmconfig["kernelfunction"]);
	// convert capitals to lower
	std::transform(attrbutevalue.begin(), attrbutevalue.end(), attrbutevalue.begin(), ::tolower);
	if(attrbutevalue.compare("linear") == 0)
		m_kernel_type = CvSVM::LINEAR;
	else if(attrbutevalue.compare("poly") == 0)
		m_kernel_type = CvSVM::POLY;
	else if(attrbutevalue.compare("rbf") == 0)
		m_kernel_type = CvSVM::RBF;
	else if(attrbutevalue.compare("sigmoid") == 0)
		m_kernel_type = CvSVM::SIGMOID;
	

	m_degree = svmconfig["degree"]; // get degree value
	m_gamma  = svmconfig["gamma"];	// get gamma value
	m_coef0  = svmconfig["coef0"];	// get coef0
	m_cvalue = svmconfig["cvalue"];	// get cvalue
	m_nu	 = svmconfig["nuvalue"];	// get nu value
	m_p		 = svmconfig["pvalue"];	// get p value
/*
	int i,j;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			cfmatrix[i][j] = 0;*/
}

SVM::~SVM()
{
	//int i,j;
	//std::ofstream file;
	//file.open("cf.txt",std::ofstream::out | std::ofstream::app);
	//file<<"Confusion Matrix"<<std::endl;
	//for(i=0; i<8; i++){
	//	for(j=0; j<8; j++){
	//		file<<cfmatrix[i][j]<<"   ";
	//	}
	//	file<<std::endl;
	//}
}



void SVM::runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData,std::vector<int> &testResultTable)
{

	unsigned int NumTest = testData.size().height;
	unsigned int NumTrain= trainningData.size().height;
	unsigned int Dimensions = testData.size().width;


	// finding maximum value
	int i,j;
	//int max_train =0;
	double max_train = findMaxValue(trainningData);
	double max_test =  findMaxValue(testData);
	//for(i=0; i< NumTrain; i++)
	//{
	//	int * TrainPtr = trainningData.ptr<int>(i);
	//	for(j=0; j<Dimensions; j++)
	//	{
	//		if( TrainPtr[j]  > max_train)
	//			max_train = TrainPtr[j];
	//	}
	//}
	//
	//int max_test =0;
	//for(i=0; i< NumTest; i++)
	//{
	//	int * TestPtr = testData.ptr<int>(i);
	//	for(j=0; j<Dimensions; j++)
	//	{
	//		if( TestPtr[j]  > max_test)
	//			max_test = TestPtr[j];
	//	}
	//}
	
	double maxvalue;
	if(max_train > max_test)
		maxvalue = max_train;
	else
		maxvalue = max_test;
	

	cv::Mat trainningClass;
	trainningClass.create(NumTrain,1,CV_32SC1);
	
	cv::Mat traingDataFloat;
	trainningData.convertTo(traingDataFloat,CV_32F,1.0 / maxvalue);

	

    //int testSize = CV_ELEM_SIZE1(cv::DataType<int>::depth);
	// Make truth table of training data
	for(i=0; i < trainningTruthTable.size(); i++)
		trainningClass.at<int>(i,0) =  trainningTruthTable[i];

	// SVM �н� �Ű� ������ ������ ���� �����Ѵ�.
	// svm ����  : m_svm_type
	// Ŀ�� �Լ� : m_kernel_type
	// degree    : m_degree
	// gamma     : m_gamma
	// coef0	 : m_coef0
	// C         : m_cvalue
	// nu        : m_nu
	// p         : m_p
	CvTermCriteria criteria = cvTermCriteria (CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-6);

	CvSVMParams param (m_svm_type,m_kernel_type, m_degree, m_gamma, m_coef0, m_cvalue, m_nu,m_p, NULL, criteria);
	
	CvSVM svm;
	
	svm.train(traingDataFloat,trainningClass,cv::Mat(), cv::Mat(), param);
	//svm.train_auto(traingDataFloat,trainningClass,cv::Mat(),cv::Mat(),param,10);
	
    //svm.save("d:\\genderSVMModel.xml");

	int WellClasifiedNumber = 0;

	// confusion matrix
	//int cfmatrix[7][7];
	


    //TraceMessage::addMessageLine("Test Results");
    cv::Mat testDataFloat;
    testData.convertTo(testDataFloat,CV_32F,1.0 / maxvalue);
	for(i=0; i< NumTest; i++)
	{
		cv::Mat testSample = testDataFloat(cv::Rect(0,i,Dimensions,1));
		//testSample.convertTo(testSample,CV_32F,1.0 / (double)maxvalue);

		float response = svm.predict (testSample);

        //TraceMessage::addMessageLine(response);

		int ClassificationResult = cvRound(response);
        testResultTable[i] = ClassificationResult;
        //== testTruthTable[i])
		//	WellClasifiedNumber++;

		//cfmatrix[testTruthTable[i]][cvRound(response)]++;
	}

	

	//return  (double)WellClasifiedNumber / (double)NumTest;
}

double SVM::findMaxValue(cv::Mat data)
{
	unsigned int numData = data.size().height;
	unsigned int numDimension = data.size().width;

	int dataType = data.type();
	int i,j;
	double max=0;

	if(dataType == CV_8U)
	{
		for(i=0; i< numData; i++)
		{
			uchar * TrainPtr = data.ptr<uchar>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}

	}
	else if(dataType == CV_8S)
	{
		for(i=0; i< numData; i++)
		{
			char * TrainPtr = data.ptr<char>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}
	}
	else if(dataType == CV_16U)
	{
		for(i=0; i< numData; i++)
		{
			unsigned short * TrainPtr = data.ptr<unsigned short>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}
	}
	else if(dataType == CV_16S)
	{
		for(i=0; i< numData; i++)
		{
			short * TrainPtr = data.ptr<short>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}
	}
	else if(dataType == CV_32S)
	{
		for(i=0; i< numData; i++)
		{
			int * TrainPtr = data.ptr<int>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}
	}
	else if(dataType == CV_32F)
	{
		for(i=0; i< numData; i++)
		{
			float * TrainPtr = data.ptr<float>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}
	}
	else if(dataType == CV_64F)
	{
		for(i=0; i< numData; i++)
		{
			double * TrainPtr = data.ptr<double>(i);
			for(j=0; j<numDimension; j++)
			{
				if( TrainPtr[j]  > max)
					max = TrainPtr[j];
			}
		}
	}
	
	return max;
}

float SVM::runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData)
{
    unsigned int NumTest = testData.size().height;
	unsigned int NumTrain= trainningData.size().height;
	unsigned int Dimensions = testData.size().width;


	// finding maximum value
	int i,j;
	//int max_train =0;
	double max_train = findMaxValue(trainningData);
	double max_test =  findMaxValue(testData);
	//for(i=0; i< NumTrain; i++)
	//{
	//	int * TrainPtr = trainningData.ptr<int>(i);
	//	for(j=0; j<Dimensions; j++)
	//	{
	//		if( TrainPtr[j]  > max_train)
	//			max_train = TrainPtr[j];
	//	}
	//}
	//
	//int max_test =0;
	//for(i=0; i< NumTest; i++)
	//{
	//	int * TestPtr = testData.ptr<int>(i);
	//	for(j=0; j<Dimensions; j++)
	//	{
	//		if( TestPtr[j]  > max_test)
	//			max_test = TestPtr[j];
	//	}
	//}
	
	double max;
	if(max_train > max_test)
		max = max_train;
	else
		max = max_test;
	

	cv::Mat trainningClass;
	trainningClass.create(NumTrain,1,CV_32SC1);
	
	cv::Mat traingDataFloat;
	trainningData.convertTo(traingDataFloat,CV_32F,1.0 / max);

	
	
	// Make truth table of training data
    int *trainingClassPtr;
	for(i=0; i < trainningTruthTable.size(); i++)
    {
        trainingClassPtr = trainningClass.ptr<int>(i);
        trainingClassPtr[0] = trainningTruthTable[i];
    }
	//	trainningClass.at<int>(i,0) =  trainningTruthTable[i];

	// SVM �н� �Ű� ������ ������ ���� �����Ѵ�.
	// svm ����  : m_svm_type
	// Ŀ�� �Լ� : m_kernel_type
	// degree    : m_degree
	// gamma     : m_gamma
	// coef0	 : m_coef0
	// C         : m_cvalue
	// nu        : m_nu
	// p         : m_p
	CvTermCriteria criteria = cvTermCriteria (CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-6);

	CvSVMParams param (m_svm_type,m_kernel_type, m_degree, m_gamma, m_coef0, m_cvalue, m_nu,m_p, NULL, criteria);
	
	CvSVM svm;
	
	svm.train(traingDataFloat,trainningClass,cv::Mat(), cv::Mat(), param);
	//svm.train_auto(traingDataFloat,trainningClass,cv::Mat(),cv::Mat(),param,10);
	


	int WellClasifiedNumber = 0;

	// confusion matrix
	//int cfmatrix[7][7];
	



	//for(i=0; i< NumTest; i++)
	//{
		cv::Mat testSample = testData(cv::Rect(0,0,Dimensions,1));
		testSample.convertTo(testSample,CV_32F,1.0 / (double)max);

		float response = svm.predict (testSample);

		//if(cvRound(response) == testTruthTable[i])
		//	WellClasifiedNumber++;

		//cfmatrix[testTruthTable[i]][cvRound(response)]++;
	//}

	

	return  (float)cvRound(response);

}
