/**
*  @file FacialAnalysisFrame.h
*  Defines the frame class (FacialAnalysisFrame) of the facial analysis system.
*/

#ifndef FacialAnalysisFrame_H_INCLUDED
#define FacialAnalysisFrame_H_INCLUDED

#include <cv.h>
#include <highgui.h>

#include "ImageFileManager.h"
#include "PreprocessingManager.h"
#include "AppearanceCodeManager.h"
#include "FeatureRepresentationManager.h"
#include "ConfigFileManager.h"
#include "ClassifierManager.h"
#include "TraceMessage.h"

namespace kifas
{
	
/**
* The principal frame of the system. It executes the main rutine of the program FacialAnalysisFrame::Run.
* @ingroup core
*/
class FacialAnalysisFrame
{

public:
	FacialAnalysisFrame();
	~FacialAnalysisFrame();

	bool Run();

	void copyFeatureVectorData(const cv::Mat inputVector, cv::Mat cache, const int row);
	cv::Mat getFeature(const std::string path);
    cv::Mat getFeature(const std::vector<std::string> paths);

	/** Different types of the training available. */
	enum TrainType { FOLD, SEPARATE };

	/** Different modes of running the application available. */
	enum RunType { TRAIN, TEST, BOTH };

	enum ColOrderType { NAME_PERID_ANYID, NAME_ANYID, NAME, NAME_SEQID_PERID_ANYID, NAME_SEQID_ANYID };

//	std::string getImageFileList()				{ return mImageFileList; };
	std::string getImageFileListTrain() 		{ return mImageFileListTrain; };
	std::string getImageFileListTest()			{ return mImageFileListTest; };
	TrainType getTrainMode()				    { return mTrainMode; };
	int getFolds()								{ return mFolds; };
//	std::string getImageDir()				    { return mImageDir; };
	std::string getImageDirTrain()				{ return mTrainImageDir; };
	std::string getImageDirTest()				{ return mTestImageDir; };
	RunType getRunType()				    	{ return mRunType; };
	std::list<std::string> getPreprocessing()	{ return mPreprocessing; };
	std::string getAppearanceCode()			    { return mAppearanceCode; };
	std::string getFeatureRepresentation()		{ return mFeatureRepresentation; };
	std::string getFeatureReduction()			{ return mFeatureReduction;	};
	std::string getClassifier()				    { return mClassifier;	};
	bool isVerbose()							{ return mVerbosflag;	}; 


	void readConfigFiles();
	void readClassList(std::list<std::string> &list, std::string path);
	void traceListString(std::list<std::string> &list);
	void readSystemConfigFile();
	void traceConfigResult();

protected:

	ImageFileManager				mTrainImageFiles;
	ImageFileManager				mTestImageFiles;

	PreprocessingManager			mPreprocessingManager;
	AppearanceCodeManager			mAppearanceCodeManager;
	FeatureRepresentationManager	mFeatureRepresentationManager;
	ConfigFileManager				mConfigFileManager;
	ClassifierManager				mClassifierManager;

	cv::Mat mTrainingData;


	/** @name System Configuration variables 
			This variables store systmeconfigration information
	@{ */
	RunType					mRunType;
	bool					mCache;
	std::string				mTrainCacheFile;
	std::string				mTestCacheFile;
	bool					mAppend;

	TrainType				mTrainMode;
	std::string				mImageFileListTrain;
	std::string				mImageFileListTest;
	std::string				mTrainImageDir;
	std::string				mTestImageDir;

	bool					mPersonIndepent;    // PersonIndepent
	//std::string				mImageFileList;
	int						mFolds;
	int						mColOrder;
	
	//std::string				mImageDir;
	

	std::list<std::string>	mPreprocessing;
	std::string				mAppearanceCode;
	std::string				mFeatureRepresentation;
	std::string				mFeatureReduction;
	std::string				mClassifier;
	/** @} */

	std::list<std::string> mPreprocessingList;
	std::list<std::string> mAppearanceCodeList;
	std::list<std::string> mFeatureRepresentationList;
	std::list<std::string> mFeatureReductionList;
	std::list<std::string> mClassifierList;

	bool mVerbosflag;


};

} // end of namespace
#endif
