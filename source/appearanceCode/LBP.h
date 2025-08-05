#ifndef LBP_APPEARANCECODE_H__
#define LBP_APPEARANCECODE_H__

// OpenCV
#include <cv.h>
#include "BaseAppearanceCode.h"


/**
  The Preprocessing Abstract class. It defines how to get frames.
  @ingroup appearanceCode
*/
class LBP : public kifas::BaseAppearanceCode{
	public:
		//constructor
		LBP(); 
		~LBP() {}
		//methods
		void calculateUniformLBPCode(cv::Mat &input);
		//unsigned char biLinear(int valUL, int UR, int DL, int DR);
		std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &input);
        cv::Mat runCodeGenerate_(const cv::Mat input);

		/**
		Calculate the size of the appearance code of the class
		@return the size of the code
		*/
		int getCodeSize();
	private:
		void makeUniformMappingTable();
		//constant parameters
		//static const int dir8 = 8;
		//static const int dir16 = 16;
		//static const int dir24 = 24;
		//static const int dir32 = 32;
		//static const int dir40 = 40;
		std::string mConfigFile;
		//internal variables from file
		//int directions;
		//int radius;
		int m_radius;
		int m_neighbors;
		int m_uniform;
		bool m_IsUniform;

		int *mUniformMappingTable;
		int mUniformCodeDiversity;
		//std::string	shape;
};

#endif /* LDP_APPEARANCECODE_H__ */