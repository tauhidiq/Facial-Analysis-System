#include "GaussianSmoothing.h"
#include <highgui.h>

void GaussianSmoothing::RunPreprocessing(std::vector<cv::Mat> &input)
{
	int i;
    cv::Mat tempMat;
    for(i=0; i< input.size(); i++)
    {
        cv::GaussianBlur(input[i],tempMat,cv::Size(3,3),0,0);
        input[i] = tempMat;
    }
}