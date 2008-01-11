#ifndef HPDLibrary_HPDNoiseDataFrame_h
#define HPDLibrary_HPDNoiseDataFrame_h

#include "TObject.h"

#include <iostream>
#include <stdint.h>
#include "DataFormats/HcalDetId/interface/HcalDetId.h"


namespace {
  const unsigned int FRAMESIZE = 10;
}

//class HPDNoiseDataFrame : public TObject {
class HPDNoiseDataFrame {
 public:
  HPDNoiseDataFrame () {};
  HPDNoiseDataFrame (HcalDetId fId, const float* fCharges);
  virtual ~HPDNoiseDataFrame ();
  HcalDetId id () const {return HcalDetId (mId);}
  float charge (unsigned i) const {return (i < FRAMESIZE) ? mCharge[i] : -1.;}
  const float* getFrame () const {return mCharge;}
 private:
  uint32_t mId;
  float mCharge [10];
  ClassDef(HPDNoiseDataFrame,1)
};

std::ostream& operator<< (std::ostream&, const HPDNoiseDataFrame&);
    
#endif
