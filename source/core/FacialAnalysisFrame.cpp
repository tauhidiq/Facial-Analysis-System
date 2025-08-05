#include "FacialAnalysisFrame.h"
#include "Definitions.h"
#include <algorithm>    // std::transform
//#include <ctime>

kifas::FacialAnalysisFrame::FacialAnalysisFrame()// : mConfigFileManager("facial_analysis.conf")
{
	//ConfigFileManager				mConfigFileManager("facial_analysis.conf");
	mPersonIndepent = false;
	mAppend = false;
	mFolds = false;
}

kifas::FacialAnalysisFrame::~FacialAnalysisFrame()
{

}


void kifas::FacialAnalysisFrame::traceConfigResult()
{
	std::list<std::string>::iterator it,it2;

	TraceMessage::addMessage("RunType = ");
  switch(mRunType)
  {
  case TRAIN:
    TraceMessage::addMessageLine("Train only");
    break;
  case TEST:
    TraceMessage::addMessageLine("Test only");
    break;
  case BOTH:
    TraceMessage::addMessageLine("Train and test");
    break;
  }	

  TraceMessage::addMessage("TrainMode = ");
  switch(mTrainMode)
  {
  case SEPARATE:
    TraceMessage::addMessageLine("Separate");
    break;
  case FOLD:
    TraceMessage::addMessageLine("Folds");
    if (mFolds > 0)
    {
      TraceMessage::addMessage("No. of Folds = ");
      TraceMessage::addMessageLine(mFolds);
    }
    break;
  }
  
  //if(mImageFileList != "")
  //{
  //  TraceMessage::addMessage("ImageListFile = ");
	 // TraceMessage::addMessageLine(mImageFileList);
  //}
  if(mImageFileListTrain != "")
  {
    TraceMessage::addMessage("ImageListFileTrain = ");
	TraceMessage::addMessageLine(mImageFileListTrain);
  }

  if((mImageFileListTest != "") && (mTrainMode == SEPARATE))
  {
    TraceMessage::addMessage("ImageListFileTest = ");
	  TraceMessage::addMessageLine(mImageFileListTest);
  }  

  if(mTrainImageDir != "")
  {
    TraceMessage::addMessage("ImageDirTrain = ");
	TraceMessage::addMessageLine(mTrainImageDir);
  }

  if((mTestImageDir != "") && (mTrainMode == SEPARATE))
  {
    TraceMessage::addMessage("ImageDirTest = ");
	TraceMessage::addMessageLine(mTestImageDir);
  }
  //TraceMessage::addMessage("ImageDir = ");
  //TraceMessage::addMessageLine(mImageDir);


	bool valid;
	std::string string_lawcase;
	// Preprocessing, AppearanceCode, FeatureRepresentation
	TraceMessage::addMessage("Preprocessing  = ");
	for(it = mPreprocessing.begin(); it != mPreprocessing.end(); it++)
	{
		bool valid = false;
		
		// check whether input preprocessing names are valid or not
		for( it2 = mPreprocessingList.begin(); it2 != mPreprocessingList.end(); it2++)
		{
			// transform to lower case the values
			string_lawcase = *it2;
			std::transform(string_lawcase.begin(), string_lawcase.end(), string_lawcase.begin(), ::tolower);
			if( string_lawcase.compare(*it) == 0)
			{
				valid = true;

				*it = *it2;
				break;
			}
		}

		TraceMessage::addMessage(*it);
		TraceMessage::addMessage(",");
	}
	TraceMessage::addMessageLine("");


	for(it = mAppearanceCodeList.begin(); it != mAppearanceCodeList.end(); it++)
	{
		bool valid = false;
		
		// check whether input preprocessing names are valid or not
		string_lawcase = *it;
		std::transform(string_lawcase.begin(), string_lawcase.end(), string_lawcase.begin(), ::tolower);
		if( string_lawcase.compare(mAppearanceCode) == 0)
		{
			valid = true;
			mAppearanceCode = *it;
			break;
		}
	}
	TraceMessage::addMessage("AppearanceCode = ");
	TraceMessage::addMessageLine(mAppearanceCode);


	for(it = mFeatureRepresentationList.begin(); it != mFeatureRepresentationList.end(); it++)
	{
		bool valid = false;
		
		// check whether input preprocessing names are valid or not
		string_lawcase = *it;
		std::transform(string_lawcase.begin(), string_lawcase.end(), string_lawcase.begin(), ::tolower);
		if( string_lawcase.compare(mFeatureRepresentation) == 0)
		{
			valid = true;
			mFeatureRepresentation = *it;
			break;
		}
	}
	TraceMessage::addMessage("FeatureRepresentation = ");
	TraceMessage::addMessageLine(mFeatureRepresentation);


	for(it = mFeatureReductionList.begin(); it != mFeatureReductionList.end(); it++)
	{
		bool valid = false;
		
		// check whether input preprocessing names are valid or not
		string_lawcase = *it;
		std::transform(string_lawcase.begin(), string_lawcase.end(), string_lawcase.begin(), ::tolower);
		if( string_lawcase.compare(mFeatureReduction) == 0)
		{
			valid = true;
			mFeatureReduction = *it;
			break;
		}
	}
	TraceMessage::addMessage("FeatureReduction = ");
	TraceMessage::addMessageLine(mFeatureReduction);



	for(it = mClassifierList.begin(); it != mClassifierList.end(); it++)
	{
		bool valid = false;
		
		// check whether input preprocessing names are valid or not
		string_lawcase = *it;
		std::transform(string_lawcase.begin(), string_lawcase.end(), string_lawcase.begin(), ::tolower);
		if( string_lawcase.compare(mClassifier) == 0)
		{
			valid = true;
			mClassifier = *it;
			break;
		}
	}
	TraceMessage::addMessage("Classifier = ");
	TraceMessage::addMessageLine(mClassifier);
	TraceMessage::addMessageLine("");
}

/**
  This function parses the configuration file: facial_analysis.conf.
*/
void kifas::FacialAnalysisFrame::readSystemConfigFile()
{
	
	std::string SystemConfigFile = "facial_analysis.conf";
	//std::string SystemConfigFilePath;
	//SystemConfigFilePath = getConfigFilesDir() + "facial_analysis.conf";
	

	std::map<std::string, kifas::NumStr> SystemConfig;
	SystemConfig["runtype"] = "both";							// Run Type "train", "test", "both"
	SystemConfig["cache"] = "false";								// true | false
	SystemConfig["traincachefile"] ="train.db";				// default cache "train.db"
	SystemConfig["testcachefile"] ="test.db";				// default cache "test.db"
	SystemConfig["append"] ="false";						// default false
	
	SystemConfig["trainmode"] ="folds";						// seperate , fold
	
	// for trainMode = seperate
	//SystemConfig["imagelistfiletrain"] ="";		// ex) "F:\Documents\Data set\FERET-Cropped\norm-rot\fa.lst"
	//SystemConfig["imagelistfiletest"] = "";		

	// for TrainMode = fold
	SystemConfig["folds"] = "10";								// default = 10
	//SystemConfig["imagelistfile"] = "";				        // image file list
	
	// for list files for training and testing
	SystemConfig["imagelistfiletrain"] = "";				// image file list for train
	SystemConfig["imagelistfiletest"]  = "";				// image file list for test
	
	// for directory of images for training and testing
	SystemConfig["imagedirtrain"] = "";						// directory of image files for train
	SystemConfig["imagedirtest"] = "";						// directory of image files for test

	// for person independent
	SystemConfig["personindependent"] = "false";			// if true, perseon independent, or person dependent
	
	// ColOrder
	// 0 -> name perId expId
	// 1 -> name anyId
	// 2 -> name
	SystemConfig["colorder"] = 1;						    // image file list
	

	//SystemConfig["imagedir"] = "";
	


	SystemConfig["preprocessing"] = "";
	SystemConfig["appearancecode"] = "ldp";
	SystemConfig["featurerepresentation"] ="uniformhistogram";
	SystemConfig["featurereduction"] = "";
	SystemConfig["classifier"] = "templatematching";

	// read config file
	kifas::ConfigFileManager::ConfigFileRead(SystemConfigFile, SystemConfig);
	
	// run type
	std::string attrbutevalue = SystemConfig["runtype"];
	if (attrbutevalue.compare("train") == 0)
		mRunType = TRAIN;
	else if (attrbutevalue.compare("test") == 0) 
		mRunType = TEST;
	else if (attrbutevalue.compare("both") == 0 )
		mRunType = BOTH;

	// cache
	attrbutevalue = std::string(SystemConfig["cache"]);
	if (attrbutevalue.compare("true") == 0)
		mCache = true;
	else
		mCache = false;

	// TrainCacheFile
	mTrainCacheFile = std::string(SystemConfig["traincachefile"]);
	
	// TestCacheFile
	mTestCacheFile = std::string(SystemConfig["testcachefile"]);

	// append
	attrbutevalue = std::string(SystemConfig["append"]);
	if (attrbutevalue.compare("true") == 0)
		mAppend = true;
	else
		mAppend = false;

	// trainning mode
	attrbutevalue = std::string(SystemConfig["trainmode"]);
	if (attrbutevalue.compare("separate") == 0)
		mTrainMode = SEPARATE;
	else
		mTrainMode = FOLD;
	
	mFolds = SystemConfig["folds"];
	

	mImageFileListTrain	= std::string(SystemConfig["imagelistfiletrain"]);
	mImageFileListTest = std::string(SystemConfig["imagelistfiletest"]);
	mTrainImageDir= std::string(SystemConfig["imagedirtrain"]);
	mTestImageDir= std::string(SystemConfig["imagedirtest"]);

	// cache
	attrbutevalue = std::string(SystemConfig["personindependent"]);
	if (attrbutevalue.compare("true") == 0)
		mPersonIndepent = true;
	else
		mPersonIndepent = false;

	mColOrder =	SystemConfig["colorder"];
	if(mColOrder < 0 || mColOrder > 4)
	{
		throw KifasExpeption("[FacialAnalysisFrame::readSystemConfigFile] : the value of \"colorder\" in facial_analysis.conf is not valid.");
	}

    // Check person information is contained or not
	if( !( (mColOrder == 0) || (mColOrder == 3)) && mPersonIndepent)
	{
		TraceMessage::addMessage("Because PersonIndependent is set  as true but ColOrder is not 0 in facial_analysis.conf, PersonIndependent will be set false");
		mPersonIndepent = false;
	}

	//	mImageFileList = SystemConfig["ImageListFile"];
//mImageDir = SystemConfig["ImageDir"];


	mAppearanceCode = std::string(SystemConfig["appearancecode"]);
	mFeatureRepresentation = std::string(SystemConfig["featurerepresentation"]);
	mFeatureReduction =	std::string(SystemConfig["featurereduction"]);
	mClassifier = std::string(SystemConfig["classifier"]);

	attrbutevalue = std::string(SystemConfig["preprocessing"]);

	if(attrbutevalue.compare("") != 0)
	{
		size_t current;
		size_t next = -1;
		do
		{
			current =next +1;
			next = attrbutevalue.find_first_of(",",current);

            if(next != std::string::npos)
			    mPreprocessing.push_back(attrbutevalue.substr(next-current));
            else
                mPreprocessing.push_back(attrbutevalue);

		}while(next != std::string::npos);
	}

	return;
}
void kifas::FacialAnalysisFrame::traceListString(std::list<std::string> &list)
{
	std::list<std::string>::iterator it;

	for(it = list.begin(); it != list.end(); it++)
	{
		TraceMessage::addMessage("    -> ");
		TraceMessage::addMessageLine(*it);
	}
}

/**
  Loads the class list from the file pointed by path into the list.
  @param list is the final list of classes.
  @param path is the file path which contains a list of classes to register.
*/

void kifas::FacialAnalysisFrame::readClassList(std::list<std::string> &list, std::string path)
{
	std::string ListFilePath;
	std::string ClassName;
	ListFilePath = kifas::ConfigFileManager::getConfigFilesDir() + path;

	std::ifstream ListFile;
	
	ListFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

	try
	{
		ListFile.open(ListFilePath.c_str(),std::ios_base::in);
	}
	catch(std::ifstream::failure &e)
	{
		std::cout<<e.what()<<std::endl;
		throw KifasExpeption("[FacialAnalysisFrame::readClassList] :While opening \"" + ListFilePath + "\", an exception occurred!!\n"+e.what());
		//ListFile.close();
		//return false;
	}

	try
	{
		while(!ListFile.eof())
		{
			ListFile>>ClassName;
			list.push_back(ClassName);
		}
	}
	catch(std::ifstream::failure &e)
	{
		if(ListFile.eof() && ListFile.fail())
		{
			ListFile.close();
			return;
		}

		ListFile.close();
		throw KifasExpeption("[FacialAnalysisFrame::readClassList] : While reading \"" + ListFilePath + "\", an exception occurred!!\n"+e.what());
	}

	ListFile.close();

	return;
}


void kifas::FacialAnalysisFrame::readConfigFiles()
{	
	// read files which store class lists
	try
	{
		TraceMessage::addMessageLine("Reading the list of pre-processing classes");
		readClassList(mPreprocessingList,"preprocessing_class_list");
		traceListString(mPreprocessingList);
		TraceMessage::addMessageLine("");

		TraceMessage::addMessageLine("Reading the list of appearance code classes");
		readClassList(mAppearanceCodeList,"appearancecode_class_list");
		traceListString(mAppearanceCodeList);
		TraceMessage::addMessageLine("");

		TraceMessage::addMessageLine("Reading the list of feature representation classes");
		readClassList(mFeatureRepresentationList,"featurerepresentation_class_list");
		traceListString(mFeatureRepresentationList);
		TraceMessage::addMessageLine("");

		TraceMessage::addMessageLine("Reading the list of feature reduction classes");
		readClassList(mFeatureReductionList,"featurereduction_class_list");
		traceListString(mFeatureReductionList);
		TraceMessage::addMessageLine("");

		TraceMessage::addMessageLine("Reading the list of classifier classes");
		readClassList(mClassifierList,"classifier_class_list");
		traceListString(mClassifierList);
		TraceMessage::addMessageLine("");


		readSystemConfigFile();

		traceConfigResult();
	}
	catch(KifasExpeption &e)
	{		
		throw;
	}
	
}

/**
* Validates the inputVector and the cache sizes and types, and inserts the vector in the specified position.
* @param inputVector is the vector to insert.
* @param cache is the larger matrix to store the vector.
* @param row is the row number to insert the inputVector.
*/
void kifas::FacialAnalysisFrame::copyFeatureVectorData(cv::Mat inputvector, cv::Mat trainningVector, const int train_number)
{
	// 1. check if train_number is valid or not
	// 2. check if inputvecotr data type
	// 3. make code to copy data according to each data type
	
	int TrainXsize = trainningVector.size().width;
	int TrainYsize = trainningVector.size().height;

	int InputVectorXsize = inputvector.size().width;
	int InputVectorYsize = inputvector.size().height;

	int TrainDataType = trainningVector.type();
	int InputDataType = inputvector.type();

	// if train_number is out of range
	if(! ((train_number >=0) && (train_number < TrainYsize)))
	{
		//std::cout<<"void kifas::FacialAnalysisFrame::CopyFeatureVectorData(cv::Mat inputvector,cv::Mat trainningVector,int train_number) makes errors"<<std::endl;
		//std::cout<<"int train_number is out of range "<<std::endl;
		
		throw KifasExpeption("[FacialAnalysisFrame::copyFeatureVectorData] : int train_number is out of range ");
	}

	// if vector size is different 
	if(TrainXsize != InputVectorXsize)
	{
		//std::cout<<"void kifas::FacialAnalysisFrame::CopyFeatureVectorData(cv::Mat inputvector,cv::Mat trainningVector,int train_number) makes errors"<<std::endl;
		//std::cout<<"cv::Mat inputvector and cv::Mat trainningVector have different vector size."<<std::endl;
		
		throw KifasExpeption("[FacialAnalysisFrame::copyFeatureVectorData] : cv::Mat inputvector and cv::Mat trainningVector have different vector size.");
	}

	// if train_number is out of range
	if(InputVectorYsize > 1)
	{
		//std::cout<<"void kifas::FacialAnalysisFrame::CopyFeatureVectorData(cv::Mat inputvector,cv::Mat trainningVector,int train_number) makes errors"<<std::endl;
		//std::cout<<"inputvector should be 1 height size "<<std::endl;
		
		throw KifasExpeption("[FacialAnalysisFrame::copyFeatureVectorData] : inputvector should be 1 height size.");
	}

	// if train_number is out of range
	if(TrainDataType !=  InputDataType)
	{
		//std::cout<<"void kifas::FacialAnalysisFrame::CopyFeatureVectorData(cv::Mat inputvector,cv::Mat trainningVector,int train_number) makes errors"<<std::endl;
		//std::cout<<"cv::Mat inputvector and cv::Mat trainningVector have different data type"<<std::endl;
		throw KifasExpeption("[FacialAnalysisFrame::copyFeatureVectorData] : cv::Mat inputvector and cv::Mat trainningVector have different data type.");
	}
	
	void *dst;
	void *src;
	int DataByteSize=0;
	switch(TrainDataType)
	{
		case CV_8U:
			src = inputvector.ptr<uchar>(0);
			dst = trainningVector.ptr<uchar>(train_number);

			DataByteSize =1;
			break;

		case CV_8S:
			src = inputvector.ptr<schar>(0);
			dst = trainningVector.ptr<schar>(train_number);
			DataByteSize =1;
			break;

		case CV_16U:
			src = inputvector.ptr<ushort>(0);
			dst = trainningVector.ptr<ushort>(train_number);
			DataByteSize =2;
			break;

		case CV_16S:
			src = inputvector.ptr<short>(0);
			dst = trainningVector.ptr<short>(train_number);
			DataByteSize =2;
			break;

		case CV_32S:
			src = inputvector.ptr<int>(0);
			dst = trainningVector.ptr<int>(train_number);
			DataByteSize =4;
			break;

		case CV_32F:
			src = inputvector.ptr<float>(0);
			dst = trainningVector.ptr<float>(train_number);
			DataByteSize =4;
			break;

		case CV_64F:
			src = inputvector.ptr<double>(0);
			dst = trainningVector.ptr<double>(train_number);
			DataByteSize =8;
			break;

		//case CV_USRTYPE1:
		//	break;
	}

	// if data type is in valid 
	if(DataByteSize == 0)
	{
		//std::cout<<"void kifas::FacialAnalysisFrame::CopyFeatureVectorData(cv::Mat inputvector,cv::Mat trainningVector,int train_number) makes errors"<<std::endl;
		//std::cout<<"inputvector and Trainning vector don't have one of CV_8U, CV_8S, CV_16U, CV_16S, CV_32S, CV_32F, CV_64F"<<std::endl;
		throw KifasExpeption("[FacialAnalysisFrame::copyFeatureVectorData] : inputvector and Trainning vector don't have one of CV_8U, CV_8S, CV_16U, CV_16S, CV_32S, CV_32F, CV_64F.");
	}

	memcpy(dst,src,DataByteSize * TrainXsize);
}

/**
  Main process that runs all the things.
  It reads the configuration files, and process them.
*/
bool kifas::FacialAnalysisFrame::Run()
{	
    // write log file automatically
    TraceMessage::setFileWriteFlage(true);

	//LDP::GetDir();
	try{
		// for loop
		int i;

		TraceMessage::addMessageLine("KyungHee Image-Processing-Lab Facial Analysis System v0.1");

		// read config files
		TraceMessage::addMessageLine("Reading the config file.");
		readConfigFiles();		

		// set preprocessing algorithms
		std::list<std::string>::iterator it;
		for(it = mPreprocessing.begin(); it != mPreprocessing.end(); it++)
			mPreprocessingManager.addPreprocessingInstanceToList(*it);

		// set a Appearance code algorithm
		if( !mAppearanceCodeManager.selectAppearanceCode(mAppearanceCode))
		{
			//TraceMessage::addError("The appearance coding algorithm \"" + mAppearanceCode + "\" is not register properly in the system, or does not exist.");
			throw KifasExpeption("[FacialAnalysisFrame::Run] : The appearance coding algorithm \"" + mAppearanceCode + "\" is not register properly in the system, or does not exist.");
		}

		// set a feature representation algorithm
		if( !mFeatureRepresentationManager.SelectFeatureRepresentation(mFeatureRepresentation))
		{
			//TraceMessage::addError("The feature representation algorithm \"" + mFeatureRepresentation + "\" is not register properly in the system, or does not exist.");
			throw KifasExpeption("[FacialAnalysisFrame::Run] :The feature representation algorithm \"" + mFeatureRepresentation + "\" is not register properly in the system, or does not exist.");
		}

		// set a classifier algorithm
		if( !mClassifierManager.selectClassifer(mClassifier) )
		{
			//TraceMessage::addError("The classifier \"" + mClassifier + "\" is not register properly in the system, or does not exist.");
			throw KifasExpeption("[FacialAnalysisFrame::Run] : The classifier \"" + mClassifier + "\" is not register properly in the system, or does not exist.");
		}

		// Process depending on the type of train mode
		// In both cases (fold or separate), train all the images
  
		//ImageFileManager trainFiles;
		
		
		
		// set the file of list to store names of image files
		if ( mTrainMode == FacialAnalysisFrame::FOLD )
		{
			// set the path of image files
			mTrainImageFiles.setImageFileDir(mTrainImageDir);
			mTrainImageFiles.setColOrder(mColOrder);
			if ( mImageFileListTrain != "" )        
			{
				mTrainImageFiles.readFileNames(mImageFileListTrain);          
			}
			else
			{
				//TraceMessage::addError("Check the option ImageFileList, as it is not set for the fold training");
				throw KifasExpeption("[FacialAnalysisFrame::Run] : Check the option ImageFileList, as it is not set for the fold training");
				// you should add exit code
			}
		}
		else if ( mTrainMode == FacialAnalysisFrame::SEPARATE )		// SEPERATE
		{
			// set the path of image files
			mTrainImageFiles.setImageFileDir(mTrainImageDir);
			mTrainImageFiles.setColOrder(mColOrder);
			if (mImageFileListTrain != "" && mImageFileListTest != "" )
			{
				mTrainImageFiles.readFileNames(mImageFileListTrain);                        
			}
			else
			{
				//TraceMessage::addError("Check the options ImageFileListTrain and ImageFileListTest, as they are not set for separate training");
				throw KifasExpeption("[FacialAnalysisFrame::Run] : Check the options ImageFileListTrain and ImageFileListTest, as they are not set for separate training.");
				// you should add exit code
			}
		}
		else
		{
			//TraceMessage::addError("Not a valid training mode. Use 'TrainMode = fold | separate'");
			throw KifasExpeption("[FacialAnalysisFrame::Run] : Not a valid training mode. Use 'TrainMode = fold | separate'");
			// you should add exit code
		}	
		
    
		TraceMessage::addMessageLine("Creating trainning images");		
    
		// set selected code size on Feature representation manager
		int appCodeSize = mAppearanceCodeManager.getCodeSize();
        int sequenceSize = mTrainImageFiles.getSequenceSize();
		mFeatureRepresentationManager.setCodeSize(appCodeSize);
        mFeatureRepresentationManager.setSequenceSize(sequenceSize);

        // creating row, cols matrix
		//int featureType = 
		int featureType = 	mFeatureRepresentationManager.getFeatureType();
		int featureLength = mFeatureRepresentationManager.getFeatureLength();
		cv::Mat trainingData(mTrainImageFiles.getImageFileNumber(),  mFeatureRepresentationManager.getFeatureLength(), featureType );
		
        // pass the number of classes to ClassifierManager
        mClassifierManager.SetConfusionMatrix(mTrainImageFiles.getClassNumber());

		// Appearance Code
		double tic_t = clock();

		for(i=0; i < mTrainImageFiles.getImageFileNumber(); i++)
		{
			// get the feature of the image
			cv::Mat feature = getFeature( mTrainImageFiles.getNextImageFilePaths() );
			
			// Copy feature vector to traindata
			copyFeatureVectorData(feature, trainingData, i);

			// print calculation time of encoding 100 images
			if( (i % 100) == 0)
			{
				std::cout << "It took "<< (clock() - tic_t)/CLOCKS_PER_SEC << " second(s)."<< std::endl;
				tic_t = clock();
				TraceMessage::addMessage(".");
			}
		}
		TraceMessage::addMessageLine("");

		double result = 0;
		double num = 0;
		double minResult = 100;
		double maxResult = -100;
		// Now check if we would train the test images or just mutual verification
		if ( mTrainMode == FacialAnalysisFrame::SEPARATE )
		{
			TraceMessage::addMessageLine("Creating testing images");
			mTestImageFiles.setImageFileDir(mTestImageDir);
			mTestImageFiles.setColOrder(mColOrder);
			mTestImageFiles.readFileNames(mImageFileListTest); 
            int testSqeuenceSize = mTestImageFiles.getSequenceSize();


            if(sequenceSize != testSqeuenceSize)
            {
                throw KifasExpeption("[kifas::FacialAnalysisFrame::Run] :Test and Train data have different sequence size!!. Check train and test image set\n");
            }

			//ImageFileManager testFiles( mImageDir, mImageFileListTest );
			cv::Mat testingData( mTestImageFiles.getImageFileNumber(), featureLength, featureType );

			// create feature vectors for testing
			for(i=0; i < mTestImageFiles.getImageFileNumber(); i++)
			{        
				// get the feature of the image
				cv::Mat feature = getFeature( mTestImageFiles.getNextImageFilePath() );
			
				// Copy feature vector to traindata
				copyFeatureVectorData(feature, testingData, i);

				if( (i % 100) == 0)
					TraceMessage::addMessage(".");
			}  

			TraceMessage::addMessageLine("Running classifier");
            std::vector<int> testResult;
            testResult.assign(mTestImageFiles.getImageFileTruthTable().size(),-1);
            std::vector<int> tempTruthTable = mTrainImageFiles.getImageFileTruthTable();
			mClassifierManager.RunClassifier( trainingData, tempTruthTable, testingData, testResult );

            // calculate percentage
            int j;
            double CurrentResult =0;
            for(j=0; j< testResult.size(); j++)
                if( testResult[j] == mTestImageFiles.getImageFileTruthTable()[j]) CurrentResult++;

            result = CurrentResult / (double)(testResult.size());

            // calculate confusion matrix
            tempTruthTable = mTestImageFiles.getImageFileTruthTable();
            mClassifierManager.AccConfusionMatrix(tempTruthTable,testResult);

            // print detail result of each file
            mTestImageFiles.resetFileIndex();
            for(j=0; j< testResult.size(); j++)
            {
                std::stringstream ss;
                ss << mTestImageFiles.getNextImageFilePath() <<" "<<mTestImageFiles.getImageFileTruthTable()[j]<<" "<<testResult[j];
                TraceMessage::addMessageLine(ss.str());
            }
			num++;
		}
		else if ( mTrainMode == FacialAnalysisFrame::FOLD )
		{
			//int folds = mConfigFileManager.getFolds();
			if (mFolds <= 0)
			{
				TraceMessage::addWarning("The number of folds is not greater than zero. Using default 10 folds");
				mFolds = 10;
			}
			TraceMessage::addMessageLine("Running mutial verification");
			std::stringstream strNum;
			strNum << mFolds;
			TraceMessage::addMessageLine("Divide number = " + strNum.str());

			result = 0;
			minResult =100;
			maxResult = -100;
			if(mPersonIndepent)
			{
				result = mClassifierManager.runClassifierMutialVerificationPersonIndepent(trainingData,mTrainImageFiles.getImageFileTruthTable(),mTrainImageFiles.getImageFilePersonTable());
				num++;
			}
			else{
				for(i=0; i< 10; i++)
				{
					float r = mClassifierManager.runClassifierMutualVerification(trainingData,mTrainImageFiles.getImageFileTruthTable(), mFolds);
					result +=r; 
					TraceMessage::addMessage(i+1);
					TraceMessage::addMessage(" final Result = ");
					TraceMessage::addMessageLine(r);

					if( r < minResult)	minResult = r;
					if( r > maxResult)  maxResult = r;

					num++;
				}
			}
		}

		std::ostringstream avgstrs,minstrs,maxstrs;
		avgstrs << result/num;
		TraceMessage::addMessageLine("Result of average = " + avgstrs.str());
		
		minstrs << minResult;
		TraceMessage::addMessageLine("Result of minimum = " + minstrs.str());

		maxstrs << maxResult;
		TraceMessage::addMessageLine("Result of maximum = " + maxstrs.str());

        TraceMessage::addMessageLine("Confusion Matrix");
        mClassifierManager.PrintConfusionMatrix();
	}
	catch(KifasExpeption &e)
	{
		TraceMessage::addError("Error occurs!!!");
		TraceMessage::addError(e.what());
		return false;
	}
	
	
 return true;
}

/**
* This function encapsulates all the processing needed for generating the feature vector of a given image path.
* It reads the image, run all the preprocessing steps, generates the appearance code and the feature vector.
* @param path is the path of the image file to generate the feature vector from.
* @returns a matrix that represents the feature vector of the given image.
*/
cv::Mat kifas::FacialAnalysisFrame::getFeature(std::string path)
{
	// read image
	//cv::Mat img = cv::imread(	path ,CV_LOAD_IMAGE_ANYDEPTH );

	//// Preprocessing
	//mPreprocessingManager.runAllPreprocessing(img);
	//
	//// Feature extraction
	//cv::Mat appearanceCode = mAppearanceCodeManager.genAppearanceCode(img);
	//

	//// Feature representation
	//cv::Mat featureVector = mFeatureRepresentationManager.genFeatureVectors(appearanceCode);
    cv::Mat img;
    return img;
  //return featureVector;
}

cv::Mat kifas::FacialAnalysisFrame::getFeature(const std::vector<std::string> paths)
{
    // read image
    int i;
    std::vector<cv::Mat> imageList;
    for(i=0; i< paths.size(); i++)
    {
	    cv::Mat img = cv::imread(paths[i] ,CV_LOAD_IMAGE_ANYDEPTH );
        imageList.push_back(img);
    }

    // preprocessing
    mPreprocessingManager.runAllPreprocessing(imageList);

    // Feature representation
    std::vector<cv::Mat> appearanceCodeList;
    appearanceCodeList = mAppearanceCodeManager.genAppearanceCode(imageList);

    //// Feature representation
	cv::Mat featureVector = mFeatureRepresentationManager.genFeatureVectors(appearanceCodeList);

    return featureVector;
}
/*
void kifas::FacialAnalysisFrame::addMessage(std::string msg)
{
	std::cout<<msg.c_str()<<std::endl;
}*/
