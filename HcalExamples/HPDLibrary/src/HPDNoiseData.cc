#include "HcalExamples/HPDLibrary/interface/HPDNoiseData.h"

HPDNoiseData::~HPDNoiseData () {}

void HPDNoiseData::addChannel (HcalDetId fId, const float* fCharges) {
  mData.push_back (HPDNoiseDataFrame (fId, fCharges));
}

std::vector<HcalDetId> HPDNoiseData::getAllDetIds () const {
  std::vector<HcalDetId> result;
  for (int i = 0; i < mData.size(); ++i) result.push_back (getDataFrame(i).id());
  return result;
}

const HPDNoiseDataFrame& HPDNoiseData::getDataFrame (unsigned i) const {
  return mData[i];
}

std::ostream& operator<< (std::ostream& fStream, const HPDNoiseData& fData) {
  for (size_t i = 0; i < fData.size (); ++i) fStream << fData.getDataFrame (i) << std::endl;
  return fStream;
}

