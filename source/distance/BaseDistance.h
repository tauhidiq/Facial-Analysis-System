#ifndef BASE_DISTANCE_H__
#define BASE_DISTANCE_H__

// OpenCV
#include <cv.h>


/**
  @defgroup distance "Distance Function"
  A distance function computes the similarity of two feature vectors by comparing how close they are.  
*/

namespace kifas
{

/**
  The Abstract Distance class, that computes the distance of two feature vectors.
  @ingroup distance
*/
class BaseDistance {
public:

  BaseDistance() {}
	
	/**
	  Computes the distance from the vectors.
    @param vector1 is one vector to compute the distance.
    @param vector2 is the other vector to compute the distance.
    @return the computed distance from vector1 and vector2. In case of error it returns a negative number.
    @warning The vectors should be row vectors. That is, the row should be one, and have N different columns.
	*/
	virtual double getDistance(const cv::Mat vector1, const cv::Mat vector2) = 0;

};

}

#endif /* BASE_DISTANCE_H__ */