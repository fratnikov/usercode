#include "HcalExamples/HPDLibrary/interface/HPDNoiseDataFrame.h"

HPDNoiseDataFrame::HPDNoiseDataFrame (HcalDetId fId, const float* fCharges) 
  : mId (fId)
{
  for (size_t i = 0; i < FRAMESIZE; ++i) mCharge[i] = fCharges[i];
}

HPDNoiseDataFrame::~HPDNoiseDataFrame () {}

std::ostream& operator<< (std::ostream& fStream, const HPDNoiseDataFrame& fFrame) {
  fStream << fFrame.id();
  for (size_t i = 0; i < FRAMESIZE; ++i) fStream << ' ' << i << ':' << fFrame.charge (i);
  return fStream;
}
