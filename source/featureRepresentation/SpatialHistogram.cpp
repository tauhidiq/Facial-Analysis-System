#include "SpatialHistogram.h"
#include "Definitions.h"
#include "ConfigFileManager.h"
#include "Parser.h"
#include <opencv2/highgui/highgui.hpp>


static cv::Mat histc_(const cv::Mat& src, int minVal=0, int maxVal=255, bool normed=false);
static cv::Mat histc(cv::Mat src, int minVal, int maxVal, bool normed);


SpatialHistogram::SpatialHistogram() 
{
	mConfigFile = "SpatialHistogram.conf";
  
	std::map<std::string, kifas::NumStr> ldpconfig;
	ldpconfig["xgrid"] = 8;
	ldpconfig["ygrid"] = 9;
	
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, ldpconfig);

	// set the values
	mXgrid = ldpconfig["xgrid"];
	mYgrid= ldpconfig["ygrid"];
}

/**
* Perform Preprocessing
* @param input is the coded image to represent.
* @return image after preprocessing.	
*/
cv::Mat SpatialHistogram::runFeatureVectors(std::vector<cv::Mat> &inputVector)
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

	// Get input Type
	int inputType = input.type();

	// block sizes
	int width = input.cols / mXgrid;
	int height = input.rows /mYgrid;

	// set size and type of a feature vector 
	cv::Mat result = cv::Mat::zeros(mXgrid * mYgrid, mCodeDiversity, CV_32FC1);
	
	// initial result_row
    int resultRowIdx = 0;

	// for loop
	int i,j;
	
	// iterate through grid
	for(i = 0; i < mYgrid; i++) {
        for(j = 0; j < mXgrid; j++) {
            
			// get a sub block
			cv::Mat src_cell = cv::Mat(input, cv::Range(i*height,(i+1)*height), cv::Range(j*width,(j+1)*width));
            
			// calculate histogram of the sub block
			cv::Mat cell_hist = histc(src_cell, 0, (mCodeDiversity-1), false);
            
			// copy to the result matrix
            cv::Mat result_row = result.row(resultRowIdx);
            cell_hist.reshape(1,1).convertTo(result_row, CV_32FC1);
            // increase row count in result matrix
            resultRowIdx++;
        }
    }
    // return result as reshaped feature vector
    return result.reshape(1,1);
}

int SpatialHistogram::getFeatureLength()
{
  return mCodeDiversity * mXgrid * mYgrid;
}

static cv::Mat
histc_(const cv::Mat& src, int minVal, int maxVal, bool normed)
{
	cv::Mat result;
	// Establish the number of bins.
	int histSize = maxVal-minVal+1;
	
	// Set the ranges.
	float range[] = { static_cast<float>(minVal), static_cast<float>(maxVal+1) };
	const float* histRange = { range };
	// calc histogram
	cv::calcHist(&src, 1, 0, cv::Mat(), result, 1, &histSize, &histRange, true, false);
	
	// normalize
	if(normed) {
		result /= (int)src.total();
	}
	return result.reshape(1,1);
}

static cv::Mat histc(cv::Mat src, int minVal, int maxVal, bool normed)
{
	//Mat src = _src.getMat();
	switch (src.type()) {
		case CV_8SC1:
			return histc_(cv::Mat_<float>(src), minVal, maxVal, normed);
			break;
		case CV_8UC1:
			return histc_(src, minVal, maxVal, normed);
			break;
		case CV_16SC1:
			return histc_(cv::Mat_<float>(src), minVal, maxVal, normed);
			break;
		case CV_16UC1:
			return histc_(src, minVal, maxVal, normed);
			break;
		case CV_32SC1:
			return histc_(cv::Mat_<float>(src), minVal, maxVal, normed);
			break;
		case CV_32FC1:
			return histc_(src, minVal, maxVal, normed);
			break;
		default:
			CV_Error(CV_StsUnmatchedFormats, "This type is not implemented yet."); break;
	}
	return cv::Mat();
}