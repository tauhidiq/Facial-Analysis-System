#include "LDTPsHistogram.h"
#include "Definitions.h"
#include "ConfigFileManager.h"
#include "Parser.h"
#include <opencv2/highgui/highgui.hpp>


LDTPsHistogram::LDTPsHistogram() 
{
	mConfigFile = "LDTPsHistogram.conf";
  
	std::map<std::string, kifas::NumStr> LDTPsHistogramConfig;
	LDTPsHistogramConfig["xgrid"] = 8;          // grid size
	LDTPsHistogramConfig["ygrid"] = 10;         // grid size
    LDTPsHistogramConfig["divtype"] = "nodiv";  // sub grid size
	LDTPsHistogramConfig["kvalue"] = 4;         // active code

	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, LDTPsHistogramConfig);

	// set the values
	mXgrid = LDTPsHistogramConfig["xgrid"];
	mYgrid= LDTPsHistogramConfig["ygrid"];
    kValue = LDTPsHistogramConfig["kvalue"];
    if( kValue < 2 || kValue > 8)
    {
		TraceMessage::addMessage("In LDTPsHistogram.conf, kValue is set larger than 8 or less 2 as "+kValue);
		TraceMessage::addMessage("kValue is set 2 as default value.");
        kValue = 2;
    }

    if( (kValue % 2) != 0)
    {
        TraceMessage::addMessage("In LDTPsHistogram.conf, kValue is set as odd number,"+kValue);
		TraceMessage::addMessage("kValue is set 2 as default value.");
        kValue = 2;
    }

    std::string attributevalue = LDTPsHistogramConfig["divtype"];
    std::transform(attributevalue.begin(), attributevalue.end(), attributevalue.begin(), ::tolower);
    if(attributevalue.compare("nodiv") == 0)
        mDivType = SpatialCodeBlock::NO_DIVISION;
	else if(attributevalue.compare("ver2")==0)
        mDivType = SpatialCodeBlock::VERTICAL_2;
    else if(attributevalue.compare("ver4")==0)
        mDivType = SpatialCodeBlock::VERTICAL_4;
    else if(attributevalue.compare("hor2")==0)
        mDivType = SpatialCodeBlock::HORIZONTAL_2;
    else if(attributevalue.compare("hor4")==0)
        mDivType = SpatialCodeBlock::HORIZONTAL_4;
    else if(attributevalue.compare("verhor4")==0)
        mDivType = SpatialCodeBlock::VER_HOR_4;
	else 
	{
		mDivType = SpatialCodeBlock::NO_DIVISION;
		TraceMessage::addMessage("In LDTPsHistogram.conf, DivType is set unknown value as "+attributevalue);
		TraceMessage::addMessage("NoDiv as default DivType, will be used.");
	}


	mFeatureBlockInit = false;

	mFeatureBlocks = NULL;
	mDynamicCodes = NULL;
}

/**
* Perform Preprocessing
* @param input is the coded image to represent.
* @return image after preprocessing.	
*/
cv::Mat LDTPsHistogram::runFeatureVectors(std::vector<cv::Mat> &inputVector)
{
    // sequence support check
    if(inputVector.size() > 1)
        throw KifasExpeption("[CLBP::runCodeGenerate] :CLBP method cannot extract features from sequence images. Check input images are sequence or not \n");
	
    //convert to grayscale
    cv::Mat input = inputVector[0];


	// 1. determine vector dimensions
	// 2. calculate vectors
	if(mCodeDiversity == 0)
		throw KifasExpeption("[SpatialHistogram::runFeatureVectors] : the number of bins can not be calculated because code diversiy is zero!!");

	if(input.empty())
		throw KifasExpeption("[SpatialHistogram::runFeatureVectors] : Histogram vectors cannot be calculated since input matrix is empty!!");

	// // block class allocations
	if(mFeatureBlockInit == false)
		LDTPsHistogram::makeFeatureBlocks();

	// Get input Type
	int inputType = input.type();

	// for loop
	int i,j;

	// block sizes
	int width = input.cols / mXgrid;
	int height = input.rows /mYgrid;

	// set size and type of a feature vector 
	int featureSize = 0;

	for(i = 0; i < mYgrid; i++) {
        for(j = 0; j < mXgrid; j++){
			featureSize += mFeatureBlocks[i][j].getHistogramSize();
		}
	}

	cv::Mat result = cv::Mat::zeros(1, featureSize, CV_32FC1);
	result.setTo(0);

	// initial result_row
    int resultColIdx = 0;

	float *histo = result.ptr<float>(0);
	
	int x_start_pos = ((input.cols) - (width * mXgrid) ) /2;
	int y_start_pos = ((input.rows) - (height * mYgrid) ) /2;
	// iterate through grid
	for(i = 0; i < mYgrid; i++) {
        for(j = 0; j < mXgrid; j++) {
            
			// get a sub block
			cv::Mat src_cell = cv::Mat(input, cv::Range(y_start_pos+i*height,y_start_pos+(i+1)*height), 
											  cv::Range(x_start_pos+j*width,x_start_pos+(j+1)*width));
            
			// calculate LDTPs histogram
			mFeatureBlocks[i][j].getHistogram(src_cell,(histo+resultColIdx));
            
			// increase row count in result matrix
			resultColIdx += mFeatureBlocks[i][j].getHistogramSize();
        }
    }

    // return result as reshaped feature vector
    return result;
}

int LDTPsHistogram::getFeatureLength()
{
	// block class allocations
	if(mFeatureBlockInit == false)
		LDTPsHistogram::makeFeatureBlocks();

	// set size and type of a feature vector 
	int featureSize = 0;

	int i,j;
	for(i = 0; i < mYgrid; i++) {
        for(j = 0; j < mXgrid; j++){
			featureSize += mFeatureBlocks[i][j].getHistogramSize();
		}
	}

	return featureSize;
}

void LDTPsHistogram::makeFeatureBlocks()
{
	// 1, dynamic allocation blocks
	// 2. dynamic allocation dynamic codes

	deleteFeatureBlocks();

	int i,j;
	mFeatureBlocks = new SpatialCodeBlock*[mYgrid];
	mDynamicCodes = new std::vector<int> *[mYgrid];

	for(i=0; i<mYgrid; i++)
	{
		mFeatureBlocks[i] = new SpatialCodeBlock[mXgrid];
		mDynamicCodes[i] = new std::vector<int> [mXgrid];
	}

	for(i=0; i< mYgrid; i++)
	{
		for(j=0; j<mXgrid; j++)
		{
            // k = 4
			 /* mDynamicCodes[i][j].push_back(2); 			mDynamicCodes[i][j].push_back(7);
			mDynamicCodes[i][j].push_back(10);			mDynamicCodes[i][j].push_back(13);

			mDynamicCodes[i][j].push_back(16);			mDynamicCodes[i][j].push_back(18);
			mDynamicCodes[i][j].push_back(24);			mDynamicCodes[i][j].push_back(26);

			mDynamicCodes[i][j].push_back(32);			mDynamicCodes[i][j].push_back(34);
			mDynamicCodes[i][j].push_back(40);			mDynamicCodes[i][j].push_back(42);

			mDynamicCodes[i][j].push_back(45);			mDynamicCodes[i][j].push_back(48);
			mDynamicCodes[i][j].push_back(51);			mDynamicCodes[i][j].push_back(56);*/

            // k = 2
            if(kValue == 2)
            {
                mDynamicCodes[i][j].push_back(7);   mDynamicCodes[i][j].push_back(18);
                mDynamicCodes[i][j].push_back(32);   mDynamicCodes[i][j].push_back(34);
                mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);
                mDynamicCodes[i][j].push_back(48);   mDynamicCodes[i][j].push_back(51);
            }
            else if(kValue == 4)
            {
                mDynamicCodes[i][j].push_back(2); 			mDynamicCodes[i][j].push_back(7);
			    mDynamicCodes[i][j].push_back(10);			mDynamicCodes[i][j].push_back(13);

			    mDynamicCodes[i][j].push_back(16);			mDynamicCodes[i][j].push_back(18);
			    mDynamicCodes[i][j].push_back(24);			mDynamicCodes[i][j].push_back(26);

			    mDynamicCodes[i][j].push_back(32);			mDynamicCodes[i][j].push_back(34);
			    mDynamicCodes[i][j].push_back(40);			mDynamicCodes[i][j].push_back(42);

			    mDynamicCodes[i][j].push_back(45);			mDynamicCodes[i][j].push_back(48);
			    mDynamicCodes[i][j].push_back(51);			mDynamicCodes[i][j].push_back(56);
            }
            else if(kValue == 6)
            {
                mDynamicCodes[i][j].push_back(1);   mDynamicCodes[i][j].push_back(2);
                mDynamicCodes[i][j].push_back(7);   mDynamicCodes[i][j].push_back(8);
                mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(13);
                mDynamicCodes[i][j].push_back(15);   mDynamicCodes[i][j].push_back(16);
                mDynamicCodes[i][j].push_back(18);   mDynamicCodes[i][j].push_back(22);
                mDynamicCodes[i][j].push_back(24);   mDynamicCodes[i][j].push_back(26);
                mDynamicCodes[i][j].push_back(29);   mDynamicCodes[i][j].push_back(32);
                mDynamicCodes[i][j].push_back(34);   mDynamicCodes[i][j].push_back(36);
                mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);
                mDynamicCodes[i][j].push_back(43);   mDynamicCodes[i][j].push_back(45);
                mDynamicCodes[i][j].push_back(48);   mDynamicCodes[i][j].push_back(50);
                mDynamicCodes[i][j].push_back(51);   mDynamicCodes[i][j].push_back(56);

            }
            else if(kValue == 8)
            {
                mDynamicCodes[i][j].push_back(1);   mDynamicCodes[i][j].push_back(2);
                mDynamicCodes[i][j].push_back(4);   mDynamicCodes[i][j].push_back(5);
                mDynamicCodes[i][j].push_back(7);   mDynamicCodes[i][j].push_back(8);
                mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(11);
                mDynamicCodes[i][j].push_back(13);   mDynamicCodes[i][j].push_back(15);   
                mDynamicCodes[i][j].push_back(16);   mDynamicCodes[i][j].push_back(18);   
                mDynamicCodes[i][j].push_back(21);   mDynamicCodes[i][j].push_back(22);
                mDynamicCodes[i][j].push_back(24);   mDynamicCodes[i][j].push_back(26);
                mDynamicCodes[i][j].push_back(27);   mDynamicCodes[i][j].push_back(28);
                mDynamicCodes[i][j].push_back(29);   mDynamicCodes[i][j].push_back(32);
                mDynamicCodes[i][j].push_back(34);   mDynamicCodes[i][j].push_back(36);
                mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);
                mDynamicCodes[i][j].push_back(43);   mDynamicCodes[i][j].push_back(45);
                mDynamicCodes[i][j].push_back(47);   mDynamicCodes[i][j].push_back(48);   
                mDynamicCodes[i][j].push_back(50);   mDynamicCodes[i][j].push_back(51);   
                mDynamicCodes[i][j].push_back(53);   mDynamicCodes[i][j].push_back(56);
            }
            // k = 6
            /*mDynamicCodes[i][j].push_back(1);   mDynamicCodes[i][j].push_back(2);
            mDynamicCodes[i][j].push_back(7);   mDynamicCodes[i][j].push_back(8);
            mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(13);
            mDynamicCodes[i][j].push_back(15);   mDynamicCodes[i][j].push_back(16);
            mDynamicCodes[i][j].push_back(18);   mDynamicCodes[i][j].push_back(22);
            mDynamicCodes[i][j].push_back(24);   mDynamicCodes[i][j].push_back(26);
            mDynamicCodes[i][j].push_back(29);   mDynamicCodes[i][j].push_back(32);
            mDynamicCodes[i][j].push_back(34);   mDynamicCodes[i][j].push_back(36);
            mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);
            mDynamicCodes[i][j].push_back(43);   mDynamicCodes[i][j].push_back(45);
            mDynamicCodes[i][j].push_back(48);   mDynamicCodes[i][j].push_back(50);
            mDynamicCodes[i][j].push_back(51);   mDynamicCodes[i][j].push_back(56);*/
           
            // k = 8
            /*mDynamicCodes[i][j].push_back(1);   mDynamicCodes[i][j].push_back(2);
            mDynamicCodes[i][j].push_back(4);   mDynamicCodes[i][j].push_back(5);
            mDynamicCodes[i][j].push_back(7);   mDynamicCodes[i][j].push_back(8);
            mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(11);
            mDynamicCodes[i][j].push_back(13);   mDynamicCodes[i][j].push_back(15);   
            mDynamicCodes[i][j].push_back(16);   mDynamicCodes[i][j].push_back(18);   
            mDynamicCodes[i][j].push_back(21);   mDynamicCodes[i][j].push_back(22);
            mDynamicCodes[i][j].push_back(24);   mDynamicCodes[i][j].push_back(26);
            mDynamicCodes[i][j].push_back(27);   mDynamicCodes[i][j].push_back(28);
            mDynamicCodes[i][j].push_back(29);   mDynamicCodes[i][j].push_back(32);
            mDynamicCodes[i][j].push_back(34);   mDynamicCodes[i][j].push_back(36);
            mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);
            mDynamicCodes[i][j].push_back(43);   mDynamicCodes[i][j].push_back(45);
            mDynamicCodes[i][j].push_back(47);   mDynamicCodes[i][j].push_back(48);   
            mDynamicCodes[i][j].push_back(50);   mDynamicCodes[i][j].push_back(51);   
            mDynamicCodes[i][j].push_back(53);   mDynamicCodes[i][j].push_back(56);*/


            // Adin's suggestion

            //if(kValue == 2)
            //{
            //    mDynamicCodes[i][j].push_back(7);    mDynamicCodes[i][j].push_back(32);   
            //    mDynamicCodes[i][j].push_back(34);   mDynamicCodes[i][j].push_back(40);   
            //    mDynamicCodes[i][j].push_back(42);   mDynamicCodes[i][j].push_back(51);
            //}
            //else if(kValue == 4)
            //{
            //
            //}
            // k = 2
            /**/

            // k = 4
           /* mDynamicCodes[i][j].push_back(2);    mDynamicCodes[i][j].push_back(7);  
            mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(18);   
            mDynamicCodes[i][j].push_back(32);   mDynamicCodes[i][j].push_back(34);  
            mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);
            mDynamicCodes[i][j].push_back(48);   mDynamicCodes[i][j].push_back(51);*/
           
            // k = 6
            //mDynamicCodes[i][j].push_back(2);    mDynamicCodes[i][j].push_back(7);
            //mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(16);
            //mDynamicCodes[i][j].push_back(18);   mDynamicCodes[i][j].push_back(24);   
            //mDynamicCodes[i][j].push_back(26);   mDynamicCodes[i][j].push_back(32);
            //mDynamicCodes[i][j].push_back(34);   mDynamicCodes[i][j].push_back(40);   
            //mDynamicCodes[i][j].push_back(42);   mDynamicCodes[i][j].push_back(45);
            //mDynamicCodes[i][j].push_back(48);   mDynamicCodes[i][j].push_back(51);   
            //mDynamicCodes[i][j].push_back(56);
           
            // k = 8
            /*mDynamicCodes[i][j].push_back(2);    mDynamicCodes[i][j].push_back(7);
            mDynamicCodes[i][j].push_back(10);   mDynamicCodes[i][j].push_back(13);   
            mDynamicCodes[i][j].push_back(16);   mDynamicCodes[i][j].push_back(18);   
            mDynamicCodes[i][j].push_back(24);   mDynamicCodes[i][j].push_back(26);   
            mDynamicCodes[i][j].push_back(32);   mDynamicCodes[i][j].push_back(34);   
            mDynamicCodes[i][j].push_back(40);   mDynamicCodes[i][j].push_back(42);   
            mDynamicCodes[i][j].push_back(45);   mDynamicCodes[i][j].push_back(48);   
            mDynamicCodes[i][j].push_back(51);   mDynamicCodes[i][j].push_back(56);*/

           

		}
	}
	

	for(i=0; i< mYgrid; i++)
	{
		for(j=0; j<mXgrid; j++)
		{
			//VER_HOR_4, VERTICAL_2, HORIZONTAL_2
			mFeatureBlocks[i][j].setBlockDivision(mDivType);
			mFeatureBlocks[i][j].setMainCodes(mDynamicCodes[i][j]);
			mFeatureBlocks[i][j].setOriginalCodeDimensions(mCodeDiversity);
			mFeatureBlocks[i][j].setNormalization(false);
		}
	}
	//for(i=0; i< 3; i++)
	//{
	//	for(j=0; j<mXgrid; j++)
	//	{
	//		//VER_HOR_4, VERTICAL_2, HORIZONTAL_2
	//		mFeatureBlocks[i][j].setBlockDivision(SpatialCodeBlock::VERTICAL_4);
	//		mFeatureBlocks[i][j].setMainCodes(mDynamicCodes[i][j]);
	//		mFeatureBlocks[i][j].setOriginalCodeDimensions(mCodeDiversity);
	//		mFeatureBlocks[i][j].setNormalization(false);
	//	}
	//}

	//for(i=3; i< mYgrid; i++)
	//{
	//	for(j=0; j<mXgrid; j++)
	//	{
	//		//VER_HOR_4, VERTICAL_2, HORIZONTAL_2
	//		mFeatureBlocks[i][j].setBlockDivision(SpatialCodeBlock::VER_HOR_4);
	//		mFeatureBlocks[i][j].setMainCodes(mDynamicCodes[i][j]);
	//		mFeatureBlocks[i][j].setOriginalCodeDimensions(mCodeDiversity);
	//		mFeatureBlocks[i][j].setNormalization(false);
	//	}
	//}

	mFeatureBlockInit = true;
}

void LDTPsHistogram::deleteFeatureBlocks()
{
	int i;
	if(mFeatureBlocks != NULL)
	{
		for(i=0; i< mYgrid; i++)
			delete mFeatureBlocks[i];

		delete mFeatureBlocks;
		mFeatureBlocks = NULL;
	}

	if(mDynamicCodes != NULL)
	{
		for(i=0; i< mYgrid; i++)
			delete mDynamicCodes[i];

		delete mDynamicCodes;
		mDynamicCodes= NULL;
	}

	mFeatureBlockInit = false;
}




SpatialCodeBlock::SpatialCodeBlock(){
    mBlockDivType = NO_DIVISION;
    mCodeDimensions = 0;
    mNormalization = false;
}
SpatialCodeBlock::~SpatialCodeBlock(){


}

int SpatialCodeBlock::getHistogramSize(){
    int subBlocks = getDivisionNumber();
    int histSize = mCodeDimensions + ((subBlocks -1) * mMainCodes.size());

    return histSize;
}

int SpatialCodeBlock::getDivisionNumber()
{
    if(mBlockDivType == NO_DIVISION)
        return 1;
    else if((mBlockDivType == VERTICAL_4) || (mBlockDivType == HORIZONTAL_4) || (mBlockDivType == VER_HOR_4))
        return 4;
    else if((mBlockDivType == VERTICAL_2) || (mBlockDivType == HORIZONTAL_2))
        return 2;

    return 0;
}


void SpatialCodeBlock::setMainCodes(std::vector<int> mainCodes)
{
    mMainCodes = mainCodes;

    int i; 
    for(i=0; i< mMainCodes.size(); i++)
    {
        mMainCodeIndex[mMainCodes[i]] = i;
    }
}

void SpatialCodeBlock::getHistogram(cv::Mat blockCodeImg,float* hist){
    
    if(mBlockDivType == NO_DIVISION)
        _getHistogramNoDiv(blockCodeImg, hist);
    else if(mBlockDivType == VERTICAL_4)
        _getHistogramVer4(blockCodeImg, hist);
    else if(mBlockDivType == HORIZONTAL_4)
        _getHistogramHor4(blockCodeImg, hist);
    else if(mBlockDivType == VERTICAL_2)
        _getHistogramVer2(blockCodeImg, hist);
    else if(mBlockDivType == HORIZONTAL_2)
        _getHistogramHor2(blockCodeImg, hist);
    else if(mBlockDivType == VER_HOR_4)
        _getHistogramVerHor4(blockCodeImg, hist);
}



void SpatialCodeBlock::_getHistogramNoDiv(cv::Mat blockCodeImg,float* hist){
    int i,j;

    int size_x, size_y;

    size_x = blockCodeImg.cols; // get x size
    size_y = blockCodeImg.rows; // get y size
    int totalAcc = 0;               // The total number of accumulation to histogram
    
    for(i=0; i< size_y; i++)
    {
        uchar* lineBuf = blockCodeImg.ptr<uchar>(i); // get line buffer

        for(j=0; j< size_x; j++)
        {
            // check validation of feature codes
            if(  ( lineBuf[j] >= 0) && (lineBuf[j] < mCodeDimensions))
            {
                hist[lineBuf[j]]++;
                totalAcc++;
            }
        }
    }

    if(mNormalization == true)
    {
        int HistSize = getHistogramSize();

        for(i=0; i< HistSize; i++) 
            hist[i] = hist[i] / (float)totalAcc;

    }
}


void SpatialCodeBlock::_getHistogramVer4(cv::Mat blockCodeImg,float* hist){
    int ypos,xpos;

    int size_x, size_y;

    size_x = blockCodeImg.cols; // get x size
    size_y = blockCodeImg.rows; // get y size

    int quarter_hegiht =size_y /4;
    int totalAcc = 0;               // The total number of accumulation to histogram

    for(ypos=0; ypos< size_y; ypos++)
    {
        uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

        for(xpos=0; xpos< size_x; xpos++)
        {
            uchar valuePix = lineBuf[xpos];
            uchar newCode = valuePix;

            if((std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end()) || (( ypos >=0 ) && (ypos < quarter_hegiht)))
                newCode = valuePix;
            else if(( ypos >= quarter_hegiht) && ( ypos< (quarter_hegiht *2))) // 주요 코드 이고 두번째 division에 있는 경우 
                newCode = mCodeDimensions + mMainCodeIndex.at(valuePix);
            else if( ypos >= (quarter_hegiht *2) && (ypos < (quarter_hegiht *3)))
                newCode = mCodeDimensions + mMainCodes.size() + mMainCodeIndex.at(valuePix);
            else if(ypos >= (quarter_hegiht *3) ) 
                newCode = mCodeDimensions + mMainCodes.size() + mMainCodes.size() + mMainCodeIndex.at(valuePix);

            // check validation of feature codes
            if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
        }
    }

    int i;
    if(mNormalization == true)
    {
        int HistSize = getHistogramSize();

        for(i=0; i< HistSize; i++) 
            hist[i] = hist[i] / (float)totalAcc;

    }
}


void SpatialCodeBlock::_getHistogramHor4(cv::Mat blockCodeImg,float* hist){
    int ypos,xpos;

    int size_x, size_y;

    size_x = blockCodeImg.cols; // get x size
    size_y = blockCodeImg.rows; // get y size

    int quarter_width =size_x /4;
    int totalAcc = 0;               // The total number of accumulation to histogram

    for(ypos=0; ypos< size_y; ypos++)
    {
        uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

        for(xpos=0; xpos< size_x; xpos++)
        {
            uchar valuePix = lineBuf[xpos];
            uchar newCode = valuePix;

            if((std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end()) || (( xpos >=0 ) && (xpos < quarter_width)))
                newCode = valuePix;
            else if(( xpos >= quarter_width) && ( xpos< (quarter_width *2))) // 주요 코드 이고 두번째 division에 있는 경우 
                newCode = mCodeDimensions + mMainCodeIndex.at(valuePix);
            else if( xpos >= (quarter_width *2) && (xpos < (quarter_width *3)))
                newCode = mCodeDimensions + mMainCodes.size() + mMainCodeIndex.at(valuePix);
            else if(xpos >= (quarter_width *3) ) 
                newCode = mCodeDimensions + mMainCodes.size() + mMainCodes.size() + mMainCodeIndex.at(valuePix);

            // check validation of feature codes
            if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
        }
    }

    int i;
    if(mNormalization == true)
    {
        int HistSize = getHistogramSize();

        for(i=0; i< HistSize; i++) 
            hist[i] = hist[i] / (float)totalAcc;

    }
}

void SpatialCodeBlock::_getHistogramVerHor4(cv::Mat blockCodeImg,float* hist){
    int ypos,xpos;
    int size_x, size_y;

    size_x = blockCodeImg.cols; // get x size
    size_y = blockCodeImg.rows; // get y size

    int half_height =size_y /2;
    int half_width = size_x / 2;
    int totalAcc = 0;               // The total number of accumulation to histogram

    //for(ypos=0; ypos< size_y; ypos++)
    //{
    //    uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

    //    for(xpos=0; xpos< size_x; xpos++)
    //    {
    //        uchar valuePix = lineBuf[xpos];
    //        uchar newCode = valuePix;

    //        // 추가 위치 정보 코드가 아니거나, @@@위치에 있는 경우 원본 코드
    //        //  ---------------------
    //        //  |  @@@    |         |
    //        //  ---------------------
    //        //  |         |         |
    //        //  ---------------------
    //        if((std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end()) || (( ypos >=0 ) && (ypos < half_height) && ( xpos >=0) && (xpos < half_width)))
    //            newCode = valuePix;

    //        // 추가 위치 정보 코드가 , $$$위치에 있는 경우 원본 코드
    //        //  ---------------------
    //        //  |         |  $$$    |
    //        //  ---------------------
    //        //  |         |         |
    //        //  ---------------------
    //        else if(( ypos >=0 ) && (ypos < half_height) && ( xpos >=  half_width))
    //            newCode = mCodeDimensions + mMainCodeIndex.at(valuePix);


    //        // 추가 위치 정보 코드가 , ###위치에 있는 경우 원본 코드
    //        //  ---------------------
    //        //  |         |         |
    //        //  ---------------------
    //        //  |   ###   |         |
    //        //  ---------------------
    //        else if(ypos >= (half_height) && ( xpos >=0) && (xpos < half_width)) 
    //            newCode = mCodeDimensions + mMainCodes.size() + mMainCodeIndex.at(valuePix);
    //        
    //        // 추가 위치 정보 코드가 , ***위치에 있는 경우 원본 코드
    //        //  ---------------------
    //        //  |         |         |
    //        //  ---------------------
    //        //  |         |   ***   |
    //        //  ---------------------
    //        else if(ypos >= (half_height) && ( xpos >=  half_width)) 
    //            newCode = mCodeDimensions + mMainCodes.size() + mMainCodes.size() + mMainCodeIndex.at(valuePix);

    //        // check validation of feature codes
    //        if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
    //        {
    //            hist[newCode]++;
    //            totalAcc++;
    //        }
    //    }
    //}

	// 추가 위치 정보 코드가 아니거나, @@@위치에 있는 경우 원본 코드
    //  ---------------------
    //  |  @@@    |         |
    //  ---------------------
    //  |         |         |
    //  ---------------------
	for(ypos=0; ypos < half_height; ypos++)
	{
		uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

		for(xpos=0; xpos < half_width; xpos++)
		{
			uchar valuePix = lineBuf[xpos];
			uchar newCode = valuePix;

			if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
		}
	}

	// 추가 위치 정보 코드가 아니거나, @@@위치에 있는 경우 원본 코드
    //  ---------------------
    //  |         |   $$$   |
    //  ---------------------
    //  |         |         |
    //  ---------------------
	for(ypos=0; ypos < half_height; ypos++)
	{
		uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

		for(xpos=half_width; xpos < size_x; xpos++)
		{
			uchar valuePix = lineBuf[xpos];
			uchar newCode = valuePix;

			if(std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end())
				newCode = valuePix;	// normal codes
			else
				newCode = mCodeDimensions + mMainCodeIndex.at(valuePix);	// dynamic codes

			if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
		}
	}

	// 추가 위치 정보 코드가 아니거나, @@@위치에 있는 경우 원본 코드
    //  ---------------------
    //  |         |         |
    //  ---------------------
    //  |   %%%   |         |
    //  ---------------------
	for(ypos=half_height; ypos < size_y; ypos++)
	{
		uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

		for(xpos=0; xpos < half_width; xpos++)
		{
			uchar valuePix = lineBuf[xpos];
			uchar newCode = valuePix;

			if(std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end())
				newCode = valuePix;	// normal codes
			else
				newCode = mCodeDimensions +  mMainCodes.size()+ mMainCodeIndex.at(valuePix);	// dynamic codes

			if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
		}
	}

	// 추가 위치 정보 코드가 아니거나, @@@위치에 있는 경우 원본 코드
    //  ---------------------
    //  |         |         |
    //  ---------------------
    //  |         |   ###   |
    //  ---------------------
	for(ypos=half_height; ypos < size_y; ypos++)
	{
		uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

		for(xpos=half_width; xpos < size_x; xpos++)
		{
			uchar valuePix = lineBuf[xpos];
			uchar newCode = valuePix;

			if(std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end())
				newCode = valuePix;	// normal codes
			else
				newCode = mCodeDimensions +  mMainCodes.size() +  mMainCodes.size() + mMainCodeIndex.at(valuePix);	// dynamic codes

			if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
		}
	}

    int i;
    if(mNormalization == true)
    {
        int HistSize = getHistogramSize();

        for(i=0; i< HistSize; i++) 
            hist[i] = hist[i] / (float)totalAcc;

    }   
    
}


 void SpatialCodeBlock::_getHistogramVer2(cv::Mat blockCodeImg,float* hist){
    int ypos,xpos;
    int size_x, size_y;

    size_x = blockCodeImg.cols; // get x size
    size_y = blockCodeImg.rows; // get y size

    int half_height =size_y /2;
    int totalAcc = 0;               // The total number of accumulation to histogram

    for(ypos=0; ypos< size_y; ypos++)
    {
        uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

        for(xpos=0; xpos< size_x; xpos++)
        {
            uchar valuePix = lineBuf[xpos];
            uchar newCode = valuePix;

            if((std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end()) || (( ypos >=0 ) && (ypos < half_height)))
                newCode = valuePix;
            else if(ypos >= (half_height) ) 
                newCode = mCodeDimensions + mMainCodeIndex.at(valuePix);

            // check validation of feature codes
            if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
        }
    }

    int i;
    if(mNormalization == true)
    {
        int HistSize = getHistogramSize();

        for(i=0; i< HistSize; i++) 
            hist[i] = hist[i] / (float)totalAcc;

    }

 }

void SpatialCodeBlock::_getHistogramHor2(cv::Mat blockCodeImg,float* hist){
    int ypos,xpos;
    int size_x, size_y;

    size_x = blockCodeImg.cols; // get x size
    size_y = blockCodeImg.rows; // get y size

    int half_width =size_x /2;
    int totalAcc = 0;               // The total number of accumulation to histogram

    for(ypos=0; ypos< size_y; ypos++)
    {
        uchar* lineBuf = blockCodeImg.ptr<uchar>(ypos); // get line buffer

        for(xpos=0; xpos< size_x; xpos++)
        {
            uchar valuePix = lineBuf[xpos];
            uchar newCode = valuePix;

            if((std::find(mMainCodes.begin(),mMainCodes.end(),valuePix) == mMainCodes.end()) || (( xpos >=0 ) && (xpos < half_width)))
                newCode = valuePix;
            else if(xpos >= (half_width) ) 
                newCode = mCodeDimensions + mMainCodeIndex.at(valuePix);

            // check validation of feature codes
            if(  ( valuePix >= 0) && (valuePix < mCodeDimensions))
            {
                hist[newCode]++;
                totalAcc++;
            }
        }
    }

    int i;
    if(mNormalization == true)
    {
        int HistSize = getHistogramSize();

        for(i=0; i< HistSize; i++) 
            hist[i] = hist[i] / (float)totalAcc;

    }

}