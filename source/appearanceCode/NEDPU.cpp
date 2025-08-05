#include "NEDPU.h"
#include "ConfigFileManager.h"
#include "Parser.h"
#include <stdlib.h>     /* abs */
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#define PI 3.14159265
using namespace cv;

NEDPU::NEDPU()
{
	mConfigFile = "NEDPU.conf";
	std::map<std::string, kifas::NumStr> nedpuconfig;
	kifas::ConfigFileManager::ConfigFileRead(mConfigFile, nedpuconfig);

	edgepercent = nedpuconfig["edgepercent"];
	scorethresh = nedpuconfig["scorethresh"];
	sigma = nedpuconfig["sigmaw"];
	ignorecode = nedpuconfig["ignorecode"];
	padding = nedpuconfig["padding"];
}

std::vector<cv::Mat> NEDPU::runCodeGenerate(const std::vector<cv::Mat> &input)
{

	cv::Mat sourceImage, MAGNITUDE, PHASE;
	cv::Mat dstCode, score;

	if (input[0].channels() >= 3)
		cv::cvtColor(input, sourceImage, CV_RGB2GRAY);
	else
		sourceImage = input[0];


	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;


	/// Gradient X
	Sobel(sourceImage, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	Sobel(sourceImage, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	///// Total Gradient (approximate)
	/*MAGNITUDE.convertTo(MAGNITUDE, CV_8U);
	addWeighted(abs_grad_x, 1.0f, abs_grad_y, 1.0f, 0, MAGNITUDE);*/

	MAGNITUDE = cv::Mat::zeros(grad_x.rows, grad_x.cols, CV_8U);
	int mTmp;
	for (int i = 0; i < grad_x.rows; i++)
	{
		for (int j = 0; j < grad_x.cols; j++)
		{
			mTmp = hypot(abs_grad_x.at<uchar>(i, j), abs_grad_y.at<uchar>(i, j));
			if (mTmp > 255.f) mTmp = 255;
			MAGNITUDE.at<uchar>(i, j) = mTmp;
		}
	}

	////to store the gradients
	PHASE = Mat::zeros(MAGNITUDE.rows, MAGNITUDE.cols, CV_32F);
	grad_x.convertTo(grad_x, CV_32F);
	grad_y.convertTo(grad_y, CV_32F);

	phase(grad_x, grad_y, PHASE, true);

	for (int i = 0; i < grad_x.rows; i++)
	{
		for (int j = 0; j < grad_x.cols; j++)
		{
			if (PHASE.at<float>(i, j) != 0.0f)
				PHASE.at<float>(i, j) = 360.0f - PHASE.at<float>(i, j);
		}
	}

	//ADAPTIVE THRESHOLD
	magThresh = ComputeMagThreshold(MAGNITUDE, edgepercent);



	//general variables
	int i = 0, j = 0, k = 0;
	int temp;


	//Gaussian Weight definition
	double *ANGLEIDEAL, *SimAGNLE, *SimMag;
	ANGLEIDEAL = gaussianTable(90);
	//SimAGNLE = gaussianTable(360); //<5X5>
	//SimMag = gaussianTable(255); //<5X5>
	//float nnPW[3] = { 1,0.8,0.8 };

	int xIndx[8] = { 1, 1,  0,   -1, -1, -1,  0, 1 };
	int yIndx[8] = { 0, -1, -1,  -1,  0,  1,  1, 1 };

	int nAng, nMag, cMag, cAng;
	struct { float val; int indx; } max1, max2, max3, tmp, firstS;
	float tVal, supp_tval = 0, mgSWi = 0;

	int indxC, indxL, indxR, gdCP, gdNP, cpX, cpY;
	float  mgCP, mgNP, tmpScore;
	int dif, gd1, gd2;
	unsigned char tcode, sign1, sign2;



	//create output Image
	dstCode = Mat::zeros(MAGNITUDE.size().height - padding * 2, MAGNITUDE.size().width - padding * 2, MAGNITUDE.type());
	int dx = dstCode.size().width;
	int dy = dstCode.size().height;


	int p, q;
	for (p = 0; p < dy; p++)
		for (q = 0; q < dx; q++)
		{

			i = p + padding; j = q + padding;


			if ((cMag = MAGNITUDE.at<uchar>(cvPoint(j, i)))>magThresh)  //First checking current pixel>magthreshold or not
			{
				cAng = PHASE.at<float>(cvPoint(j, i));             //current angel, becuase to consider similarity angle

				max1.val = max2.val = max3.val = 1.0; //threshLow should be bigger than 1
				for (k = 0; k<8; k++)
				{

					if ((nMag = MAGNITUDE.at<uchar>(cvPoint(j + xIndx[k], i + yIndx[k])))>magThresh) // Again checking neighboring pixel>magthreshold or not
					{

						nAng = PHASE.at<float>(cvPoint(j + xIndx[k], i + yIndx[k]));   // Neighboring pixel angle

						int ID_angDiff = angleDiff(k, nAng);    float idANgleD = ANGLEIDEAL[ID_angDiff];
						//int magdiff = abs(cMag - nMag);       float MDiff = SimAGNLE[magdiff];   // <5X5>
						//int angldiff = abs(cAng - nAng);      float AnDiff = SimAGNLE[angldiff]; // <5X5>


						tVal = nMag *  idANgleD;     //* Magnitude * Angle Weight					
													 //supp_tval = MDiff *  AnDiff;    // Magnitude similarity*Angle similarity*CAng similarity //<5X5>
													 //tVal = tVal * ( 1 + supp_tval);//Final tVal <Score>  //<5X5>




													 //////Put Bigger window score here......
													 ///////////////////////////////////////////////////////////////////////  
													 ////// get the score for the each direction
													 //indxC = k;
													 //cpX = j + xIndx[indxC]; cpY = i + yIndx[indxC];	// CNP- gradient magnitude
													 //gdCP = PHASE.at<float>(cvPoint(cpX, cpY)); // CNP- gradient angel
													 //
													 ////C-NNP: check the next neighbor pixel(NNP) in the current nighbor pixel(CNP) direction "indxC"
													 //gdNP = PHASE.at<float>(cvPoint(cpX + xIndx[indxC], cpY + yIndx[indxC])); // gradient angle(GA) of the NNP in the CNP direction
													 //mgNP = MAGNITUDE.at<uchar>(cvPoint(cpX + xIndx[indxC], cpY + yIndx[indxC])); //NNP- gradient magnitude
													 //// if NNP has the gradient direction similar to the CNP, then assign higher score  
													 //firstS.val = SimAGNLE[abs(gdCP - gdNP)] * nnPW[0] * mgNP;    // Similarity Weight * Position Weight * Magnitude 
													 //firstS.indx = gdNP;
													 //

													 ////R-NNP: check the NNP on the left of CNP direction(indxC-1)
													 //indxL = indxC - 1; 	if (indxL == -1) indxL = 7;
													 //gdNP = PHASE.at<float>(cvPoint(cpX + xIndx[indxL], cpY + yIndx[indxL])); // GA of NNP on the left of CNP direction
													 //mgNP = MAGNITUDE.at<uchar>(cvPoint(cpX + xIndx[indxL], cpY + yIndx[indxL]));
													 //// if NNP has GA similar to the CNP, then assign higher score      
													 //tmpScore = SimAGNLE[abs(gdCP - gdNP)] * nnPW[1] * mgNP;
													 //if (tmpScore>firstS.val) { firstS.val = tmpScore; firstS.indx = gdNP; }
													 //


													 ////L-NNP: check the NNP on the right of CNP direction(indxC+1)
													 //indxR = indxC + 1;		if (indxR == 8) indxR = 0;
													 //gdNP = PHASE.at<float>(cvPoint(cpX + xIndx[indxR], cpY + yIndx[indxR])); // GA of NNP on the right of CNP direction
													 //mgNP = MAGNITUDE.at<uchar>(cvPoint(cpX + xIndx[indxR], cpY + yIndx[indxR]));
													 //// if NNP has GA similar to the CNP, then assign higher score  
													 //tmpScore = SimAGNLE[abs(gdCP - gdNP)] * nnPW[1] * mgNP;
													 //if (tmpScore>firstS.val) { firstS.val = tmpScore; firstS.indx = gdNP; }
													 //
													 ////....................................
													 //tVal = tVal + firstS.val;  // Final Score
													 //////.............................................................End bigger window




													 // update max
						if (tVal>scorethresh && tVal>max3.val)  // score should be bigger than scoreThresh	
						{
							max3.val = tVal;  max3.indx = k;
							if (max3.val>max2.val)
							{
								tmp = max2; max2 = max3; max3 = tmp;
								if (max2.val>max1.val) { tmp = max1; max1 = max2; max2 = tmp; }
							}
						}

					}
				}


				// Remove the adjacent MAX scores
				if (abs(max1.indx - max2.indx) == 1 || abs(max1.indx - max2.indx) == 7)
				{
					// max1
					max1 = (max1.val >= max2.val) ? max1 : max2;
					// max2
					if (max3.val > scorethresh)
						max2 = max3;
					else  max2.val = 1;
				}



				// Flat case:     max1.val=1 && max2.val=1  // because, the initialization was same and no change
				// Texture case : max1.val>1 && max2.val>1
				// End of Line :  max1.val>1 && max2.val=1

				// assign code
				if (max1.val == 1) { dstCode.at<uchar>(cvPoint(q, p)) = ignorecode; }	//flat case		

				else
				{

					if (max1.val>1 && max2.val>1) // not considering the end of line //[In this implementation, end of line and flat edge case generate-> same code 0]
					{
						gd1 = PHASE.at<float>(cvPoint(j + xIndx[max1.indx], i + yIndx[max1.indx]));
						gd2 = PHASE.at<float>(cvPoint(j + xIndx[max2.indx], i + yIndx[max2.indx]));


						sign1 = calcSIGN(max1.indx, gd1);
						sign2 = calcSIGN(max2.indx, gd2);



						// Sign with Primary + No sign with secondary , Because, maximum case, primary direction sign is almost same with secondary direction sign.
						if (max1.indx < max2.indx)\
							//unsigned
							dstCode.at<uchar>(cvPoint(q, p)) = max1.indx* 8 + max2.indx;
							
							//signed
							//dstCode.at<uchar>(cvPoint(q, p)) = (max1.indx + sign1) * 8 + max2.indx;
						else
							//unsigned
							dstCode.at<uchar>(cvPoint(q, p)) = max2.indx * 8+  max1.indx;

							//signed
							//dstCode.at<uchar>(cvPoint(q, p)) = max2.indx * 16 + (sign1 + max1.indx);

					} // End of IF: <Not ENDOFLINE IF>


					else if (max1.val>1 && max2.val == 1) // ENDOFLINE
					{

						gd1 = PHASE.at<float>(cvPoint(j + xIndx[max1.indx], i + yIndx[max1.indx]));
						sign1 = calcSIGN(max1.indx, gd1);

						//unsi9gned
						dstCode.at<uchar>(cvPoint(q, p)) = max1.indx  * 8 + max1.indx;

						//signed
						//dstCode.at<uchar>(cvPoint(q, p)) = (max1.indx + sign1) * 8 + max1.indx;
					} // End of IF: <ENDOFLINE IF>



					else { dstCode.at<uchar>(cvPoint(q, p)) = ignorecode; }   // 
				}
			}
			else { dstCode.at<uchar>(cvPoint(q, p)) = ignorecode; }
		}

	delete ANGLEIDEAL, SimAGNLE, SimMag;
	//cv::imwrite ("C:\\Users\\Pran Tauhid Iqbal\\Google Drive\\Matlab\\uneven_illumination\\Manual\\NEDP\\100.png",dstCode);
	//return dstCode;

	cv::imwrite("D://Research//1.PAPER//NEDP_TAC//NEDPU_uneven//file.png", dstCode);

	std::vector<cv::Mat> returnVector;
	returnVector.push_back(dstCode);
	return returnVector;
}



int NEDPU::getCodeSize()
{
	return 64;
	//return 128; //because, highest val= 1111_111=127 (0 to 127=128)
}



double* NEDPU::gaussianTable(int radius) {

	int kernalSize = 2 * radius + 1;
	double c = 2.0 * sigma * sigma;

	double* kernel = NULL;
	kernel = new double[kernalSize];
	double x;

	for (int i = 0; i < kernalSize; ++i) {
		x = i - kernalSize / 2;
		kernel[i] = exp(-x * x / c) / sqrt(c * 3.1416);
	}


	//Generating weight
	double* weightTable = NULL; 	weightTable = new double[radius + 1];
	int w = 1; weightTable[0] = 1.0;
	double ScaleFactor = 0.95 / kernel[radius];

	for (int kp = radius; kp >= 1; kp--) {
		weightTable[w] = kernel[kp] * ScaleFactor;
		//cout << "\n" << w << "= " << weightTable[w];
		w++;
	}

	delete[] kernel;
	kernel = NULL;
	return weightTable;
}


//Calcualte difference
int NEDPU::angleDiff(int position, float angle) {

	float diff1 = 0, diff2 = 0;

	//0,4
	if (position == 0 || position == 4) {

		diff1 = abs(90 - angle);
		diff2 = abs(270 - angle);
	}


	//2,6
	else if (position == 2 || position == 6) {

		if (angle>270 && angle <= 360)   diff1 = abs(360 - angle);
		else diff1 = abs(0 - angle);

		diff2 = abs(180 - angle);
	}


	//5,1
	else if (position == 5 || position == 1) {

		if (angle >= 0 && angle <45)   diff1 = angle + 45;
		else diff1 = abs(315 - angle);

		diff2 = abs(135 - angle);
	}

	//3,7
	else if (position == 3 || position == 7) {

		if (angle >= 315 && angle <360)   diff1 = (360 - angle) + 45;
		else diff1 = abs(45 - angle);

		diff2 = abs(225 - angle);
	}

	//return 
	int DIF;
	if (diff1 <= diff2) 	DIF = diff1;
	else DIF = diff2;

	return DIF;
}



int NEDPU::calcSIGN(int position, float angle) {

	int sign = 0;


	//0,4
	if (position == 0 || position == 4) {

		if (angle >= 0 && angle <= 180)
			sign = 0;
		else
			sign = 8;
	}

	//2,6
	else if (position == 2 || position == 6) {

		if (angle >= 90 && angle <= 270)
			sign = 8;
		else
			sign = 0;
	}

	//1,5
	else if (position == 1 || position == 5) {

		if (angle >= 45 && angle <= 225)
			sign = 0;
		else
			sign = 8;
	}

	//3,7
	else if (position == 3 || position == 7) {

		if (angle >= 135 && angle <= 315)
			sign = 8;
		else
			sign = 0;
	}


	return sign;
}


int NEDPU::ComputeMagThreshold(cv::Mat gradMag, float mT1Percent)
{
	int height = gradMag.size().height;
	int width = gradMag.size().width;
	int totalPixels = (height - (padding * 2)) * (width - (padding * 2));
	int mag_bin = 0, tmp;
	float sum = 0;


	//// Generate histogram of magnitudes, to see the distribution
	int hist_mag[256];
	std::fill(hist_mag, hist_mag + 256, 0);


	for (int i = padding; i < (height - padding); i++)
	{
		for (int j = padding; j < (width - padding); j++)
		{
			tmp = gradMag.at<uchar>(i, j);
			hist_mag[tmp]++;
		}
	}

	for (int h = (256 - 1); h >= 0; h--)
	{
		sum = sum + hist_mag[h];
		if ((sum / totalPixels) > mT1Percent)
		{
			mag_bin = h;
			break;
		}
	}

	return mag_bin;
}






