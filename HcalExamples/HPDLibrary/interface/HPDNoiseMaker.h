#ifndef HPDLibrary_HPDNoiseMaker_h
#define HPDLibrary_HPDNoiseMaker_h

#include "HcalExamples/HPDLibrary/interface/HPDNoiseData.h"
#include "HcalExamples/HPDLibrary/interface/HPDNoiseDataCatalog.h"


class TFile;
class TTree;

class HPDNoiseMaker {
 public:
  HPDNoiseMaker (const std::string& fFileName);
  ~HPDNoiseMaker ();
  int addHpd (const std::string& fName, float fRate);
  void newHpdEvent (const std::string& mName, const HPDNoiseData& mData);
  void newHpdEvent (size_t i, const HPDNoiseData& mData);
  
 private:
  HPDNoiseMaker (const HPDNoiseMaker&);
  HPDNoiseMaker& operator=(const HPDNoiseMaker&);

  TFile* mFile;
  std::vector <TTree*> mTrees;
  std::vector <std::string> mNames;
  HPDNoiseDataCatalog mCatalog;
};

#endif
