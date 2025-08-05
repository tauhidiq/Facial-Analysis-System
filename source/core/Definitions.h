/**
  @file Definitions.h
  Contains several global definitions that are used in several classes.
*/

/**
  @defgroup definitions "Constants and Definitions"
  @ingroup core
  Definitions and constants used in the system.
*/

#ifndef Definitions_H_INCLUDED
#define Definitions_H_INCLUDED

#include <string>

#include "GenericFactory.h"

#include "BasePreprocessing.h"
#include "BaseAppearanceCode.h"
#include "BaseFeatureRepresentation.h"
#include "BaseClassifier.h"
#include "BaseDistance.h"

namespace kifas
{

/** 
  Definition of our featuve vector type
  @ingroup definitions
*/
#define FV_TYPE CV_32S

/** 
  Length used for arrays of characters 
  @ingroup definitions
*/
const int STRLEN = 1024; 

/**
  The directory in which all the configuration files are.
  @ingroup definitions
*/
const std::string CONFIG_DIR("conf");




/**
  Factory definitions
*/
/* @{ */
typedef GenericFactory<BasePreprocessing> PreprocessingFactory;
typedef GenericFactory<BaseAppearanceCode> AppearanceCodeFactory;
typedef GenericFactory<BaseFeatureRepresentation> FeatureRepresentationFactory;
typedef GenericFactory<BaseClassifier> ClassifierFactory;
typedef GenericFactory<BaseDistance> DistanceFactory;
/* @} */
}

#endif