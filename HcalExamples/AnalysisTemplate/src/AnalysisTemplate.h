#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

class TFile;

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
 private:
  TFile* mFile;
};
