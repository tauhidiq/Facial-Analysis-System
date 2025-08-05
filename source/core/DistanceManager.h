/**
  @file DistanceManager.h
  Defines the Manager class of distances, for example ChiSquare, Intersection, ect.
*/

/**
  @defgroup distance Distance
  A distance is a similarity meassure, that gives how similar two vectors are.
*/

#ifndef ClassifierManager_H_INCLUDED
#define ClassifierManager_H_INCLUDED

// OpenCV
#include <cv.h>
#include <vector>
#include "TraceMessage.h"

#include "GenericFactory.h"
#include "Definitions.h"
#include "BaseDistance.h"


namespace kifas
{

/**
  Manages the different Distance instances. 
  It has the logic to register new distances, and how to run them.
  @ingroup core distance
*/
class DistanceManager
{

public:
	/**	 Constructor	*/
	DistanceManager();

	/**	 Destructor	*/
	~DistanceManager();


	void registerDistanceClasses();
	bool selectDistance(std::string id);		

protected:	
	cv::Ptr<BaseDistance> mSelectedDistance;

};



}

#endif