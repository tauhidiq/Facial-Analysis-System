#ifndef LDTPS_HISTOGRAM_FEATURE_REPRESENTATION_H__
#define LDTPS_HISTOGRAM_FEATURE_REPRESENTATION_H__

#include "BaseFeatureRepresentation.h"

class SpatialCodeBlock {
public:
    enum div_type{NO_DIVISION, VERTICAL_4, HORIZONTAL_4, VER_HOR_4, VERTICAL_2, HORIZONTAL_2};

    /**
      Constructor
    */
    SpatialCodeBlock();

    /**
      Destructor
    */
    ~SpatialCodeBlock();


    /**
      set the main codes for adding spatial info
      @param mainCodes the main codes contained in the vector container
    */
    void setMainCodes(std::vector<int> mainCodes);

    /**
      set the type of block division
      @param type the type of block division 
    */
    void setBlockDivision(div_type type){
        mBlockDivType = type;
    }

    /**
      set the dimensions of the original code
      @param dimensions the dimension of the original code
    */
    void setOriginalCodeDimensions(int dimensions){
        mCodeDimensions = dimensions;
    }

    /**
      get the size of histogram feature vector from this block
      @return the size of histogram feature vector
    */
    int getHistogramSize();

    /**
      get the size of histogram feature vector from this block
      @return the size of histogram feature vector
    */
    void getHistogram(cv::Mat blockCodeImg,float* hist);

    void setNormalization(bool normal){
        mNormalization = normal;
    }
private:

    /**
      get the number of sub block division
      @return the number of sub block division
    */
    int getDivisionNumber();

    void _getHistogramNoDiv(cv::Mat blockCodeImg,float* hist);

    void _getHistogramVer4(cv::Mat blockCodeImg,float* hist);

    void _getHistogramHor4(cv::Mat blockCodeImg,float* hist);

    void _getHistogramVerHor4(cv::Mat blockCodeImg,float* hist);

    void _getHistogramVer2(cv::Mat blockCodeImg,float* hist);

    void _getHistogramHor2(cv::Mat blockCodeImg,float* hist);

    std::vector<int>    mMainCodes;
    div_type            mBlockDivType;
    int                 mCodeDimensions;
    std::map< int,int>  mMainCodeIndex; // Key : code value, value : mMainCodes index

    bool                mNormalization;

};

//class SpatialCodeBlock;
/**
* This feature representation extracts a uniform grid from the image, and concatenates the histogram of each cell as feature vector.
* This representation needs a grid size to extract the feature representation from the image.
* @ingroup featureRepresentation
*/
class LDTPsHistogram : public kifas::BaseFeatureRepresentation {
	public:

		LDTPsHistogram();
		~LDTPsHistogram(){
			deleteFeatureBlocks();
		}

		cv::Mat runFeatureVectors(std::vector<cv::Mat> &inputVector);
		int getFeatureLength();
		int getFeatureType() {return CV_32F;}

		void makeFeatureBlocks();
		void deleteFeatureBlocks();
	private:
		int mXgrid;
		int mYgrid;
		
        SpatialCodeBlock::div_type mDivType;
        //int mXSubGrid;
        //int mYSubGrid;

        int kValue;
		std::string mConfigFile;// = "UniformHistogram.conf";

		bool mFeatureBlockInit;
		SpatialCodeBlock **mFeatureBlocks;			// block object
		std::vector<int> **mDynamicCodes;			// main codes
};



#endif /* UNIFORM_HISTOGRAM_FEATURE_REPRESENTATION_H__ */