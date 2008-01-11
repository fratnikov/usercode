#ifndef HPDLibrary_HPDNoiseData_h
#define HPDLibrary_HPDNoiseData_h

#include "TObject.h"

#include <iostream>
#include <stdint.h>
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "HcalExamples/HPDLibrary/interface/HPDNoiseDataFrame.h"


class HPDNoiseData {
 public:
  HPDNoiseData () {}
  virtual ~HPDNoiseData ();

  unsigned size () const {return mData.size();}
  void addChannel (HcalDetId fId, const float* fCharges);
  std::vector<HcalDetId> getAllDetIds () const;
  const HPDNoiseDataFrame& getDataFrame (unsigned i) const;
 private:
  std::vector<HPDNoiseDataFrame> mData;

  ClassDef(HPDNoiseData,1)
};

std::ostream& operator<< (std::ostream&, const HPDNoiseData&);

#endif
