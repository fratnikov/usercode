#include "HcalExamples/HPDLibrary/interface/HPDNoiseMaker.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include <iostream>

HPDNoiseMaker::HPDNoiseMaker (const std::string& fFileName) {
  mFile = new TFile (fFileName.c_str(), "RECREATE");
}

HPDNoiseMaker::~HPDNoiseMaker () {
  mFile->cd();  
  for (size_t i = 0; i < mTrees.size(); ++i) {
    std::cout << "HPDNoiseMaker::~HPDNoiseMaker-> " << i << '/' << mTrees[i]->GetEntries() << std::endl;
    mTrees[i]->Write();
    delete mTrees[i];
  }
  mCatalog.Write();
  delete mFile;
}

int HPDNoiseMaker::addHpd (const std::string& fName, float fRate) {
   TDirectory* currentDirectory = gDirectory;
   mFile->cd();
  mCatalog.addHpd (fName, fRate);
  mNames.push_back (fName);
  mTrees.push_back (new TTree (fName.c_str(), fName.c_str()));
  void* addr = 0;
  mTrees.back()->Branch ("HPDNoiseData", "HPDNoiseData", &addr, 32000, 1);
   currentDirectory->cd();
  return mNames.size();
}

void HPDNoiseMaker::newHpdEvent (const std::string& fName, const HPDNoiseData& fData) {
  for (size_t i = 0; i < mNames.size(); ++i) {
    if (mNames[i] == fName) {
      newHpdEvent (i, fData);
    }
  }
}

void HPDNoiseMaker::newHpdEvent (size_t i, const HPDNoiseData& fData) {
  if (i < mTrees.size()) {
    HPDNoiseData* data = (HPDNoiseData*) &fData;
    mTrees[i]->GetBranch ("HPDNoiseData")->SetAddress(&data);
    mTrees[i]->Fill();
  }
}

