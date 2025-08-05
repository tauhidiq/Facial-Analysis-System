#ifndef UNIFORM_HISTOGRAM_FEATURE_REPRESENTATION_H__
#define UNIFORM_HISTOGRAM_FEATURE_REPRESENTATION_H__

#include "BaseFeatureRepresentation.h"


/**
* This feature representation extracts a uniform grid from the image, and concatenates the histogram of each cell as feature vector.
* This representation needs a grid size to extract the feature representation from the image.
* @ingroup featureRepresentation
*/
class UniformHistogram : public kifas::BaseFeatureRepresentation {
	public:
		/// Constructor
		UniformHistogram();

		/// Destructor
		~UniformHistogram() {}
		
		/// calcaulates feature vectors
		cv::Mat runFeatureVectors(std::vector<cv::Mat> &inputVector);
		void runFeatureVectors_(cv::Mat input,int *startPointer);

		/// Gets the length of feature vectors
		int getFeatureLength();
		
		/// Gets the type of feature vectors 
		/// @return return the data type which 
		int getFeatureType() {return CV_32S;}

	private:
		int mXgrid;
		int mYgrid;
		//int mActiveBitnumber;
		//int mCodeLength;
		
		bool mIncludeRemains; // for containing remain feature values that are outside block area.
		std::string mConfigFile;// = "UniformHistogram.conf";
};

#endif /* UNIFORM_HISTOGRAM_FEATURE_REPRESENTATION_H__ */