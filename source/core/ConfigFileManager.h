/**
  @file ConfigFileManager.h
  Defines the configure class of the facial analysis system.
*/

#ifndef ConfigFileManager_H_INCLUDED
#define ConfigFileManager_H_INCLUDED

// OpenCV
#include <cv.h>

#include <string>
#include <list>

#include "Exceptions.h"
#include "TraceMessage.h"
#include "Definitions.h"
#include "NumStr.h"

namespace kifas
{

/**
* This class parses the configuration files, and loads that information into the system.
* @ingroup core
*/
class ConfigFileManager
{

public:
	
	//ConfigFileManager(const std::string file);
  //ConfigFileManager(const std::string file, std::map<std::string, NumStr> defaults);

	ConfigFileManager();
	

	/** Destructor*/
	~ConfigFileManager();

  /** Different types of the training available. */
  //enum TrainType { FOLD, SEPARATE };
  /** Different modes of running the application available. */
  //enum RunType { TRAIN, TEST, BOTH };


//	void readClassList(std::list<std::string> &list,std::string path);
//	void readSystemConfigFile();
//	void deleteSpace(char *cStr);


//	bool checkList(std::list<std::string> &list,std::string input);

	
//	void readConfigFiles();

//  NumStr const& getValue(std::string const& key) const;

	void traceListString(std::list<std::string> &list);
//	void traceConfigResult();

	static std::string getConfigFilesDir();
	
	/// read config file to config map
	static void ConfigFileRead(const std::string file, std::map<std::string, NumStr> &config);

	/// write config file to config map
	/// Config file   |   Config Map   |   Result
	///    0          |       X        |  Config File
	///    X          |       O        |  Config Map
	///    O          |       X        |  Config Map
	static void ConfigFileWrite(const std::string file, std::map<std::string, NumStr> &config);

//	std::string getImageFileList()	    			{ return mImageFileList; };
//  std::string getImageFileListTrain()  			{ return mImageFileListTrain; };
//  std::string getImageFileListTest()	 			{ return mImageFileListTest; };
//  TrainType getTrainMode()                  { return mTrainMode; };
//  int getFolds()                            { return mFolds; };
//		std::string getImageDir()				        	{ return mImageDir; };
//	RunType getRunType()				    	        { return mRunType; };
//	std::list<std::string> getPreprocessing()	{ return mPreprocessing; };
//	std::string getAppearanceCode()			    	{ return mAppearanceCode; };
//	std::string getFeatureRepresentation()	  { return mFeatureRepresentation; };
//	std::string getFeatureReduction()			    { return mFeatureReduction;	};
//	std::string getClassifier()				      	{ return mClassifier;	};
//	bool isVerbose()							            { return mVerbosflag;	};  	

  
protected:
//	enum ConfigID { RUN_TYPE, IMAGE_LIST_FILE, IMAGE_LIST_FILE_TRAIN, IMAGE_LIST_FILE_TEST, TRAIN_MODE, FOLDS,
//    IMAGE_DIR,  PREPROCESSING,  APPEARANCE_CODE,  FEATURE_REPRESENTATION, FEATURE_REDUCTION, CLASSIFIER };
  
	//
  //std::string mImageFileListTrain;
  //std::string mImageFileListTest;

	/** @name System Configuration variables 
			This variables store systmeconfigration information
	@{ */
//	RunType									mRunType;
//  bool										mCache;
//  std::string							mTrainCacheFile;

//	TrainType								mTrainMode;
//	std::string							mImageFileListTrain;
//	std::string							mImageFileListTest;
//	int											mFolds;
//	std::string							mImageFileList;
	//std::string							mImageListFileTest;
//	std::string							mColOrder;

//	std::string							mImageDir;
//	std::list<std::string>	mPreprocessing;
//	std::string							mAppearanceCode;
//  std::string							mFeatureRepresentation;
//	std::string							mFeatureReduction;
//	std::string							mClassifier;
	/** @} */

	/** @name Lists of Algorithms for the systme
			These lists store the class names of algoritms
	@{ */
//std::list<std::string> mPreprocessingList;
//	std::list<std::string> mAppearanceCodeList;
//	std::list<std::string> mFeatureRepresentationList;
//	std::list<std::string> mFeatureReductionList;
//	std::list<std::string> mClassifierList;
	/** @} */

//	bool mVerbosflag;

//  std::map<std::string, NumStr> mContent;

};


}//namespace

#endif