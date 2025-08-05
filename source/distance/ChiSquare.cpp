#include "ChiSquare.h"
#include "Definitions.h"

ChiSquare::ChiSquare()
{
}

ChiSquare::~ChiSquare()
{
}

/**
	Computes the distance from the vectors, as the square of the difference of the vectors over its sum @f$ \Chi^2 = \frac{( V_1 - V_2 )^2}{V_1 + V_2} @f$
  @param vector1 is one vector to compute the distance.
  @param vector2 is the other vector to compute the distance.
  @return the computed distance from vector1 and vector2. In case of error it returns a negative number.
  @warning The vectors should be row vectors. That is, the row should be one, and have N different columns.
*/
double ChiSquare::getDistance(const cv::Mat vector1, const cv::Mat vector2)
{
  if(vector1.cols != vector2.cols)
    return -1;

  if(vector1.type() != vector2.type())
	return -1;

  double sim = 0;  
  
  switch(vector1.type()){
	case CV_8SC1: sim = getDistance_<char>(vector1, vector2); break;
	case CV_8UC1: sim = getDistance_<unsigned char>(vector1, vector2); break;
	case CV_16SC1: sim = getDistance_<short>(vector1, vector2); break;
	case CV_16UC1: sim = getDistance_<unsigned short>(vector1, vector2); break;
	case CV_32SC1: sim = getDistance_<int>(vector1, vector2); break;
	case CV_32FC1: sim = getDistance_<float>(vector1, vector2); break;
	case CV_64FC1: sim = getDistance_<double>(vector1, vector2); break;
  }
 /* for(int i=0; i < vector1.cols; i++)
  {
    double diff = vector1.at<double>(1,i) - vector2.at<double>(1,i);
    double sum = vector1.at<double>(1,i) + vector2.at<double>(1,i);
    if (sum == 0)
      sum = 1;
    sim += diff*diff/sum;
  }
  return sim;*/
  return sim;
}

template <typename _Tp> double ChiSquare::getDistance_(const cv::Mat& vector1, const cv::Mat& vector2)
{
	double sim = 0;

	int i;
	for(i=0; i< vector1.cols; i++)
	{
		double diff = vector1.at<_Tp>(0,i) - vector2.at<_Tp>(0,i);
		double sum  = vector1.at<_Tp>(0,i) + vector2.at<_Tp>(0,i);
		if(sum == 0)
			sum = 1.0;
		sim += (diff*diff)/sum;
	}

	return sim;
}