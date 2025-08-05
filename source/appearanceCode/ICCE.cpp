#include "ICCE.h"
#include "ConfigFileManager.h"
#include "Parser.h"
#include <string>
#include <sstream>
#include <iostream>

ICCE::ICCE() 
{

}

/**
	Creates the appearance code of the class.
	@param input the image to generate the micro features from.
	@return an image after making the appearance code.	
*/


template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }
std::string decimal_tobinary(int dec)
{
	  int rem,i=1,sum=0;
   do {
        rem=dec%2;
        sum=sum + (i*rem);
        dec=dec/2;
        i=i*10;
    }while(dec>0);

   
	std::string str;
	 if (sum==0) str="00"+NumberToString(sum);
	 else if (sum==1) str="00"+NumberToString(sum);
	 else if (sum==10) str="0"+NumberToString(sum);
	 else if (sum==11) str="0"+NumberToString(sum);
	 else  str=NumberToString(sum);

	return str;
}

int binary2decimal(int n)
{ 
	 int factor = 1;
    int total = 0;

    while (n != 0)
    {
        total += (n%10) * factor;
        n /= 10;
        factor *= 2;
    }
return total;

}

std::vector<cv::Mat> ICCE::runCodeGenerate(const std::vector<cv::Mat> &inputVector)
//cv::Mat ICCE::runCodeGenerate(const cv::Mat input)
{
    // sequence support check
    if(inputVector.size() > 1)
        throw KifasExpeption("[ICCE::runCodeGenerate] :ICCE method cannot extract features from sequence images. Check input images are sequence or not \n");
	
    //convert to grayscale
    cv::Mat input = inputVector[0];

	cv::Mat grayinput,Matdst;

   
	cv::cvtColor(input,grayinput,CV_RGB2GRAY);
   

	Matdst.create(grayinput.size().height-2,grayinput.size().width-2,grayinput.type());
	//Kirsh mask
	int Kirsh_mask[8][3][3] = {
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
	int Kirsh_result[8];
	int i,j,k;
	int xsize,ysize;
	int temp;
	xsize = grayinput.size().width;
	ysize = grayinput.size().height;
	uchar *psrc,*pdst,*psrc_upper,*psrc_down; //pointers for neighbors
	
	//aplying the mask convoluted over the image
	for(i=1; i<ysize-1; i++)
	{
		psrc_upper = grayinput.ptr<uchar>(i-1);	
		psrc	   = grayinput.ptr<uchar>(i);	
		psrc_down  = grayinput.ptr<uchar>(i+1);	
		
		pdst = Matdst.ptr<uchar>(i-1);			//pointer to destination pixel
		
		for (j=1; j<xsize-1; j++)
		{
			for(k=0; k<8; k++)
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
				//bool check=true;
				
					//Kirsh_result[k] = abs (temp);

			}

			// LDP code calculation
			pdst[j-1] = calculateLDNPCode(Kirsh_result);



		
		
		}
	}

	
	std::vector<cv::Mat> returnVector;
    returnVector.push_back(Matdst);

	return returnVector;
}



unsigned char ICCE::calculateLDNPCode(int *kirchResult)
{
	int kirsh_temp[8];
	int i,j;
	//int bitset[8] = {0,0,0,0,0,0,0,0};
	int mainDirections[2];	// main edge direction D1 and D2

    int maxdirectionbit;
	int direction_arr[4];
	int Threshold =10;
    int Ternary;

	//create a temporal copy of the results for the pixel in kirsh_temp
	for(i=0; i<8; i++)
		kirsh_temp[i] = kirchResult[i];
	
	
    // get the k values prominents
	for(i=0; i<2; i++)
	{
		int max= -10000000;
		
		mainDirections[i]=0;
		for(j=0; j<8; j++)
		{
			if( kirsh_temp[j] > max)
			{
				max = kirsh_temp[j];
				mainDirections[i] = j;
			}
		}
		kirsh_temp[mainDirections[i]] =-10000000;
        kirsh_temp[(mainDirections[i]+1)%8] =-10000000;
        kirsh_temp[(mainDirections[i]+7)%8] =-10000000;
	}

	// get the prominent 3 direction
	//for(i=0; i<4; i++)
	//{
	//	int max= -1;
	//	int direction =0;
	//	
	//	for(j=0; j<8; j++)
	//	{
	//		if( kirsh_temp[j] > max)
	//		{
	//			max = kirsh_temp[j];
	//			direction = j;
	//		}

	//	}

	//	direction_arr[i]=direction;

	//	bitset[direction] = 1;
	//	kirsh_temp[direction] = -1;
	//	
	//	//to get the maximum response of the pixel
	//	maxdirectionbit = (i==0)? direction:maxdirectionbit;
	//}// end of prominenet direction search


	//if ( abs (direction_arr[0]-direction_arr[1])==1  ||   abs (direction_arr[0]-direction_arr[1])==7 )
	//{
	//	direction_arr[1]=direction_arr[2]	;

	//	if ( abs (direction_arr[0]-direction_arr[2])==1  ||   abs (direction_arr[0]-direction_arr[2])==7 )
	//	
	//		direction_arr[1]=direction_arr[3]	;
	//}

    if(Threshold < kirchResult[mainDirections[0]]){
		Ternary = 1;
	}
	else if((Threshold*-1) > (kirchResult[mainDirections[0]])){
		Ternary = 2;
	}
	else{
		Ternary = 0;
		mainDirections[0] =0;
        mainDirections[1] =0;
	}


    unsigned char r = (32* mainDirections[0])+ (4* mainDirections[1])+ Ternary;
	//// Setting Threshold for magnitude
	//std::string val="00";

	//if (kirsh_temp[ direction_arr[0] ] < Threshold )
	//	val="00"; // binary version of 0,1,2

	//else if (kirsh_temp[ direction_arr[0] ] >Threshold)
	//	val="01";
	//else 
	//	val="10";

	//std::string ternary_pattern= decimal_tobinary( direction_arr[0]) + decimal_tobinary (direction_arr[1]) + val ;
	//
	//// converting string to number (binary pattern will appear)
	//int convt_trPt;
	//std::istringstream ( ternary_pattern ) >>  convt_trPt;
	//
	//int LTP_f_val= binary2decimal(convt_trPt);

	return r;

}

int ICCE::getCodeSize()
{
	return 255;
}
