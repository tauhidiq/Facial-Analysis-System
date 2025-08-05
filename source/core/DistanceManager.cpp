#include "DistanceManager.h"

#include "ChiSquare.h"

/** Constructor */
kifas::DistanceManager::DistanceManager()
{
	registerDistanceClasses();
}

/** Destructor */
kifas::DistanceManager::~DistanceManager()
{
}

/**
  Register all classifier instances to the GenericFactory 
*/
void kifas::DistanceManager::registerDistanceClasses()
{
	RegisterInFactory<BaseDistance, ChiSquare> regChiSquare("ChiSquare");	
}