#include "KNN.h"
#include "ChiSquare.h"

KNN::KNN()
{
	mK = 1;
}

KNN::~KNN()
{

}

void KNN::runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData,std::vector<int> &testResultTable)
{
	// Nearest Neighbor Distance
	double *NearestNeighborDistance;
	int   *NearestNeighborID;

	// mK is K value which we found 
	NearestNeighborDistance = new double[mK];
	NearestNeighborID = new int[mK];

	int TrainDataType = trainningData.type();

	// int형의 벡터라고 가정하고 함 짜보자
	unsigned int i,j,k;
	int WellClasifiedNumber = 0;

	unsigned int NumTest = testData.size().height;
	unsigned int NumTrain= trainningData.size().height;
	unsigned int Dimensions = testData.size().width;

	// data conversion from unknown input data type to float
	cv::Mat traingDataFloat;
	cv::Mat testDataFloat;
	trainningData.convertTo(traingDataFloat,CV_32F);
	testData.convertTo(testDataFloat,CV_32F);

/*	for(i=0; i< mK; i++)
	{
		NearestNeighborDistance[i] = 10000000000000000000.0;
		NearestNeighborID[i] = 0;
	}
	*/
	kifas::BaseDistance *distanceCalculator= new ChiSquare();

	for(i=0; i< NumTest; i++)
	{
		// get a test data
		float *TempTestData = testDataFloat.ptr<float>(i);
		cv::Mat TempTestMat = testDataFloat(cv::Rect(0,i,testDataFloat.cols,1));
		//int truthvalue = testTruthTable[i];
		
		for(j=0; j< mK; j++)
		{
			NearestNeighborDistance[j] = 10000000000000000000.0;
			NearestNeighborID[j] = 0;
		}

		for(j=0; j< NumTrain; j++)
		{
			
			float *TempTrainData = traingDataFloat.ptr<float>(j);
			cv::Mat TempTrainMat = traingDataFloat(cv::Rect(0,j,traingDataFloat.cols,1));
			double distance = 0.0f;

			// calculate distance
			distance = distanceCalculator->getDistance(TempTrainMat,TempTestMat);

			/*for(k=0; k< Dimensions; k++)
			{
				distance += (double)(abs( TempTrainData[k] -TempTestData[k]));
			}*/

			// if current distance ranks within K number, add it 
			if(distance < NearestNeighborDistance[mK-1])
			{
				// find add position
				for(k=0; k< mK; k++)
				{
					if(NearestNeighborDistance[k] > distance)
						break;
				}

				int addingpos = k;
				
				for(k=(mK-1); k> addingpos; k--)
				{
					NearestNeighborDistance[k] = NearestNeighborDistance[k-1]; 
					NearestNeighborID[k] = NearestNeighborID[k-1];
				}
				NearestNeighborDistance[addingpos] = distance;
				NearestNeighborID[addingpos] = trainningTruthTable[j];
			}
		}

		std::map<int,int> knnmap;
		std::map<int,int>::iterator it;

		// find classification result
		for(k=0; k< mK; k++)
		{
			it = knnmap.find(NearestNeighborID[k]);

			// if attribut string is not found in configmap
			if(it == knnmap.end())
				knnmap.insert(std::pair<int,int>(NearestNeighborID[k],1));
			else
				(it->second)++;
		}

		it=knnmap.begin();
		int classifiedID = it->first;
		int largestValue = it->second;
		for(; it != knnmap.end(); it++)// knnmapsize; k++)
		{
			if(largestValue < it->second)
			{
				classifiedID = it->first;
				largestValue = it->second;
			}
			
		}

        testResultTable[i] = classifiedID;
		//if( truthvalue == classifiedID)
		//	WellClasifiedNumber++;

	}

    delete[] NearestNeighborDistance;
    delete[] NearestNeighborID;
    delete distanceCalculator;

	//return (double)WellClasifiedNumber / (double)NumTest;
}

float KNN::runClassification(cv::Mat trainningData,std::vector<int> &trainningTruthTable,cv::Mat testData){

    // Nearest Neighbor Distance
	double *NearestNeighborDistance;
	int   *NearestNeighborID;

	// mK is K value which we found 
	NearestNeighborDistance = new double[mK];
	NearestNeighborID = new int[mK];

	int TrainDataType = trainningData.type();

	// int형의 벡터라고 가정하고 함 짜보자
	unsigned int i,j,k;
	//int WellClasifiedNumber = 0;

	unsigned int NumTest = testData.size().height;
	unsigned int NumTrain= trainningData.size().height;
	unsigned int Dimensions = testData.size().width;

	// data conversion from unknown input data type to float
	cv::Mat traingDataFloat;
	cv::Mat testDataFloat;
	trainningData.convertTo(traingDataFloat,CV_32F);
	testData.convertTo(testDataFloat,CV_32F);

/*	for(i=0; i< mK; i++)
	{
		NearestNeighborDistance[i] = 10000000000000000000.0;
		NearestNeighborID[i] = 0;
	}
	*/
	kifas::BaseDistance *distanceCalculator= new ChiSquare();

	//for(i=0; i< NumTest; i++)
	//{
		// get a test data
		float *TempTestData = testDataFloat.ptr<float>(0);
		cv::Mat TempTestMat = testDataFloat(cv::Rect(0,0,testDataFloat.cols,1));
		//int truthvalue = testTruthTable[i];
		
		for(j=0; j< mK; j++)
		{
			NearestNeighborDistance[j] = 10000000000000000000.0;
			NearestNeighborID[j] = 0;
		}

		for(j=0; j< NumTrain; j++)
		{
			
			float *TempTrainData = traingDataFloat.ptr<float>(j);
			cv::Mat TempTrainMat = traingDataFloat(cv::Rect(0,j,traingDataFloat.cols,1));
			double distance = 0.0f;

			// calculate distance
			distance = distanceCalculator->getDistance(TempTrainMat,TempTestMat);

			/*for(k=0; k< Dimensions; k++)
			{
				distance += (double)(abs( TempTrainData[k] -TempTestData[k]));
			}*/

			// if current distance ranks within K number, add it 
			if(distance < NearestNeighborDistance[mK-1])
			{
				// find add position
				for(k=0; k< mK; k++)
				{
					if(NearestNeighborDistance[k] > distance)
						break;
				}

				int addingpos = k;
				
				for(k=(mK-1); k> addingpos; k--)
				{
					NearestNeighborDistance[k] = NearestNeighborDistance[k-1]; 
					NearestNeighborID[k] = NearestNeighborID[k-1];
				}
				NearestNeighborDistance[addingpos] = distance;
				NearestNeighborID[addingpos] = trainningTruthTable[j];
			}
		}

		std::map<int,int> knnmap;
		std::map<int,int>::iterator it;

		// find classification result
		for(k=0; k< mK; k++)
		{
			it = knnmap.find(NearestNeighborID[k]);

			// if attribut string is not found in configmap
			if(it == knnmap.end())
				knnmap.insert(std::pair<int,int>(NearestNeighborID[k],1));
			else
				(it->second)++;
		}

		it=knnmap.begin();
		int classifiedID = it->first;
		int largestValue = it->second;
		for(; it != knnmap.end(); it++)// knnmapsize; k++)
		{
			if(largestValue < it->second)
			{
				classifiedID = it->first;
				largestValue = it->second;
			}
			
		}

		//if( truthvalue == classifiedID)
		//	WellClasifiedNumber++;

	//}

    delete[] NearestNeighborDistance;
    delete[] NearestNeighborID;
    delete distanceCalculator;

	return (float )classifiedID;
}