#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

class TFile;
class TH1F;
class TH2F;

class MySusyAnalysis : public edm::EDAnalyzer {
 public:
  MySusyAnalysis (const edm::ParameterSet& fConfiguration);
  virtual ~MySusyAnalysis ();

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

  
  // met
  TH1F* mMetEt;
  TH1F* mFinalMet;

  //jets
  TH1F* mJetPt;
  TH1F* mJetEta;
  TH1F* mJetHadFraction;

  TH1F* mNJetsGood;

  // muons
  TH1F* mMuonGood;
  TH1F* mMuonPt;
  TH1F* mMuonEta;
  TH1F* mMuonHcalIsoRel;
  TH1F* mMuonEcalIsoRel;
  TH1F* mMuonTrackIsoRel;
  TH1F* mMuonTotalIsoRel;
  TH1F* mMuonChi2;
  TH1F* mMuonDof;
  TH1F* mMuonChi2Dof;
  TH1F* mMuonD0;
  TH1F* mMuonNhits;
  TH1F* mMuonHcalE;
  TH1F* mMuonEcalE;

  TH1F* mNMuonsGood;
};
