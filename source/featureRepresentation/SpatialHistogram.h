#ifndef SPATIAL_HISTOGRAM_FEATURE_REPRESENTATION_H__
#define SPATIAL_HISTOGRAM_FEATURE_REPRESENTATION_H__

#include "BaseFeatureRepresentation.h"


/**
* This feature representation extracts a uniform grid from the image, and concatenates the histogram of each cell as feature vector.
* This representation needs a grid size to extract the feature representation from the image.
* @ingroup featureRepresentation
*/
class SpatialHistogram : public kifas::BaseFeatureRepresentation {
	public:

		SpatialHistogram();
		~SpatialHistogram() 
		{
		//if(mDataMappingTable != NULL)
		//	delete mDataMappingTable;
		}

		cv::Mat runFeatureVectors(std::vector<cv::Mat> &inputVector);
		int getFeatureLength();
		//void makeDataMappingTable();
		//int bin(int n, int r)
		//{
		//	int iteration = r;

		//	int i;
		//	int denominator=1;
		//	int nominator=1;
		//	for(i=0; i< iteration; i++)
		//	{
		//		nominator *= n;
		//		denominator *= r;
		//		n= n-1;
		//		r= r-1;
		//	}

		//	return nominator/ denominator;
		//}
		int getFeatureType() {return CV_32F;}
	private:
		int mXgrid;
		int mYgrid;
		//int mActiveBitnumber;
		//int mCodeLength;
		
		//int *mDataMappingTable;
		std::string mConfigFile;// = "UniformHistogram.conf";
};

#endif /* UNIFORM_HISTOGRAM_FEATURE_REPRESENTATION_H__ */