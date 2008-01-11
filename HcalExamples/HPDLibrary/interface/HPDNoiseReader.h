#ifndef HPDLibrary_HPDNoiseReader_h
#define HPDLibrary_HPDNoiseReader_h

#include <string>
#include <vector>

class TFile;
class TTree;
class HPDNoiseData;

class HPDNoiseReader {
 public:
  typedef int Handle;
  HPDNoiseReader (const std::string& fFileName);
  ~HPDNoiseReader ();

  std::vector<std::string> allNames () const;
  Handle getHandle (const std::string& fName);
  bool valid (Handle fHandle) const {return fHandle >= 0;}
  float rate (Handle fHandle) const;
  unsigned long totalEntries (Handle fHandle) const;
  void getEntry (Handle fHandle, unsigned long fEntry, HPDNoiseData** fData);
  
 private:
  HPDNoiseReader (const HPDNoiseReader&); // no copy
  HPDNoiseReader& operator=(const HPDNoiseReader&); // no copy
  void grabEntry (Handle fHandle, unsigned long fEntry);

  TFile* mFile;
  std::vector <TTree*> mTrees;
  std::vector <float> mRates;
  HPDNoiseData* mBuffer;
};

#endif
