#include "LBP.h"
#include "ConfigFileManager.h"
#include "Parser.h"

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
template <typename _Tp> static
inline void elbp_(cv::Mat src, cv::Mat &_dst, int radius, int neighbors);

LBP::LBP() 
{
	mConfigFile = "LBP.conf";

	std::map<std::string, kifas::NumStr> lbpconfig;
	
	// default setting for CLDP
	//ldpconfig["directions"] = 4;
	lbpconfig["radius"] = 1;
	lbpconfig["neighbors"] = 8;
	lbpconfig["uniform"] = "true";
	lbpconfig["uvalue"] = 2;

	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, lbpconfig);
	//directions = ldpconfig["codelength"];
	//radius = ldpconfig["radius"];
	//shape = ldpconfig["shape"];

	m_radius =lbpconfig["radius"];
	m_neighbors=lbpconfig["neighbors"];
	m_uniform =lbpconfig["uvalue"];

	std::string attributevalue = lbpconfig["uniform"];
    // convert capitals to lower
	std::transform(attributevalue.begin(), attributevalue.end(), attributevalue.begin(), ::tolower);
	if(attributevalue.compare("true") == 0)
	{
		m_IsUniform = true;
	}
	else if(attributevalue.compare("false")==0)
	{
		m_IsUniform = false;
	}
	else 
	{
		m_IsUniform = true;
		TraceMessage::addMessage("In LBP.conf, uniform is set unknown value as "+attributevalue);
		TraceMessage::addMessage("true as default of uniform, will be used.");
	}

	mUniformMappingTable= NULL;
	mUniformCodeDiversity =0;

	if(m_IsUniform)
		makeUniformMappingTable();
}

/**
	Creates the appearance code of the class.
	@param input the image to generate the micro features from.
	@return an image after making the appearance code.	
*/
std::vector<cv::Mat> LBP::runCodeGenerate(const std::vector<cv::Mat> &inputVector)
{
    int i;
    std::vector<cv::Mat> returnVector;
    for(i=0; i< inputVector.size(); i++)
    {
        cv::Mat dst = runCodeGenerate_(inputVector[i]);
        returnVector.push_back(dst);
    }

    return returnVector;

 //   // sequence support check
 //   if(inputVector.size() > 1)
 //       throw KifasExpeption("[LBP::runCodeGenerate] :LBP method cannot extract features from sequence images. Check input images are sequence or not \n");
	//
 //   //convert to grayscale
 //   cv::Mat input = inputVector[0];


	////convert to grayscale
	//cv::Mat grayinput,Matdst,InputPadd;

	//if(input.channels() > 1)
	//{
	//	cv::cvtColor(input,grayinput,CV_RGB2GRAY);
	//}
	//else
	//{
	//	grayinput = input;
	//}

	//int type = grayinput.type();
 //   switch (type) {
 //   case CV_8SC1:   elbp_<char>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   case CV_8UC1:   elbp_<unsigned char>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   case CV_16SC1:  elbp_<short>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   case CV_16UC1:  elbp_<unsigned short>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   case CV_32SC1:  elbp_<int>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   case CV_32FC1:  elbp_<float>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   case CV_64FC1:  elbp_<double>(grayinput,Matdst, m_radius, m_neighbors); break;
 //   default:
 //       break;
 //   }

	//if(m_IsUniform)
	//	calculateUniformLBPCode(Matdst);

	//std::vector<cv::Mat> returnVector;
 //   returnVector.push_back(Matdst);

	//return returnVector;

	////Matdst.create(grayinput.size(),grayinput.type());
	//////////////////////////////////////////////////////////// general variables ////////////////////////////////////////////////////////
	//////to save the differents values of the neighborhood
	////unsigned char *ptrBit;
	////unsigned char bitset8[LBP::dir8];
	////unsigned char bitset16[LBP::dir16];
	////unsigned char bitset24[LBP::dir24];
	////unsigned char bitset32[LBP::dir32];
	////unsigned char bitset40[LBP::dir40];
	//////the create the binary value of the neighborhood
	////int *ptrPxl;
	////int pxlValue8[LBP::dir8];
	////int pxlValue16[LBP::dir16];
	////int pxlValue24[LBP::dir24];
	////int pxlValue32[LBP::dir32];
	////int pxlValue40[LBP::dir40];
	////
	////std::vector<int> vecY,vecX;
	////std::vector<int>::iterator itx,ity;

	////int i,j,k, //i=Y, j=X, k index for bit values in the neighborhood
	////iB,iF,jB,jF, //to define the limits of the neighborhood with the size of the radius
	////ixN, //the total size of the neighborhood
	////middleVec,contMiddle, //to calculate the middle and the next value in the vector
	////threshold, //the center pixel of the neighborhood
	////iL,jL, //the index for the padd image
	////nx,ny, //index for the limits of the neighborhood
	////valueB,valueF, //limits to create the neighborhood vertical in the circle
	////vX,vY,vX1,vY1; //index to interpolate the value

	////uchar *psrc,*pdst;		//pointers for source and destination
	////uchar *psrc_level;		//pointers for neighbors in the other levels in square shape
	////uchar *psrcNext;		//pointers for neighbors in the other levels in circle shape
	////float circEq = 0.0;
	////long ix;
	////
	////int sizeN = (radius*2)+1; //size of the neighrborhood
	////int xsize = grayinput.size().width; //the real Size X of the image
	////int ysize = grayinput.size().height; //the real Size Y of the image
	////int divRow = radius; //real value to padd the image
	////int divCol = radius; //real value to padd the image
	////int mulRow = ysize+divRow*2; //new size Y of the image padded
	////int mulCol = xsize+divCol*2; //new size X of the image padded

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////assign the pointers depending of the size of the neighborhood
	////if (LBP::directions == 8)
	////{
	////	ptrBit = bitset8;
	////	ptrPxl = pxlValue8;
	////}
	////else if (LBP::directions == 16)
	////{
	////	ptrBit = bitset16;
	////	ptrPxl = pxlValue16;
	////}
	////else if (LBP::directions == 24)
	////{
	////	ptrBit = bitset24;
	////	ptrPxl = pxlValue24;
	////}
	////else if (LBP::directions == 32)
	////{
	////	ptrBit = bitset32;
	////	ptrPxl = pxlValue32;
	////}
	////else if (LBP::directions == 40)
	////{
	////	ptrBit = bitset40;
	////	ptrPxl = pxlValue40;
	////}

	//////initialize the pointer to zero
	////for(i=0; i<LBP::directions; i++)
	////{
	////	ptrBit[i] = 0;
	////	ptrPxl[i] = 0;
	////}

	//////padd the image
	////InputPadd.create(cv::Size(mulCol,mulRow),grayinput.type());
	////for(i = 0; i < mulRow; i++)
	////{
	////	pdst = InputPadd.ptr<uchar>(i);						//destination: pointer
	////	for(j = 0; j < mulCol; j++)
	////	{
	////		if (i<divRow)
	////		{
	////			psrc = grayinput.ptr<uchar>(0);				//origin: pointer
	////			
	////			if (j<divCol)								//corner north-west
	////				pdst[j] = psrc[0];		
	////			
	////			else if (j>=divCol && j<(divCol+xsize))
	////			{
	////				jL = j - divCol;
	////				pdst[j] = psrc[jL];						//up
	////			}
	////			
	////			else if (j>=(divCol+xsize))
	////				pdst[j] = psrc[xsize];					//corner north-east
	////		}
	////		else if (i>=divRow && i<(divRow+ysize))
	////		{
	////			iL = i - divRow;
	////			psrc = grayinput.ptr<uchar>(iL);			//origin: pointer
	////			
	////			if (j<divCol)
	////				pdst[j] = psrc[0];						//left
	////			
	////			else if (j>=divCol && j<(divCol+xsize))
	////			{
	////				jL = j - divCol;
	////				pdst[j] = psrc[jL];						//center
	////			}
	////			
	////			else if (j>=(divCol+xsize))
	////				pdst[j] = psrc[xsize];					//right
	////		}
	////		else if (i>=(divRow+ysize))
	////		{
	////			psrc = grayinput.ptr<uchar>(ysize-1);		//origin: pointer
	////			
	////			if (j<divCol)
	////				pdst[j] = psrc[0];						//corner south-west
	////			
	////			else if (j>=divCol && j<(divCol+xsize))
	////			{
	////				jL = j - divCol;
	////				pdst[j] = psrc[jL];						//down
	////			}

	////			else if (j>=divCol+xsize)
	////				pdst[j] = psrc[xsize-1];				//corner south-east
	////		}
	////	}
	////}
	////
	//////scan the original image to LBP
	////for(i=radius; i<(ysize+radius); i++)
	////{
	////	////check limits for x
	////	iB = i - radius; 
	////	iF = i + radius;

	////	pdst = Matdst.ptr<uchar>(i-radius);	//destination: pointer
	////	psrc = InputPadd.ptr<uchar>(i);	//origin: pointer
	////	for (j=radius; j<(xsize+radius); j++)
	////	{
	////		////check limits for y
	////		jB = j - radius;
	////		jF = j + radius;
	////		
	////		//////////////////////////////////// get the values of the neighborhood //////////////////////////////////// 
	////		threshold = psrc[j];	//center pixel
	////		k = 0;						//index of the array with values
	////		ixN = 1;					//index of the square Neighborhood

	////		if (LBP::shape.compare("square") == 0)
	////		{
	////			for (ny=iB; ny<=iF; ny++)
	////			{
	////				psrc_level = InputPadd.ptr<uchar>(ny);
	////				for (nx=jB; nx<=jF; nx++)
	////				{
	////					if (ixN==1 || ixN==sizeN)
	////						ptrPxl[k++] = psrc_level[nx];
	////					else
	////					{
	////						ptrPxl[k++] = psrc_level[jB];
	////						ptrPxl[k++] = psrc_level[jF];
	////						break;
	////					}
	////				}
	////				ixN++;
	////			}
	////		}
	////		else if (LBP::shape.compare("circle") == 0)
	////		{
	////			// get the index of the top-bottom of the circle and arc parts
	////			ity = vecY.begin();
	////			itx = vecX.begin();
	////			for (nx=jB+1; nx<jF; nx++)
	////			{
	////				circEq = sqrt((pow((float)radius,2)-pow((float)(nx-j),2)));
	////				
	////				ity = vecY.insert(ity,floor((float)i-circEq));
	////				ity = vecY.insert(ity+1,floor((float)circEq+i));
	////				
	////				itx = vecX.insert(itx,nx);
	////				itx = vecX.insert(itx+1,nx);
	////			}
	////			
	////			// get the index of the left-right of the circle
	////			valueB = vecY.at(0)+1;
	////			valueF = vecY.at(vecY.size()-1);
	////			middleVec = 0, contMiddle = 1;
	////			for (nx=valueB; nx<valueF; nx++)
	////			{
	////				ity = vecY.begin();
	////				itx = vecX.begin();

	////				ity = vecY.insert(ity,nx);
	////				itx = vecX.insert(itx,jB);
	////				
	////				middleVec = (vecY.size()/2)+contMiddle;
	////				vecY.insert(ity+middleVec,nx);
	////				vecX.insert(itx+middleVec,jF);

	////				contMiddle++;
	////			}

	////			//to extract the information in the index and make the interpolation
	////			for (ix=0; ix<(long)vecX.size(); ++ix)
	////			{
	////				vY = vecY.at(ix);
	////				vX = vecX.at(ix);
	////				vY1 = vecY.at((ix+1)%vecY.size());
	////				vX1 = vecX.at((ix+1)%vecX.size());

	////				psrc_level = InputPadd.ptr<uchar>(vY);
	////				ptrPxl[k++] = psrc_level[vX];

	////				if (vX!=vX1 && vY!=vY1)
	////				{
	////					psrcNext = InputPadd.ptr<uchar>(vY1);
	////					ptrPxl[k++] = biLinear(psrc_level[vX],psrc_level[vX1],psrcNext[vX],psrcNext[vX1]);
	////				}
	////			}

	////			vecX.clear();
	////			vecY.clear();
	////		}
	////		
	////		--k; //initialize the index(k) to size-1

	////		// Sets the values ​​to 1 if the condition is satisfied
	////		for(;k>-1;k--)
	////		{
	////			if (ptrPxl[k]>threshold)
	////				ptrBit[k] = 1;
	////		}
	////		pdst[j-radius] = calculateLBPCode(ptrBit);
	////	}
	////}
	////return Matdst;
}

cv::Mat LBP::runCodeGenerate_(const cv::Mat input)
{
    //convert to grayscale
	cv::Mat grayinput,Matdst,InputPadd;

	if(input.channels() > 1)
	{
		cv::cvtColor(input,grayinput,CV_RGB2GRAY);
	}
	else
	{
		grayinput = input;
	}

	int type = grayinput.type();
    switch (type) {
    case CV_8SC1:   elbp_<char>(grayinput,Matdst, m_radius, m_neighbors); break;
    case CV_8UC1:   elbp_<unsigned char>(grayinput,Matdst, m_radius, m_neighbors); break;
    case CV_16SC1:  elbp_<short>(grayinput,Matdst, m_radius, m_neighbors); break;
    case CV_16UC1:  elbp_<unsigned short>(grayinput,Matdst, m_radius, m_neighbors); break;
    case CV_32SC1:  elbp_<int>(grayinput,Matdst, m_radius, m_neighbors); break;
    case CV_32FC1:  elbp_<float>(grayinput,Matdst, m_radius, m_neighbors); break;
    case CV_64FC1:  elbp_<double>(grayinput,Matdst, m_radius, m_neighbors); break;
    default:
        break;
    }

	if(m_IsUniform)
		calculateUniformLBPCode(Matdst);

    return Matdst;
}


void LBP::calculateUniformLBPCode(cv::Mat &input)
{
	if(mUniformMappingTable == NULL)
		makeUniformMappingTable();

	int ysize = input.size().height;
	int xsize = input.size().width;

	int i,j;
	for(i=0; i< ysize; i++)
	{
		int *linebuf = input.ptr<int>(i);
		for(j=0; j< xsize; j++)
		{
			linebuf[j] = mUniformMappingTable[linebuf[j]];
		}

	}

}

void LBP::makeUniformMappingTable()
{
	int codeSize = static_cast<int>(std::pow(2.0, static_cast<double>(m_neighbors)));
	mUniformMappingTable = new int [codeSize];
	memset(mUniformMappingTable,0,codeSize *sizeof(int));
	
	int i,j;
	int uniformCodeSize = 0;
	for(i=0; i< codeSize; i++)
	{
		int shift_save=1;
		shift_save =  i & shift_save;
		shift_save = shift_save <<(m_neighbors-1);
		int shift_value = i >> 1;
		shift_value = shift_value | shift_save;
		int transitionBits = i ^ shift_value;

		int transitionCount = 0;
		for(j=0; j< m_neighbors; j++)
		{
			int check = transitionBits & 1;
			transitionBits = transitionBits >> 1;
			if(check == 1)
				transitionCount++;
		}

		// uniform
		if(transitionCount <= m_uniform)
		{
			mUniformMappingTable[i] = uniformCodeSize;
			uniformCodeSize++;
		}
		else
		{
			mUniformMappingTable[i] = -1;
		}
	}

	for(i=0; i< codeSize; i++)
	{
		if(mUniformMappingTable[i] == -1)
			mUniformMappingTable[i]= uniformCodeSize;
	}

	mUniformCodeDiversity = uniformCodeSize +1;

}

//
//unsigned char LBP::biLinear(int UL, int UR, int DL, int DR)
//{
//	//calculate the value by bilinear interpolation
//	unsigned char r=0;
//	double factor = 0.5*0.5;
//	r = factor * (UL + UR + DL + DR);
//	return r;
//}

/**
	Calculate the size of the appearance code of the class
	@return the size of the code
*/
int LBP::getCodeSize()
{
	if(!m_IsUniform)
		return static_cast<int>(std::pow(2.0, static_cast<double>(m_neighbors)));
	else 
		return mUniformCodeDiversity;
}

template <typename _Tp> static
inline void elbp_(cv::Mat src, cv::Mat &dst, int radius, int neighbors) {
    // allocate memory for result
    dst.create(src.rows-2*radius, src.cols-2*radius, CV_32SC1);
    //Mat dst = _dst;//.getMat();
    // zero
    dst.setTo(0);
    for(int n=0; n<neighbors; n++) {
        // sample points
        float x = static_cast<float>(-radius * sin(2.0*CV_PI*n/static_cast<float>(neighbors)));
        float y = static_cast<float>(radius * cos(2.0*CV_PI*n/static_cast<float>(neighbors)));
        // relative indices
        int fx = static_cast<int>(floor(x));
        int fy = static_cast<int>(floor(y));
        int cx = static_cast<int>(ceil(x));
        int cy = static_cast<int>(ceil(y));
        // fractional part
        float ty = y - fy;
        float tx = x - fx;
        // set interpolation weights
        float w1 = (1 - tx) * (1 - ty);
        float w2 =      tx  * (1 - ty);
        float w3 = (1 - tx) *      ty;
        float w4 =      tx  *      ty;
		
        // iterate through your data
        for(int i=radius; i < src.rows-radius;i++) {
            for(int j=radius;j < src.cols-radius;j++) {
                // calculate interpolated value
                float t = static_cast<float>(w1*src.at<_Tp>(i+fy,j+fx) + w2*src.at<_Tp>(i+fy,j+cx) + w3*src.at<_Tp>(i+cy,j+fx) + w4*src.at<_Tp>(i+cy,j+cx));
                // floating point precision, so check some machine-dependent epsilon
                dst.at<int>(i-radius,j-radius) += ((t > src.at<_Tp>(i,j)) || (std::abs(t-src.at<_Tp>(i,j)) < std::numeric_limits<float>::epsilon())) << n;
            }
        }
    }
}