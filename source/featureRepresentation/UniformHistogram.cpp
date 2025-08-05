#include "UniformHistogram.h"
#include "Definitions.h"
#include "ConfigFileManager.h"
#include "Parser.h"
#include <opencv2/highgui/highgui.hpp>


/**
  Initializes the UnifomHistogram grid class with the values specified in the configuration file, in case of error it uses the defaults values.
  The defaults values are:
  - Xgrid = 10
  - Ygrid = 10
  - CodeLength = 3
  - ActiveBitNumber = 8
  - CodeDiversity = 56  
*/
UniformHistogram::UniformHistogram() 
{
	mConfigFile = "UniformHistogram.conf";
  
	// Defaults values from config file
	std::map<std::string, kifas::NumStr> ldpconfig;
	ldpconfig["xgrid"] = 8;
	ldpconfig["ygrid"] = 9;
	ldpconfig["includeremains"] = "false";

	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, ldpconfig);
		
	// set the values
	mXgrid = ldpconfig["xgrid"];
	mYgrid= ldpconfig["ygrid"];
	
	std::string attributevalue = ldpconfig["includeremains"];
	if(attributevalue.compare("true") == 0)
	{
		mIncludeRemains = true;
	}
	else if(attributevalue.compare("false")==0)
	{
		mIncludeRemains = false;
	}
	else 
	{
		mIncludeRemains = false;
		TraceMessage::addMessage("In UniformHistogram.conf, IncludeRemains is set unknown value as "+attributevalue);
		TraceMessage::addMessage("false as default IncludeRemains, will be used.");
	}
}

/**
* Perform Preprocessing
* @param input is the coded image to represent.
* @return image after preprocessing.	
*/
cv::Mat UniformHistogram::runFeatureVectors(std::vector<cv::Mat> &inputVector)
{
    // sequence support check
    /*if(inputVector.size() > 1)
        throw KifasExpeption("[CLBP::runCodeGenerate] :CLBP method cannot extract features from sequence images. Check input images are sequence or not \n");
	*/
    //convert to grayscale

    // set size and type of a feature vector 
	cv::Mat featureVector = cv::Mat(1,getFeatureLength(),CV_32S );
	
	// initially set current feature vector as zero
	featureVector.setTo(0);

    int i;
    for(i=0; i< inputVector.size(); i++)
    {
        int startPos = i* mCodeDiversity * mXgrid * mYgrid;
        int *featurevectorPtr = featureVector.ptr<int>(0,startPos);

        cv::Mat input = inputVector[i];
        runFeatureVectors_(input,featurevectorPtr);
    }
 //   


	//// 1. determine vector dimensions
	//// 2. calculate vectors
	////cv::imshow("Display window1", input);  
	////cv::waitKey(0);

	////if(mDataMappingTable == NULL)
	////	UniformHistogram::makeDataMappingTable();
	//if(mCodeDiversity == 0)
	//	throw KifasExpeption("[UniformHistogram::runFeatureVectors] : the number of bins can not be calculated because code diversiy is zero!!");

	//int blockXsize;
	//int blockYsize;
	//int lastblockXsize;
	//int lastblockYsize;

	//// Get input Type
	//int inputType = input.type();

	//// Get input Size
	//int inputWidth = input.size().width;
	//int inputHeight = input.size().height;

	//// block sizes
	//blockXsize = inputWidth /mXgrid;
	//blockYsize = inputHeight /mYgrid;

	//// last block size
	//lastblockXsize =  inputWidth - (blockXsize * mXgrid);
	//lastblockYsize =  inputHeight - (blockYsize * mYgrid);

	//// set size and type of a feature vector 
	//cv::Mat featureVector = cv::Mat(1,getFeatureLength(),CV_32S );
	//
	//// initially set current feature vector as zero
	//featureVector.setTo(0);

	//// for loop
	//int i,j,k,l,m;
	//
	//// featurevector pointer
	//int *featurevectorPtr = featureVector.ptr<int>(0);

	////  0-> 1-> 2-> 3-> 4->
	////  5-> 6-> 7-> 8-> 9->
	//// ..........
	////int tbl[56];
	////for (int a=0; a<56; a++)
	////{
	////	tbl[a] = mDataMappingTable[a];
	////}

	///*	-->
	//	-->
	//	-->		*/
	//int accpixels =0;
	//if(inputType == CV_8U)
	//{
	//	for(i=0; i< mYgrid; i++)			// loop y grid
	//	{
	//		for(j=0; j< mXgrid; j++)		// loop x grid
	//		{
	//			//variables to scan the image with squares
	//			int startx,starty,endx,endy;
	//			startx = j * blockXsize;
	//			starty = i * blockYsize;
	//			endx = startx + blockXsize;
	//			endy = starty + blockYsize;
	//			//to adjust the end, if it is the final square
	//			if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
	//				endx = endx + lastblockXsize;
	//			if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
	//				endy = endy + lastblockYsize;
	//			//if it is the column > 1
	//			int offset = (i * mXgrid + j) * mCodeDiversity;
	//			//to compare the pixel values of the delimited square
	//			for(k=starty; k< endy; k++)
	//			{
	//				for(l=startx; l< endx; l++)
	//				{
	//					uchar valuePix = input.at<uchar>(k,l);
	//					//search the pixel value in the binary table 
	//					//and increment in 1 the value
	//					//for(m = 0; m < mCodeDiversity; m++)
	//					//{
	//					//	if(valuePix == mDataMappingTable[m])
	//					//	{
	//					if(valuePix >=0 && valuePix < mCodeDiversity)
	//					{
	//						featurevectorPtr[offset + valuePix]++;
	//						accpixels++;
	//					}
	//					//		break;
	//					//	}
	//					//}
	//				}
	//			}
	//			//end for the loops for the delimited square
	//		}
	//	}
	//}
	//else if(inputType == CV_8S)
	//{
	//	for(i=0; i< mYgrid; i++)			// loop y grid
	//	{
	//		for(j=0; j< mXgrid; j++)		// loop x grid
	//		{
	//			//variables to scan the image with squares
	//			int startx,starty,endx,endy;
	//			startx = j * blockXsize;
	//			starty = i * blockYsize;
	//			endx = startx + blockXsize;
	//			endy = starty + blockYsize;
	//			//to adjust the end, if it is the final square
	//			if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
	//				endx = endx + lastblockXsize;
	//			if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
	//				endy = endy + lastblockYsize;
	//			//if it is the column > 1
	//			int offset = (i * mXgrid + j) * mCodeDiversity;
	//			//to compare the pixel values of the delimited square
	//			for(k=starty; k< endy; k++)
	//			{
	//				for(l=startx; l< endx; l++)
	//				{
	//					char valuePix = input.at<char>(k,l);
	//					//search the pixel value in the binary table 
	//					//and increment in 1 the value
	//					//for(m = 0; m < mCodeDiversity; m++)
	//					//{
	//					//	if(valuePix == mDataMappingTable[m])
	//					//	{
	//					featurevectorPtr[offset + valuePix]++;
	//					//		break;
	//					//	}
	//					//}
	//				}
	//			}
	//			//end for the loops for the delimited square
	//		}
	//	}
	//}
	//else if(inputType == CV_16U)
	//{
	//	for(i=0; i< mYgrid; i++)			// loop y grid
	//	{
	//		for(j=0; j< mXgrid; j++)		// loop x grid
	//		{
	//			//variables to scan the image with squares
	//			int startx,starty,endx,endy;
	//			startx = j * blockXsize;
	//			starty = i * blockYsize;
	//			endx = startx + blockXsize;
	//			endy = starty + blockYsize;
	//			//to adjust the end, if it is the final square
	//			if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
	//				endx = endx + lastblockXsize;
	//			if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
	//				endy = endy + lastblockYsize;
	//			//if it is the column > 1
	//			int offset = (i * mXgrid + j) * mCodeDiversity;
	//			//to compare the pixel values of the delimited square
	//			for(k=starty; k< endy; k++)
	//			{
	//				for(l=startx; l< endx; l++)
	//				{
	//					unsigned short valuePix = input.at<unsigned short>(k,l);
	//					//search the pixel value in the binary table 
	//					//and increment in 1 the value
	//					//for(m = 0; m < mCodeDiversity; m++)
	//					//{
	//					//	if(valuePix == mDataMappingTable[m])
	//					//	{
	//					featurevectorPtr[offset + valuePix]++;
	//					//		break;
	//					//	}
	//					//}
	//				}
	//			}
	//			//end for the loops for the delimited square
	//		}
	//	}
	//}
	//else if(inputType == CV_16S)
	//{
	//	for(i=0; i< mYgrid; i++)			// loop y grid
	//	{
	//		for(j=0; j< mXgrid; j++)		// loop x grid
	//		{
	//			//variables to scan the image with squares
	//			int startx,starty,endx,endy;
	//			startx = j * blockXsize;
	//			starty = i * blockYsize;
	//			endx = startx + blockXsize;
	//			endy = starty + blockYsize;
	//			//to adjust the end, if it is the final square
	//			if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
	//				endx = endx + lastblockXsize;
	//			if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
	//				endy = endy + lastblockYsize;
	//			//if it is the column > 1
	//			int offset = (i * mXgrid + j) * mCodeDiversity;
	//			//to compare the pixel values of the delimited square
	//			for(k=starty; k< endy; k++)
	//			{
	//				for(l=startx; l< endx; l++)
	//				{
	//					short valuePix = input.at<short>(k,l);
	//					//search the pixel value in the binary table 
	//					//and increment in 1 the value
	//					//for(m = 0; m < mCodeDiversity; m++)
	//					//{
	//					//	if(valuePix == mDataMappingTable[m])
	//					//	{
	//					featurevectorPtr[offset + valuePix]++;
	//					//		break;
	//					//	}
	//					//}
	//				}
	//			}
	//			//end for the loops for the delimited square
	//		}
	//	}
	//}
	//else if(inputType == CV_32S)
	//{
	//	for(i=0; i< mYgrid; i++)			// loop y grid
	//	{
	//		for(j=0; j< mXgrid; j++)		// loop x grid
	//		{
	//			//variables to scan the image with squares
	//			int startx,starty,endx,endy;
	//			startx = j * blockXsize;
	//			starty = i * blockYsize;
	//			endx = startx + blockXsize;
	//			endy = starty + blockYsize;
	//			//to adjust the end, if it is the final square
	//			if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
	//				endx = endx + lastblockXsize;
	//			if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
	//				endy = endy + lastblockYsize;
	//			//if it is the column > 1
	//			int offset = (i * mXgrid + j) * mCodeDiversity;
	//			//to compare the pixel values of the delimited square
	//			for(k=starty; k< endy; k++)
	//			{
	//				for(l=startx; l< endx; l++)
	//				{
	//					int valuePix = input.at<int>(k,l);
	//					//search the pixel value in the binary table 
	//					//and increment in 1 the value
	//					//for(m = 0; m < mCodeDiversity; m++)
	//					//{
	//					//	if(valuePix == mDataMappingTable[m])
	//					//	{
	//					if(valuePix >=0 && valuePix < mCodeDiversity)
	//					{
	//						featurevectorPtr[offset + valuePix]++;
	//					}

	//					//						featurevectorPtr[offset + valuePix]++;
	//					//		break;
	//					//	}
	//					//}
	//				}
	//			}
	//			//end for the loops for the delimited square
	//		}
	//	}
	//}
	//

	///*int sumofvectorvalue =0;
	//for(i=0; i< (mCodeDiversity * mXgrid*mYgrid); i++)
	//{
	//	sumofvectorvalue += featurevectorPtr[i];
	//}

	//std::cout<<sumofvectorvalue<<std::endl;
	//*/
	////int *featurevectorPtr1 = featureVector.ptr<int>(0);
	////int vct[4032];
	////int total = 0;
	////for (int a=0; a<4032; a++)
	////{
	////	//imgFinal.at<char>(noimg,col_)
	////	vct[a] = featurevectorPtr1[a];
	////	total += featurevectorPtr1[a];
	////}

	return featureVector;
}


void UniformHistogram::runFeatureVectors_(cv::Mat input,int *startPointer)
{
    
    // sequence support check
    //if(inputVector.size() > 1)
    //    throw KifasExpeption("[CLBP::runCodeGenerate] :CLBP method cannot extract features from sequence images. Check input images are sequence or not \n");
	
    //convert to grayscale
    //cv::Mat input = inputVector[0];


	// 1. determine vector dimensions
	// 2. calculate vectors
	//cv::imshow("Display window1", input);  
	//cv::waitKey(0);

	//if(mDataMappingTable == NULL)
	//	UniformHistogram::makeDataMappingTable();
	
    if(mCodeDiversity == 0)
		throw KifasExpeption("[UniformHistogram::runFeatureVectors] : the number of bins can not be calculated because code diversiy is zero!!");

	int blockXsize;
	int blockYsize;
	int lastblockXsize;
	int lastblockYsize;

	// Get input Type
	int inputType = input.type();

	// Get input Size
	int inputWidth = input.size().width;
	int inputHeight = input.size().height;

	// block sizes
	blockXsize = inputWidth /mXgrid;
	blockYsize = inputHeight /mYgrid;

	// last block size
	lastblockXsize =  inputWidth - (blockXsize * mXgrid);
	lastblockYsize =  inputHeight - (blockYsize * mYgrid);

	// set size and type of a feature vector 
	//cv::Mat featureVector = cv::Mat(1,getFeatureLength(),CV_32S );
	
	// initially set current feature vector as zero
	//featureVector.setTo(0);

	// for loop
	int i,j,k,l,m;
	
	// featurevector pointer
	int *featurevectorPtr = startPointer;// featureVector.ptr<int>(0);

	//  0-> 1-> 2-> 3-> 4->
	//  5-> 6-> 7-> 8-> 9->
	// ..........
	//int tbl[56];
	//for (int a=0; a<56; a++)
	//{
	//	tbl[a] = mDataMappingTable[a];
	//}

	/*	-->
		-->
		-->		*/
	int accpixels =0;
	if(inputType == CV_8U)
	{
		for(i=0; i< mYgrid; i++)			// loop y grid
		{
			for(j=0; j< mXgrid; j++)		// loop x grid
			{
				//variables to scan the image with squares
				int startx,starty,endx,endy;
				startx = j * blockXsize;
				starty = i * blockYsize;
				endx = startx + blockXsize;
				endy = starty + blockYsize;
				//to adjust the end, if it is the final square
				if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
					endx = endx + lastblockXsize;
				if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
					endy = endy + lastblockYsize;
				//if it is the column > 1
				int offset = (i * mXgrid + j) * mCodeDiversity;
				//to compare the pixel values of the delimited square
				for(k=starty; k< endy; k++)
				{
					for(l=startx; l< endx; l++)
					{
						uchar valuePix = input.at<uchar>(k,l);
						//search the pixel value in the binary table 
						//and increment in 1 the value
						//for(m = 0; m < mCodeDiversity; m++)
						//{
						//	if(valuePix == mDataMappingTable[m])
						//	{
						if(valuePix >=0 && valuePix < mCodeDiversity)
						{
							featurevectorPtr[offset + valuePix]++;
							accpixels++;
						}
						//		break;
						//	}
						//}
					}
				}
				//end for the loops for the delimited square
			}
		}
	}
	else if(inputType == CV_8S)
	{
		for(i=0; i< mYgrid; i++)			// loop y grid
		{
			for(j=0; j< mXgrid; j++)		// loop x grid
			{
				//variables to scan the image with squares
				int startx,starty,endx,endy;
				startx = j * blockXsize;
				starty = i * blockYsize;
				endx = startx + blockXsize;
				endy = starty + blockYsize;
				//to adjust the end, if it is the final square
				if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
					endx = endx + lastblockXsize;
				if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
					endy = endy + lastblockYsize;
				//if it is the column > 1
				int offset = (i * mXgrid + j) * mCodeDiversity;
				//to compare the pixel values of the delimited square
				for(k=starty; k< endy; k++)
				{
					for(l=startx; l< endx; l++)
					{
						char valuePix = input.at<char>(k,l);
						//search the pixel value in the binary table 
						//and increment in 1 the value
						//for(m = 0; m < mCodeDiversity; m++)
						//{
						//	if(valuePix == mDataMappingTable[m])
						//	{
						featurevectorPtr[offset + valuePix]++;
						//		break;
						//	}
						//}
					}
				}
				//end for the loops for the delimited square
			}
		}
	}
	else if(inputType == CV_16U)
	{
		for(i=0; i< mYgrid; i++)			// loop y grid
		{
			for(j=0; j< mXgrid; j++)		// loop x grid
			{
				//variables to scan the image with squares
				int startx,starty,endx,endy;
				startx = j * blockXsize;
				starty = i * blockYsize;
				endx = startx + blockXsize;
				endy = starty + blockYsize;
				//to adjust the end, if it is the final square
				if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
					endx = endx + lastblockXsize;
				if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
					endy = endy + lastblockYsize;
				//if it is the column > 1
				int offset = (i * mXgrid + j) * mCodeDiversity;
				//to compare the pixel values of the delimited square
				for(k=starty; k< endy; k++)
				{
					for(l=startx; l< endx; l++)
					{
						unsigned short valuePix = input.at<unsigned short>(k,l);
						//search the pixel value in the binary table 
						//and increment in 1 the value
						//for(m = 0; m < mCodeDiversity; m++)
						//{
						//	if(valuePix == mDataMappingTable[m])
						//	{
						featurevectorPtr[offset + valuePix]++;
						//		break;
						//	}
						//}
					}
				}
				//end for the loops for the delimited square
			}
		}
	}
	else if(inputType == CV_16S)
	{
		for(i=0; i< mYgrid; i++)			// loop y grid
		{
			for(j=0; j< mXgrid; j++)		// loop x grid
			{
				//variables to scan the image with squares
				int startx,starty,endx,endy;
				startx = j * blockXsize;
				starty = i * blockYsize;
				endx = startx + blockXsize;
				endy = starty + blockYsize;
				//to adjust the end, if it is the final square
				if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
					endx = endx + lastblockXsize;
				if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
					endy = endy + lastblockYsize;
				//if it is the column > 1
				int offset = (i * mXgrid + j) * mCodeDiversity;
				//to compare the pixel values of the delimited square
				for(k=starty; k< endy; k++)
				{
					for(l=startx; l< endx; l++)
					{
						short valuePix = input.at<short>(k,l);
						//search the pixel value in the binary table 
						//and increment in 1 the value
						//for(m = 0; m < mCodeDiversity; m++)
						//{
						//	if(valuePix == mDataMappingTable[m])
						//	{
						featurevectorPtr[offset + valuePix]++;
						//		break;
						//	}
						//}
					}
				}
				//end for the loops for the delimited square
			}
		}
	}
	else if(inputType == CV_32S)
	{
		for(i=0; i< mYgrid; i++)			// loop y grid
		{
			for(j=0; j< mXgrid; j++)		// loop x grid
			{
				//variables to scan the image with squares
				int startx,starty,endx,endy;
				startx = j * blockXsize;
				starty = i * blockYsize;
				endx = startx + blockXsize;
				endy = starty + blockYsize;
				//to adjust the end, if it is the final square
				if( mIncludeRemains && ((endx +lastblockXsize) == input.size().width))
					endx = endx + lastblockXsize;
				if( mIncludeRemains && ((endy +lastblockYsize) == input.size().height))
					endy = endy + lastblockYsize;
				//if it is the column > 1
				int offset = (i * mXgrid + j) * mCodeDiversity;
				//to compare the pixel values of the delimited square
				for(k=starty; k< endy; k++)
				{
					for(l=startx; l< endx; l++)
					{
						int valuePix = input.at<int>(k,l);
						//search the pixel value in the binary table 
						//and increment in 1 the value
						//for(m = 0; m < mCodeDiversity; m++)
						//{
						//	if(valuePix == mDataMappingTable[m])
						//	{
						if(valuePix >=0 && valuePix < mCodeDiversity)
						{
							featurevectorPtr[offset + valuePix]++;
						}

						//						featurevectorPtr[offset + valuePix]++;
						//		break;
						//	}
						//}
					}
				}
				//end for the loops for the delimited square
			}
		}
	}
	

	/*int sumofvectorvalue =0;
	for(i=0; i< (mCodeDiversity * mXgrid*mYgrid); i++)
	{
		sumofvectorvalue += featurevectorPtr[i];
	}

	std::cout<<sumofvectorvalue<<std::endl;
	*/
	//int *featurevectorPtr1 = featureVector.ptr<int>(0);
	//int vct[4032];
	//int total = 0;
	//for (int a=0; a<4032; a++)
	//{
	//	//imgFinal.at<char>(noimg,col_)
	//	vct[a] = featurevectorPtr1[a];
	//	total += featurevectorPtr1[a];
	//}

	//return featureVector;

}
/*	Create a Table (mDataMappingTable)
	only the values ??that were taken into account (ActiveBitNumber=3)
	from the total length representation of each bit (codeLength=8)
	for the creation of the code in its binary representation.
*/
/*
void UniformHistogram::makeDataMappingTable()
{
	unsigned char k = 0;
	unsigned char check_bit[8];
	int i,j;
	int check_value = 1;
	int bit_number;
	int tablecount = 0;
	mCodeDiversity=bin(mCodeLength,mActiveBitnumber); // total code diversity caculated by Combination

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
		if(bit_number == mActiveBitnumber)
		{
			mDataMappingTable[tablecount] = k;
			tablecount++;
		}
	}


}*/

/**
* Gets the feature length as the multiplication of the grid size, and the code length.
* @return the length of the feature.
*/


int UniformHistogram::getFeatureLength()
{
  return mCodeDiversity * mXgrid * mYgrid * mSequenceSize;
}