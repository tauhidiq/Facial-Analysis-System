#include "LDP.h"
#include "ConfigFileManager.h"
#include "Parser.h"

LDP::LDP() 
{
	mConfigFile = "LDP.conf";
	std::map<std::string, kifas::NumStr> ldpconfig;
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, ldpconfig);
	
	directions = ldpconfig["codelength"];
	kValue = ldpconfig["kvalue"];
	Threshold = ldpconfig["threshold"];
	
    
    std::string attributevalue = ldpconfig["absolutemax"];
	// convert capitals to lower
	std::transform(attributevalue.begin(), attributevalue.end(), attributevalue.begin(), ::tolower);
	
    if(attributevalue.compare("true") == 0)
		AbsoluteMax = true;
	else if(attributevalue.compare("false")==0)
		AbsoluteMax = false;
	else 
		TraceMessage::addMessage("In LDP.conf, absolutemax is set unknown value as "+attributevalue);
	

	attributevalue = std::string(ldpconfig["lsbshift"]);
    // convert capitals to lower
	std::transform(attributevalue.begin(), attributevalue.end(), attributevalue.begin(), ::tolower);
	
    if(attributevalue.compare("true") == 0)
		LsbShift = true;
	else if(attributevalue.compare("false")==0)
		LsbShift = false;
	else 
		TraceMessage::addMessage("In LDP.conf, lsbshift is set unknown value as "+attributevalue);
	

	mDataMappingTable= NULL;
	mCodeDiversity =0;
}

/**
	Creates the appearance code of the class.
	@param input the image to generate the micro features from.
	@return an image after making the appearance code.	
*/
std::vector<cv::Mat> LDP::runCodeGenerate(const std::vector<cv::Mat> &inputVector)
//cv::Mat LDP::runCodeGenerate(const cv::Mat input)
{
    // sequence support check
    if(inputVector.size() > 1)
        throw KifasExpeption("[LDP::runCodeGenerate] :LDP method cannot extract features from sequence images. Check input images are sequence or not \n");
	
    //convert to grayscale
    cv::Mat input = inputVector[0];


	// 코드 맵핑 테이블이 없다면 생성해 준다. 
	if(mDataMappingTable == NULL)
		LDP::makeDataMappingTable();

	//convert to grayscale
	cv::Mat grayinput,Matdst;

    if(input.channels() >= 3 )
	    cv::cvtColor(input,grayinput,CV_RGB2GRAY);
    else
        grayinput = input;

	Matdst.create(grayinput.size().height-2,grayinput.size().width-2,grayinput.type());
	//Kirsh mask
	int Kirsh_mask[LDP::dir][LDP::rows][LDP::cols] = {
		{-3, -3,  5,
		 -3,  0,  5, 
		 -3, -3,  5},	//EAST
		{-3,  5,  5, 
		 -3,  0,  5, 
		 -3, -3, -3},	//NORTHEAST
		{ 5,  5,  5, 
		 -3,  0, -3, 
		 -3, -3, -3},	//NORTH
		{ 5,  5, -3,  
		  5,  0, -3, 
		 -3, -3, -3},	//NORTHWEST
		{ 5, -3, -3,  
		  5,  0, -3,  
		  5, -3, -3},	//WEST
		{-3, -3, -3,  
		  5,  0, -3,  
		  5,  5, -3},	//SOUTHWEST
		{-3, -3, -3, 
		 -3,  0, -3,  
		  5,  5,  5},	//SOUTH
		{-3, -3, -3, 
		 -3,  0,  5, 
		 -3,  5,  5}	//SOUTHEAST
	};		
	//general variables
	int Kirsh_result[LDP::dir];
	int i,j,k;
	int xsize,ysize;
	int temp;
	xsize = grayinput.size().width;
	ysize = grayinput.size().height;
	uchar *psrc,*pdst,*psrc_upper,*psrc_down; //pointers for neighbors
	// add zeros at the beginning/end of x-edge
	//for(i=0; i<(xsize-2); i++)
	//{
	//	pdst = Matdst.ptr<uchar>(0);
	//	pdst[i] = 255;
	//	pdst = Matdst.ptr<uchar > (ysize-1);
	//	pdst[i] = 255;
	//}
	//// add zeros at the beginning/end of y-edge
	//for(i=0; i<(ysize-2); i++)
	//{
	//	pdst = Matdst.ptr<uchar>(i,0);
	//	*pdst = 255;
	//	pdst = Matdst.ptr<uchar>(i,xsize-1);
	//	*pdst = 255;
	//}
	//aplying the mask convoluted over the image
	for(i=1; i<ysize-1; i++)
	{
		psrc_upper = grayinput.ptr<uchar>(i-1);	//pointer in Y-axis up
		psrc	   = grayinput.ptr<uchar>(i);	//pointer in Y-axis center
		psrc_down  = grayinput.ptr<uchar>(i+1);	//pointer in Y-axis down
		pdst = Matdst.ptr<uchar>(i-1);			//pointer to pixel destination
		for (j=1; j<xsize-1; j++)
		{
			for(k=0; k<LDP::directions; k++)
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
				// check if the response is negative to eliminate the sign, in other words absolute value
				if (LDP::AbsoluteMax)
					Kirsh_result[k] = (temp>0) ? temp:(temp *-1);
				else
					Kirsh_result[k] = temp;

			}
			pdst[j-1] = calculateLDPCode(Kirsh_result,LDP::kValue,LDP::Threshold,LDP::LsbShift);


			if(pdst[j-1] ==0)
				pdst[j-1] = 255;

			// code mapping 0~code diversity
			for(int m = 0; m < mCodeDiversity; m++)
			{
				if(pdst[j-1] == mDataMappingTable[m])
				{
					pdst[j-1] = m;
					break;
				}
			}


		}
	}
	std::vector<cv::Mat> returnVector;
    returnVector.push_back(Matdst);

	return returnVector;
}

unsigned char LDP::calculateLDPCode(int *kirsh_value,int k,int th,bool Lsb_shift)
{
	//local variables
	int kirsh_temp[LDP::dir];
	int i,j;
	unsigned char bitset[LDP::sizeCode] = {0,0,0,0,0,0,0,0};;
	int maxdirectionbit;
	//create a temporal copy of the results for the pixel in kirsh_temp
	for(i=0; i<LDP::directions; i++)
		kirsh_temp[i] = kirsh_value[i];
	// get the k values prominents
	for(i=0; i<k; i++)
	{
		int max= -1;
		int direction =0;
		for(j=0; j<LDP::directions; j++)
		{
			if( kirsh_temp[j] > max)
			{
				max = kirsh_temp[j];
				direction = j;
			}

		}
		bitset[direction] = 1;
		kirsh_temp[direction] = -1;
		//to get the maximum response of the pixel
		maxdirectionbit = (i==0)? direction:maxdirectionbit;
	}

	//discard the pixel if the maximum magnitude response is less than the threshold
	//if(kirsh_value[maxdirectionbit] < th)
	//	return 0;

	//calculate the binary value
	unsigned char r =0;
	for(i=LDP::sizeCode-1; i>0; i--)
	{
		r =  r | bitset[i];
		r = r<<1;
	}
	r =  r | bitset[0];

	if(Lsb_shift)
	{
		unsigned char shift_save=1;
		for(i=0; i<maxdirectionbit; i++)
		{
			shift_save =  r & shift_save;
			shift_save = shift_save <<7;

			r = r >> 1;
			r = r| shift_save;

			shift_save = 1;
		}
	}
	return r;
}

/**
	Calculate the size of the appearance code of the class
	@return the size of the code
*/
int LDP::getCodeSize()
{
	int n, r;
	n = sizeCode;
	r = kValue;

	int iteration = r;

	int i;
	int denominator=1;
	int nominator=1;
	for(i=0; i< iteration; i++)
	{
		nominator *= n;
		denominator *= r;
		n= n-1;
		r= r-1;
	}

	return nominator/ denominator;
}

/*	Create a Table (mDataMappingTable)
	only the values ??that were taken into account (ActiveBitNumber=3)
	from the total length representation of each bit (codeLength=8)
	for the creation of the code in its binary representation.
*/
void LDP::makeDataMappingTable()
{
	unsigned char k = 0;
	unsigned char check_bit[8];
	int i,j;
	int check_value = 1;
	int bit_number;
	int tablecount = 0;
	mCodeDiversity=getCodeSize(); // total code diversity caculated by Combination

	mDataMappingTable = new int[mCodeDiversity];	// dynamic allocation 
	
	//check_bit: create a binary array from 1-128
	for(i=0; i<8; i++)
	{
		check_bit[i] = check_value;
		check_value = check_value<<1;
	}

	for(j=0; j <= 255; j++)
	{
		k = j;
		bit_number = 0;
		for(i=0; i< 8; i++)
		{
			if( (k & check_bit[i]) == check_bit[i])
				bit_number++;
		}
		//save the numbers from [0-255] 
		//which in binary representation is written with 
		//#ActiveBitNumber (1)'s
		//ex. ActiveBitNumber = 4, all the bin number with four ones.
		if(bit_number == kValue)
		{
			mDataMappingTable[tablecount] = k;
			tablecount++;
		}
	}




}