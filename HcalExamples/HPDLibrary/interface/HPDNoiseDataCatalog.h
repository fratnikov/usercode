#ifndef HPDLibrary_HPDNoiseDataCatalog_h
#define HPDLibrary_HPDNoiseDataCatalog_h

#include <iostream>
#include <vector>
#include <string>

#include "TObject.h"

class HPDNoiseDataCatalog : public TObject {
 public:
  HPDNoiseDataCatalog () {}
  virtual ~HPDNoiseDataCatalog ();
  
  void addHpd (const std::string& fName, float fRate) {mHpdName.push_back (fName), mRate.push_back (fRate);}
  size_t size () const {return mRate.size();}
  const std::vector<std::string>& allNames () const {return mHpdName;}
  float getRate (size_t i) const {return (i < mRate.size()) ? mRate[i] : 0.;}
  const std::string& getName (size_t i) const {return mHpdName[i];}
 private:
  std::vector<std::string> mHpdName;
  std::vector<float> mRate;

  ClassDef(HPDNoiseDataCatalog,1)
};

std::ostream& operator<< (std::ostream&, const HPDNoiseDataCatalog&);

#endif
