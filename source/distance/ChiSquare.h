#ifndef CHI_SQUARE_H
#define CHI_SQUARE_H

#include "BaseDistance.h"

/**
 The Preprocessing Abstract class. It defines how to get frames.
 @ingroup core
*/
class ChiSquare : public kifas::BaseDistance {
public:

  ChiSquare();
  ~ChiSquare();

  double getDistance(const cv::Mat vector1, const cv::Mat vector2);

  template <typename _Tp> double getDistance_(const cv::Mat& vector1, const cv::Mat& vector2);

};

#endif /* CHI_SQUARE_H */