
// OpenCV
#include <cv.h>
#include "BaseAppearanceCode.h"


/**
The Preprocessing Abstract class. It defines how to get frames.
@ingroup appearanceCode
*/
class PTP : public kifas::BaseAppearanceCode {
public:
	//constructor
	PTP();
	~PTP() {}
	//methods
	unsigned char calculatePTPCode(int *kirsh_value);
	std::vector<cv::Mat> runCodeGenerate(const std::vector<cv::Mat> &input);

	/**
	Calculate the size of the appearance code of the class
	@return the size of the code
	*/
	int getCodeSize() {
		return 255;
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
