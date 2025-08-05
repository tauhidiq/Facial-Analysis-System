////////////////////////////////////////////////////////////////////////////
// Implementation of CLBP is based on the following paper:
// Guo, Zhenhua, Lei Zhang, and David Zhang. 
// "A completed modeling of local binary pattern operator for texture classification." 
//Image Processing, IEEE Transactions on 19.6 (2010): 1657-1663.
////////////////////////////////////////////////////////////////////////////

#include "CLBP.h"
#include "ConfigFileManager.h"
#include "Parser.h"

CLBP::CLBP() 
{
	mConfigFile = "LSDP.conf";
	std::map<std::string, kifas::NumStr> ldpconfig;
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, ldpconfig);
	//directions = ldpconfig["codelength"];

	directions =8;
}

/**
	Creates the appearance code of the class.
	@param input the image to generate the micro features from.
	@return an image after making the appearance code.	
*/
std::vector<cv::Mat> CLBP::runCodeGenerate(const std::vector<cv::Mat> &inputVector)
{
    // sequence support check
    if(inputVector.size() > 1)
        throw KifasExpeption("[CLBP::runCodeGenerate] :CLBP method cannot extract features from sequence images. Check input images are sequence or not \n");
	
    //convert to grayscale
    cv::Mat input = inputVector[0];

	cv::Mat grayinput,Matdst;
	cv::cvtColor(input,grayinput,CV_RGB2GRAY);
	Matdst.create(grayinput.size(),grayinput.type());
	//Kirsh mask
	int Kirsh_mask[CLBP::dir][CLBP::rows][CLBP::cols] = {
		{-3, -3,  5, -3, 0,  5, -3, -3,  5},	//EAST
		{-3,  5,  5, -3, 0,  5, -3, -3, -3},	//NORTHEAST
		{ 5,  5,  5, -3, 0, -3, -3, -3, -3},	//NORTH
		{ 5,  5, -3,  5, 0, -3, -3, -3, -3},	//NORTHWEST
		{ 5, -3, -3,  5, 0, -3,  5, -3, -3},	//WEST
		{-3, -3, -3,  5, 0, -3,  5,  5, -3},	//SOUTHWEST
		{-3, -3, -3, -3, 0, -3,  5,  5,  5},	//SOUTH
		{-3, -3, -3, -3, 0,  5, -3,  5,  5}		//SOUTHEAST
	};		
	//general variables
	int Kirsh_result[CLBP::dir];
	int i,j,k;
	int xsize,ysize;
	int temp;
	xsize = grayinput.size().width;
	ysize = grayinput.size().height;
	uchar *psrc,*pdst,*psrc_upper,*psrc_down; //pointers for neighbors
	// add zeros at the beginning/end of x-edge
	for(i=0; i<xsize; i++)
	{
		pdst = Matdst.ptr<uchar>(0);
		pdst[i] = 0;
		pdst = Matdst.ptr<uchar > (ysize-1);
		pdst[i] = 0;
	}
	// add zeros at the beginning/end of y-edge
	for(i=0; i<ysize; i++)
	{
		pdst = Matdst.ptr<uchar>(i,0);
		*pdst = 0;
		pdst = Matdst.ptr<uchar>(i,xsize-1);
		*pdst = 0;
	}
	//aplying the mask convoluted over the image
	for(i=1; i<ysize-1; i++)
	{
		psrc_upper = grayinput.ptr<uchar>(i-1);	//pointer in Y-axis up
		psrc	   = grayinput.ptr<uchar>(i);	//pointer in Y-axis center
		psrc_down  = grayinput.ptr<uchar>(i+1);	//pointer in Y-axis down
		pdst = Matdst.ptr<uchar>(i);			//pointer to pixel destination
		for (j=1; j<xsize-1; j++)
		{
			for(k=0; k<CLBP::directions; k++)
			{
				temp = (
						Kirsh_mask[k][0][0] * (psrc_upper[j-1]) + 
						Kirsh_mask[k][0][1] * (psrc_upper[j]  ) +
						Kirsh_mask[k][0][2] * (psrc_upper[j+1]) +
						Kirsh_mask[k][1][0] * (psrc[j-1]) +
						Kirsh_mask[k][1][1] * (psrc[j]	) +
						Kirsh_mask[k][1][2] * (psrc[j+1]) +
						Kirsh_mask[k][2][0] * (psrc_down[j-1]) +
						Kirsh_mask[k][2][1] * (psrc_down[j]	 ) +
						Kirsh_mask[k][2][2] * (psrc_down[j+1])
						);
				Kirsh_result[k] = temp;
			}
			pdst[j] = calculateLSDPCode(Kirsh_result);
		}
	}

    std::vector<cv::Mat> returnVector;
    returnVector.push_back(Matdst);

	return returnVector;
}

unsigned char CLBP::calculateLSDPCode(int *kirsh_value)
{
	//local variables
	int i,j;
	unsigned char bitset[CLBP::sizeCode] = {0,0,0,0,0,0};
	// get the maximum of (+)/(-) value
	int max = -1, min = kirsh_value[0];
	int dirMax = 0, dirMin = 0;
	for(j=0; j<CLBP::directions; j++)
	{
		if( kirsh_value[j] > max)
		{
			max = kirsh_value[j];
			dirMax = j;
		}
		if( kirsh_value[j] < min)
		{
			min = kirsh_value[j];
			dirMin = j;
		}
	}

	int k,ix=CLBP::sizeCode-1;
	//size: 3 [0-2] because each direction is represented by 3 bits
	//calculate the binary value of the maximum (+) response
	for (int c = (CLBP::sizeCode/2)-1; c >= 0; c--)
	{
		k = dirMax >> c;
		if (k & 1)
			bitset[ix] = 1;
		else
			bitset[ix] = 0;
		ix--;
	}
	//calculate the binary value of the maximum (-) response
	for (int c = (CLBP::sizeCode/2)-1; c >= 0; c--)
	{
		k = dirMin >> c;
		if (k & 1)
			bitset[ix] = 1;
		else
			bitset[ix] = 0;
		ix--;
	}
	
	//convert to binary
	unsigned char r =0;
	for(i=CLBP::sizeCode-1; i>0; i--)
	{
		r =  r | bitset[i];
		r = r<<1;
	}
	r =  r | bitset[0];

	return r;
}
