#ifndef LDP_APPEARANCECODE_H__
#define LDP_APPEARANCECODE_H__

// OpenCV
#include <cv.h>
#include "BaseAppearanceCode.h"


/**
  The Preprocessing Abstract class. It defines how to get frames.
  @ingroup appearanceCode
*/
class LDP : public kifas::BaseAppearanceCode{
	public:
		//constructor
		LDP(); 
		~LDP() {}
		//methods
		unsigned char calculateLDPCode(int *kirsh_value,int k,int th,bool Lsb_shift);
		std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &input);

		/**
		Calculate the size of the appearance code of the class
		@return the size of the code
		*/
		int getCodeSize();


		/*static int GetDir(){
			return dir;
		}*/
	private:
		/*	Create a Table (mDataMappingTable)
			only the values ??that were taken into account (ActiveBitNumber=3)
			from the total length representation of each bit (codeLength=8)
			for the creation of the code in its binary representation.
		*/
		void makeDataMappingTable();


		//constant parameters
		static const int dir = 8;
		static const int sizeCode = 8;
		static const int rows = 3;
		static const int cols = 3;

		//file.conf
		std::string mConfigFile;
		//internal variables from file
		int directions;
		int kValue; //top-considered
		int Threshold;
		bool AbsoluteMax;
		bool LsbShift;
		
		// mapping table
		//std::map<int, int> mCodeMappingTable;

		int *mDataMappingTable;
		int mCodeDiversity;
};

#endif /* LDP_APPEARANCECODE_H__ */