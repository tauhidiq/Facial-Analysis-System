#ifndef CLBP_APPEARANCECODE_H__
#define CLBP_APPEARANCECODE_H__

// OpenCV
#include <cv.h>
#include "BaseAppearanceCode.h"


/**
  The Preprocessing Abstract class. It defines how to get frames.
  @ingroup appearanceCode
*/
class CLBP : public kifas::BaseAppearanceCode{
	public:
		//constructor
		CLBP(); 
		~CLBP() {}
		//methods
		unsigned char calculateLSDPCode(int *kirsh_value);
		std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &inputVector);

		/**
		Calculate the size of the appearance code of the class
		@return the size of the code
		*/
		int getCodeSize(){
			return 100;
		}

	private:
		//constant parameters
		static const int dir = 8;
		static const int sizeCode = 6;
		static const int rows = 3;
		static const int cols = 3;
		//file.conf
		std::string mConfigFile;
		//internal variables from file
		int directions;
};

#endif