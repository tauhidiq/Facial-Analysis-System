#include "LDNP.h"
#include "ConfigFileManager.h"
#include "Parser.h"

LDNP::LDNP() 
{
	mConfigFile = "LDNP.conf";
	std::map<std::string, kifas::NumStr> ldpconfig;
	ldpconfig["masktype"] = "kirsch";
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, ldpconfig);
	

	std::string attributevalue = ldpconfig["masktype"];
    // convert capitals to lower
	std::transform(attributevalue.begin(), attributevalue.end(), attributevalue.begin(), ::tolower);
	if(attributevalue.compare("kirsch") == 0)
	{
		mMaskType = KIRSCH;
	}
	else if(attributevalue.compare("gaussian")==0)
	{
		mMaskType = GAUSSIAN;
	}
	else 
	{
		mMaskType = KIRSCH;
		TraceMessage::addMessage("In LDNP.conf, masktype is set unknown value as "+attributevalue);
		TraceMessage::addMessage("Kirsch as default mask type, will be used.");
	}
	
	if(mMaskType == KIRSCH)
	{
		//EAST
		mEdgeOpMask[0][0][0] = -3; 	mEdgeOpMask[0][0][1] = -3; 	mEdgeOpMask[0][0][2] =  5;
		mEdgeOpMask[0][1][0] = -3; 	mEdgeOpMask[0][1][1] =  0; 	mEdgeOpMask[0][1][2] =  5;
		mEdgeOpMask[0][2][0] = -3; 	mEdgeOpMask[0][2][1] = -3; 	mEdgeOpMask[0][2][2] =  5;

		//NORTHEAST
		mEdgeOpMask[1][0][0] = -3; 	mEdgeOpMask[1][0][1] =  5; 	mEdgeOpMask[1][0][2] =  5;
		mEdgeOpMask[1][1][0] = -3; 	mEdgeOpMask[1][1][1] =  0; 	mEdgeOpMask[1][1][2] =  5;
		mEdgeOpMask[1][2][0] = -3; 	mEdgeOpMask[1][2][1] = -3; 	mEdgeOpMask[1][2][2] = -3;

		//NORTH
		mEdgeOpMask[2][0][0] =  5; 	mEdgeOpMask[2][0][1] =  5; 	mEdgeOpMask[2][0][2] =  5;
		mEdgeOpMask[2][1][0] = -3; 	mEdgeOpMask[2][1][1] =  0; 	mEdgeOpMask[2][1][2] = -3;
		mEdgeOpMask[2][2][0] = -3; 	mEdgeOpMask[2][2][1] = -3; 	mEdgeOpMask[2][2][2] = -3;

		//NORTHWEST
		mEdgeOpMask[3][0][0] =  5; 	mEdgeOpMask[3][0][1] =  5; 	mEdgeOpMask[3][0][2] = -3;
		mEdgeOpMask[3][1][0] =  5; 	mEdgeOpMask[3][1][1] =  0; 	mEdgeOpMask[3][1][2] = -3;
		mEdgeOpMask[3][2][0] = -3; 	mEdgeOpMask[3][2][1] = -3; 	mEdgeOpMask[3][2][2] = -3;

		//WEST
		mEdgeOpMask[4][0][0] =  5; 	mEdgeOpMask[4][0][1] = -3; 	mEdgeOpMask[4][0][2] = -3;
		mEdgeOpMask[4][1][0] =  5; 	mEdgeOpMask[4][1][1] =  0; 	mEdgeOpMask[4][1][2] = -3;
		mEdgeOpMask[4][2][0] =  5; 	mEdgeOpMask[4][2][1] = -3; 	mEdgeOpMask[4][2][2] = -3;

		//SOUTHWEST
		mEdgeOpMask[5][0][0] = -3; 	mEdgeOpMask[5][0][1] = -3; 	mEdgeOpMask[5][0][2] = -3;
		mEdgeOpMask[5][1][0] =  5; 	mEdgeOpMask[5][1][1] =  0; 	mEdgeOpMask[5][1][2] = -3;
		mEdgeOpMask[5][2][0] =  5; 	mEdgeOpMask[5][2][1] =  5; 	mEdgeOpMask[5][2][2] = -3;

		//SOUTH
		mEdgeOpMask[6][0][0] = -3; 	mEdgeOpMask[6][0][1] = -3; 	mEdgeOpMask[6][0][2] = -3;
		mEdgeOpMask[6][1][0] = -3; 	mEdgeOpMask[6][1][1] =  0; 	mEdgeOpMask[6][1][2] = -3;
		mEdgeOpMask[6][2][0] =  5; 	mEdgeOpMask[6][2][1] =  5; 	mEdgeOpMask[6][2][2] =  5;

		//SOUTHEAST
		mEdgeOpMask[7][0][0] = -3; 	mEdgeOpMask[7][0][1] = -3; 	mEdgeOpMask[7][0][2] = -3;
		mEdgeOpMask[7][1][0] = -3; 	mEdgeOpMask[7][1][1] =  0; 	mEdgeOpMask[7][1][2] =  5;
		mEdgeOpMask[7][2][0] = -3; 	mEdgeOpMask[7][2][1] =  5; 	mEdgeOpMask[7][2][2] =  5;
	}
	else
	{
		// EAST
		mEdgeOpMask[0][0][0] = 0.00000263514172907059; 	mEdgeOpMask[0][0][1] =  0; 	mEdgeOpMask[0][0][2] =  -0.00000263514172907059;
		mEdgeOpMask[0][1][0] = 0.707106781176727; 	    mEdgeOpMask[0][1][1] =  0; 	mEdgeOpMask[0][1][2] =  -0.707106781176727;
		mEdgeOpMask[0][2][0] = 0.00000263514172907059; 	mEdgeOpMask[0][2][1] =  0; 	mEdgeOpMask[0][2][2] =  -0.00000263514172907059;

		//NORTHEAST
		mEdgeOpMask[1][0][0] = 0; 	                    mEdgeOpMask[1][0][1] =  -0.00000263514172907059; 	mEdgeOpMask[1][0][2] =  -0.707106781176727;
		mEdgeOpMask[1][1][0] = 0.00000263514172907059; 	mEdgeOpMask[1][1][1] =  0; 	                        mEdgeOpMask[1][1][2] =  -0.00000263514172907059;
		mEdgeOpMask[1][2][0] = 0.707106781176727; 	    mEdgeOpMask[1][2][1] =  0.00000263514172907059; 	mEdgeOpMask[1][2][2] =  0;

		//NORTH
		mEdgeOpMask[2][0][0] =  -0.00000263514172907059; 	mEdgeOpMask[2][0][1] =  -0.707106781176727; 	mEdgeOpMask[2][0][2] =  -0.00000263514172907059;
		mEdgeOpMask[2][1][0] =   0; 	                    mEdgeOpMask[2][1][1] =  0; 	                    mEdgeOpMask[2][1][2] =  0;
		mEdgeOpMask[2][2][0] =   0.00000263514172907059; 	mEdgeOpMask[2][2][1] =  0.707106781176727; 	    mEdgeOpMask[2][2][2] =  0.00000263514172907059;

		//NORTHWEST
		mEdgeOpMask[3][0][0] =  -0.707106781176727; 	  mEdgeOpMask[3][0][1] =  -0.00000263514172907059; 	mEdgeOpMask[3][0][2] =  0;
		mEdgeOpMask[3][1][0] =  -0.00000263514172907059;  mEdgeOpMask[3][1][1] =   0; 	                    mEdgeOpMask[3][1][2] =  0.00000263514172907059;
		mEdgeOpMask[3][2][0] =   0; 	                  mEdgeOpMask[3][2][1] =   0.00000263514172907059;  mEdgeOpMask[3][2][2] =  0.707106781176727;

		//WEST
		mEdgeOpMask[4][0][0] = -0.00000263514172907059; mEdgeOpMask[4][0][1] = 0; 	mEdgeOpMask[4][0][2] = 0.00000263514172907059;
		mEdgeOpMask[4][1][0] = -0.707106781176727; 	    mEdgeOpMask[4][1][1] = 0; 	mEdgeOpMask[4][1][2] = 0.707106781176727;
		mEdgeOpMask[4][2][0] = -0.00000263514172907059; mEdgeOpMask[4][2][1] = 0; 	mEdgeOpMask[4][2][2] = 0.00000263514172907059;

		//SOUTHWEST
		mEdgeOpMask[5][0][0] = 0; 	                    mEdgeOpMask[5][0][1] =  0.00000263514172907059; 	mEdgeOpMask[5][0][2] =  0.707106781176727;
		mEdgeOpMask[5][1][0] = -0.00000263514172907059; mEdgeOpMask[5][1][1] =  0; 	                        mEdgeOpMask[5][1][2] =  0.00000263514172907059;
		mEdgeOpMask[5][2][0] = -0.707106781176727; 	    mEdgeOpMask[5][2][1] =  -0.00000263514172907059; 	mEdgeOpMask[5][2][2] =  0;

		//SOUTH
		mEdgeOpMask[6][0][0] =  0.00000263514172907059; 	mEdgeOpMask[6][0][1] =  0.707106781176727; 	mEdgeOpMask[6][0][2] =  0.00000263514172907059;
		mEdgeOpMask[6][1][0] =   0; 	                    mEdgeOpMask[6][1][1] =  0; 	                mEdgeOpMask[6][1][2] =  0;
		mEdgeOpMask[6][2][0] =  -0.00000263514172907059; 	mEdgeOpMask[6][2][1] =  -0.707106781176727; mEdgeOpMask[6][2][2] =  -0.00000263514172907059;
				
		//SOUTHEAST
		mEdgeOpMask[7][0][0] =  0.707106781176727;      mEdgeOpMask[7][0][1] =  0.00000263514172907059;  mEdgeOpMask[7][0][2] =  0;
		mEdgeOpMask[7][1][0] =  0.00000263514172907059; mEdgeOpMask[7][1][1] =  0;                       mEdgeOpMask[7][1][2] = -0.00000263514172907059;
		mEdgeOpMask[7][2][0] =  0;                      mEdgeOpMask[7][2][1] = -0.00000263514172907059;  mEdgeOpMask[7][2][2] = -0.707106781176727;
	}

}

/**
	Creates the appearance code of the class.
	@param input the image to generate the micro features from.
	@return an image after making the appearance code.	
*/
std::vector<cv::Mat> LDNP::runCodeGenerate(const std::vector<cv::Mat> &inputVector)
{
    // sequence support check
    if(inputVector.size() > 1)
        throw KifasExpeption("[LDNP::runCodeGenerate] :LDNP method cannot extract features from sequence images. Check input images are sequence or not \n");
	
    //convert to grayscale
    cv::Mat input = inputVector[0];

	//convert to grayscale
	cv::Mat grayinput,Matdst;

    if(input.channels() > 1)
	    cv::cvtColor(input,grayinput,CV_RGB2GRAY);
    else
        input.copyTo(grayinput);
	
	Matdst.create(grayinput.size().height-2,grayinput.size().width-2,CV_8UC1);

	//general variables
	double Mask_result[LDNP::dir];
	int i,j,k;
	int xsize,ysize;
	double temp;
	xsize = grayinput.size().width;
	ysize = grayinput.size().height;
	uchar *psrc,*psrc_upper,*psrc_down; //pointers for neighbors
	uchar *pdst;  
	
	//aplying the mask convoluted over the image
	for(i=1; i<ysize-1; i++)
	{
		psrc_upper = grayinput.ptr<uchar>(i-1);	//pointer in Y-axis up
		psrc	   = grayinput.ptr<uchar>(i);	//pointer in Y-axis center
		psrc_down  = grayinput.ptr<uchar>(i+1);	//pointer in Y-axis down
		
		pdst = Matdst.ptr<uchar>(i-1);			//pointer to pixel destination
		for (j=1; j<xsize-1; j++)
		{
			for(k=0; k<LDNP::dir; k++)
			{
				temp = (
						mEdgeOpMask[k][0][0] * (psrc_upper[j-1]) + 
						mEdgeOpMask[k][0][1] * (psrc_upper[j]  ) +
						mEdgeOpMask[k][0][2] * (psrc_upper[j+1]) +
						mEdgeOpMask[k][1][0] * (psrc[j-1]) +
						mEdgeOpMask[k][1][1] * (psrc[j]	) +
						mEdgeOpMask[k][1][2] * (psrc[j+1]) +
						mEdgeOpMask[k][2][0] * (psrc_down[j-1]) +
						mEdgeOpMask[k][2][1] * (psrc_down[j]	 ) +
						mEdgeOpMask[k][2][2] * (psrc_down[j+1])
						);
				Mask_result[k] = temp;

			}
			pdst[j-1] = calculateLDNPCode(Mask_result);
		}
	}


	std::vector<cv::Mat> returnVector;
    returnVector.push_back(Matdst);

	return returnVector;
}

unsigned char LDNP::calculateLDNPCode(double *Mask_result)
{
	//local variables
	double MaskResult_temp[LDNP::dir];
	int i,j;

	// main edge direction D1 and D2
	// D1 Max pos
	// D2 Max neg 
	int mainDirections[2];	
	
	//create a temporal copy of the results for the pixel in kirsh_temp
	for(i=0; i<8; i++)
		MaskResult_temp[i] = (Mask_result[i]>0) ? Mask_result[i]:(Mask_result[i] *-1);;


	double max= -10000;
	double min= 10000;	
	
	// find max positive
	mainDirections[0]=0;
	for(i=0; i<8; i++)
	{
		if( MaskResult_temp[i] > max)
		{
			max = MaskResult_temp[i];
			mainDirections[0] = i;
		}
	}

	// find max negative
	mainDirections[1]=0;
	for(i=0; i<8; i++)
	{
		if( MaskResult_temp[i] < min)
		{
			min = MaskResult_temp[i];
			mainDirections[1] = i;
		}
	}

	// make final code 
	unsigned char r = (unsigned char)(8 * mainDirections[0] + mainDirections[1]);

	return r;
}

int LDNP::getCodeSize()
{
	return 8*8;
}
