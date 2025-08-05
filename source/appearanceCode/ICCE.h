#ifndef ICCE_APPEARANCECODE_H__
#define ICCE_APPEARANCECODE_H__

// OpenCV
#include <cv.h>
#include "BaseAppearanceCode.h"

#include <map>


/**
  The Preprocessing Abstract class. It defines how to get frames.
  @ingroup appearanceCode
*/
class ICCE : public kifas::BaseAppearanceCode{
	public:
		enum MaskType{KIRSCH,GAUSSIAN};

		//constructor
		ICCE(); 
		~ICCE() {}
		//methods
		unsigned char calculateLDNPCode(int *kirchResult);
		//cv::Mat runCodeGenerate(const cv::Mat input);
        std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &input);

		/**
		Calculate the size of the appearance code of the class
		@return the size of the code
		*/
		int getCodeSize();



	private:

		//constant parameters
		static const int dir = 8;
//		static const int mDirections = 8;
		static const int rows = 3;
		static const int cols = 3;

		//file.conf
		std::string mConfigFile;

		int mMaskType;		// Kirsch or Sobel

		// Edge operator masks [directions][rows][cols]
		double mEdgeOpMask[8][3][3]; 

};

#endif /* LDNP_APPEARANCECODE_H__ */
