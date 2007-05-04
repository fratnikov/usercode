#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

class TFile;
class TH1F;
class TH2F;

class AnalysisTemplate : public edm::EDAnalyzer {
 public:
  AnalysisTemplate (const edm::ParameterSet& fConfiguration);
  virtual ~AnalysisTemplate ();

  // analysis itself
  virtual void analyze(const edm::Event& fEvent, const edm::EventSetup& fSetup);

  // begin of the job
  virtual void beginJob(const edm::EventSetup& fSetup);

  // very end of the job
  virtual void endJob(void);

  // be verbose
  bool verbose () {return mVerbose;}
 private:
  bool mVerbose;
  TFile* mFile;
  TH1F* mAdcs;
  TH1F* mrechits_ene;
  TH1F* mrechits_time;
  TH2F* mrechits;
  TH2F* mCalbVsLinearized;
};
