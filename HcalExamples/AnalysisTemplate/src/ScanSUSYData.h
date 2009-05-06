#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

class TFile;
class TH1F;
class TH2F;

class ScanSUSYData : public edm::EDAnalyzer {
 public:
  ScanSUSYData (const edm::ParameterSet& fConfiguration);
  virtual ~ScanSUSYData ();

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
  bool mReweight;
  TH1F* mSusyProcess;
  TH1F* mGenMET;
  TH1F* mRecoMET;
  TH1F* mGenHT;
  TH1F* mRecoHT;
  TH1F* mFirstJetPtGen;
  TH1F* mFirstJetPtReco;
  TH1F* mFirstJetEtaGen;
  TH1F* mFirstJetEtaReco;
  TH1F* mJetPtGen;
  TH1F* mJetPtReco;
  TH1F* mNJets20GevGen;
  TH1F* mNJets20GevReco;
  TH1F* mNMuonsGen;
  TH1F* mNMuonsReco;
  TH1F* mPtMuonsGen;
  TH1F* mPtMuonsReco;
};
