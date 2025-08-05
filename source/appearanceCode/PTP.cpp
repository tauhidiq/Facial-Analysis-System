#include "PTP.h"
#include "ConfigFileManager.h"
#include "Parser.h"
#include <opencv2\opencv.hpp>

PTP::PTP()
{
	mConfigFile = "PTP.conf";
	std::map<std::string, kifas::NumStr> ldpconfig;
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, ldpconfig);
	//directions = ldpconfig["codelength"];

	directions = 8;
}

/**
Creates the appearance code of the class.
@param input the image to generate the micro features from.
@return an image after making the appearance code.
*/
std::vector<cv::Mat> PTP::runCodeGenerate(const std::vector<cv::Mat> &input)
{
	cv::Mat grayinput, Matdst;

	if (input[0].channels() >= 3)
		cv::cvtColor(input, grayinput, CV_RGB2GRAY);
	else
		grayinput = input[0];

	//cv::Size rsz(36, 48);
	//cv::Mat res_src;
	//cv::resize(grayinput, grayinput, rsz);

	Matdst.create(grayinput.size().height - 2, grayinput.size().width - 2, grayinput.type());
	//Kirsh mask
	int Kirsh_mask[8][3][3] = {
		{ -3, -3,  5,
		-3,  0,  5,
		-3, -3,  5 },	//EAST
		{ -3,  5,  5,
		-3,  0,  5,
		-3, -3, -3 },	//NORTHEAST
		{ 5,  5,  5,
		-3,  0, -3,
		-3, -3, -3 },	//NORTH
		{ 5,  5, -3,
		5,  0, -3,
		-3, -3, -3 },	//NORTHWEST
		{ 5, -3, -3,
		5,  0, -3,
		5, -3, -3 },	//WEST
		{ -3, -3, -3,
		5,  0, -3,
		5,  5, -3 },	//SOUTHWEST
		{ -3, -3, -3,
		-3,  0, -3,
		5,  5,  5 },	//SOUTH
		{ -3, -3, -3,
		-3,  0,  5,
		-3,  5,  5 }	//SOUTHEAST
	};
	//general variables
	int Kirsh_result[8];
	int i, j, k;
	int xsize, ysize;
	int temp;
	xsize = grayinput.size().width;
	ysize = grayinput.size().height;
	uchar *psrc, *pdst, *psrc_upper, *psrc_down; //pointers for neighbors

	for (i = 1; i<ysize - 1; i++)
	{
		psrc_upper = grayinput.ptr<uchar>(i - 1);	//pointer in Y-axis up
		psrc = grayinput.ptr<uchar>(i);	//pointer in Y-axis center
		psrc_down = grayinput.ptr<uchar>(i + 1);	//pointer in Y-axis down
		pdst = Matdst.ptr<uchar>(i - 1);			//pointer to pixel destination

		for (j = 1; j<xsize - 1; j++)
		{
			for (k = 0; k<8; k++)
			{
				temp = (
					Kirsh_mask[k][0][0] * (psrc_upper[j - 1]) +
					Kirsh_mask[k][0][1] * (psrc_upper[j]) +
					Kirsh_mask[k][0][2] * (psrc_upper[j + 1]) +
					Kirsh_mask[k][1][0] * (psrc[j - 1]) +
					Kirsh_mask[k][1][1] * (psrc[j]) +
					Kirsh_mask[k][1][2] * (psrc[j + 1]) +
					Kirsh_mask[k][2][0] * (psrc_down[j - 1]) +
					Kirsh_mask[k][2][1] * (psrc_down[j]) +
					Kirsh_mask[k][2][2] * (psrc_down[j + 1])
					);
				Kirsh_result[k] = temp;
			}

			pdst[j - 1] = calculatePTPCode(Kirsh_result);



		}
	}

	//cv::imwrite("D://Research//1.PAPER//AVSS_DAPP//NOISE_TEST//PTP//file.png", Matdst);

	std::vector<cv::Mat> returnVector;
	returnVector.push_back(Matdst);
	return returnVector;

}

unsigned char PTP::calculatePTPCode(int *kirsh_value)
{
	//local variables
	int kirsh_temp[8];
	int i, j;
	unsigned char bitset[8] = { 0,0,0,0,0,0,0,0 };
	int maxdirectionbit;
	//create a temporal copy of the results for the pixel in kirsh_temp
	for (i = 0; i<8; i++)
		kirsh_temp[i] = kirsh_value[i];

	int macro_code = 0;


	// get the k values prominents
	int max_dir[3];
	for (i = 0; i<3; i++)
	{
		int max = -1;

		max_dir[i] = 0;
		for (j = 0; j<8; j++)
		{
			if (kirsh_temp[j] > max)
			{
				max = kirsh_temp[j];
				max_dir[i] = j;
			}
		}
		kirsh_temp[max_dir[i]] = -1;
	}



	// Ternary pattern

	int ternary = 0, Ternary_threshold = 15;

	if (kirsh_value[max_dir[0]] < (Ternary_threshold*-1))
		ternary = 1;
	else if (kirsh_value[max_dir[0]] > Ternary_threshold)
		ternary = 2;
	else
		ternary = 0;


	// FInal Code
	int final_code = 0;


	//discarding neighboring secondary direction
	int diff = abs(max_dir[0] - max_dir[1]);
	if (diff == 1 || diff == 7)
		max_dir[1] = max_dir[2];
	/*if ( diff==1 || diff==7 || ternary==0)
	final_code=0;
	else*/

	final_code = 32 * max_dir[0] + 4 * max_dir[1] + ternary;


	return final_code;
}