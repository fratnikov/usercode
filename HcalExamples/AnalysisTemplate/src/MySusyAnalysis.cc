#include "MySusyAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/PatCandidates/interface/MHT.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

using namespace std;
using namespace reco;

namespace {
  const double MU_R_ISO = 0.3;
  const double MU_PT = 20;
  const double MU_ETA = 2.1;
  const double MU_RELIS = 0.1;
  const double MU_CHI2NDOF = 10;
  const double MU_D0 = 0.2;
  const int MU_NHITS = 11;
  const double MU_HCAL_E = 6;
  const double MU_ECAL_E = 4;

  const double ELE_R_ISO = 0.3;
  const double ELE_ET = 20;
  const double ELE_ETA = 2.5;
  const double ELE_RELIS = 0.1;
  const double ELE_D0 = 0.2;

  const double JET_PT = 50;
  const double JET_ETA = 2.4;
  const double JET_HADFRAC = 0.1;

  const double MET_ET = 100.;
  
  const bool TRACE = false; 
}

MySusyAnalysis::MySusyAnalysis (const edm::ParameterSet& fConfiguration) {
  mVerbose = fConfiguration.getUntrackedParameter<bool> ("verbose", false);
  mReweight = fConfiguration.getUntrackedParameter<bool> ("reweight", false);
  edm::Service<TFileService> fileService;
  // book histograms

  mMetEt = fileService->make<TH1F> ("mMetEt","MetEt",250, 0, 1250);
  mFinalMet = fileService->make<TH1F> ("mFinalMet","FinalMet",250, 0, 1250);

  mJetPt = fileService->make<TH1F> ("mJetPt","JetPt",250, 0, 500);
  mJetEta = fileService->make<TH1F> ("mJetEta","JetEta",100, -5, 5);
  mJetHadFraction = fileService->make<TH1F> ("mJetHadFraction","JetHadFraction",102, -0.01, 1.01 );
  mNJetsGood = fileService->make<TH1F> ("mNJetsGood","NJetsGood", 25, 0, 25 );

  mElectronGood = fileService->make<TH1F> ("mElectronGood","ElectronGood",2, -0.5, 1.5);
  mElectronEt = fileService->make<TH1F> ("mElectronEt", "ElectronEt",250, 0, 500);
  mElectronEta = fileService->make<TH1F> ("mElectronEta", "ElectronEta",100, -5, 5);
  mElectronHcalIsoRel = fileService->make<TH1F> ("mElectronHcalIsoRel", "ElectronHcalIsoRel",100, 0, 2.5);
  mElectronEcalIsoRel = fileService->make<TH1F> ("mElectronEcalIsoRel", "ElectronEcalIsoRel",100, 0, 2.5);
  mElectronTrackIsoRel = fileService->make<TH1F> ("mElectronTrackIsoRel", "ElectronTrackIsoRel",100, 0, 2.5);
  mElectronTotalIsoRel = fileService->make<TH1F> ("mElectronTotalIsoRel", "ElectronTotalIsoRel",100, 0, 2.5);
  mElectronD0 = fileService->make<TH1F> ("mElectronD0", "ElectronD0",500, 0, 1.);
  mNElectronsGood = fileService->make<TH1F> ("mNElectronsGood", "NElectronsGood",10, 0, 10);


  mMuonGood = fileService->make<TH1F> ("mMuonGood","MuonGood",2, -0.5, 1.5);
  mMuonPt = fileService->make<TH1F> ("mMuonPt", "MuonPt",250, 0, 500);
  mMuonEta = fileService->make<TH1F> ("mMuonEta", "MuonEta",100, -5, 5);
  mMuonHcalIsoRel = fileService->make<TH1F> ("mMuonHcalIsoRel", "MuonHcalIsoRel",100, 0, 2.5);
  mMuonEcalIsoRel = fileService->make<TH1F> ("mMuonEcalIsoRel", "MuonEcalIsoRel",100, 0, 2.5);
  mMuonTrackIsoRel = fileService->make<TH1F> ("mMuonTrackIsoRel", "MuonTrackIsoRel",100, 0, 2.5);
  mMuonTotalIsoRel = fileService->make<TH1F> ("mMuonTotalIsoRel", "MuonTotalIsoRel",100, 0, 2.5);
  mMuonChi2 = fileService->make<TH1F> ("mMuonChi2", "MuonChi2",100, 0, 200);
  mMuonDof = fileService->make<TH1F> ("mMuonDof", "MuonDof",100, 0, 100);
  mMuonChi2Dof = fileService->make<TH1F> ("mMuonChi2Dof", "MuonChi2Dof",100, 0, 50);
  mMuonD0 = fileService->make<TH1F> ("mMuonD0", "MuonD0",500, 0, 1.);
  mMuonNhits = fileService->make<TH1F> ("mMuonNhits", "MuonNhits",50, 0, 50.);
  mMuonHcalE = fileService->make<TH1F> ("mMuonHcalE", "MuonHcalE",100, 0, 10);
  mMuonEcalE = fileService->make<TH1F> ("mMuonEcalE", "MuonEcalE",100, 0, 20);

  mNMuonsGood = fileService->make<TH1F> ("mNMuonsGood", "NMuonsGood",10, 0, 10);

  mStepByStep = fileService->make<TH1F> ("mStepByStep", "StepByStep", 20, 0.5, 20.5);
}


MySusyAnalysis::~MySusyAnalysis () {
}

void MySusyAnalysis::analyze(const edm::Event& fEvent, const edm::EventSetup& fSetup) {
  
  // MET
  bool goodMET = false;
  double met = 0;
  if (TRACE) std::cout << "Processing jets..." << std::endl;
  edm::Handle<pat::METCollection> caloMET;
  fEvent.getByLabel("selectedLayer1METs", caloMET);
  if (!caloMET.isValid()) {
    std::cout << "Can't find pat::METCollection selectedLayer1METs" << std::endl;
    return;
  }
  met = caloMET->front().pt();
  mMetEt->Fill (met);
  if (TRACE) std::cout << "MET = " << met << std::endl;
  if (met >= MET_ET) goodMET = true;
  
  if (TRACE) std::cout << "Processing jets..." << std::endl;
  // Jets
  int nJets = 0;
  edm::Handle<pat::JetCollection> caloJets;
  fEvent.getByLabel("selectedLayer1Jets", caloJets);
  if (!caloJets.isValid()) {
    std::cout << "Can't find pat::JetCollection selectedLayer1Jets" << std::endl;
    return;
  }
  for (unsigned iJet = 0; iJet < caloJets->size(); ++iJet) {
    const pat::Jet& jet = (*caloJets)[iJet];
    double pt = jet.pt();
    mJetPt->Fill (pt);
    if (TRACE) std::cout << "jet pt " << pt << std::endl;
    if (pt < JET_PT) continue;
    double eta = jet.eta();
    mJetEta->Fill (eta);
    if (TRACE) std::cout << "jet eta " << eta << std::endl;
    if (fabs (eta) > JET_ETA) continue;
    double hadFrac = jet.energyFractionHadronic ();
    mJetHadFraction->Fill (hadFrac);
    if (TRACE) std::cout << "jet had fraction " << hadFrac << std::endl;
    if (hadFrac < JET_HADFRAC) continue;
    ++nJets;
  }
  mNJetsGood->Fill (nJets);
  
  // beam spot
  if (TRACE) std::cout << "Processing beamspot..." << std::endl;
  edm::Handle<reco::BeamSpot> beamSpot;
  fEvent.getByLabel("offlineBeamSpot", beamSpot);
  if (!beamSpot.isValid()) {
    std::cout << "Can't find reco::BeamSpot offlineBeamSpot" << std::endl;
    return;
  }

  if (TRACE) std::cout << "Processing muons..." << std::endl;
  // muons
  int nMuons = 0;
  edm::Handle<pat::MuonCollection> muons;
  fEvent.getByLabel("selectedLayer1Muons", muons);
  if (!muons.isValid()) {
    std::cout << "Can't find pat::MuonCollection selectedLayer1Muons" << std::endl;
    return;
  }
  for (unsigned iMuon = 0; iMuon < muons->size(); iMuon++) {
    const pat::Muon& muon = (*muons)[iMuon];
    if (TRACE) std::cout << "===> muon #" << iMuon << std::endl;
      TH1F* mStepByStepRel;

    int good = muon.isGood(reco::Muon::GlobalMuonPromptTight) ? 1 : 0;
    //      int good = muon.isGlobalMuon () ? 1 : 0;
    mMuonGood->Fill (good);
    if (!good) continue;
    double pt = muon.pt();
    mMuonPt->Fill (pt);
    if (TRACE) std::cout << "muon pt " << pt << std::endl;
    if (pt < MU_PT) continue;
    double eta = muon.eta();
    mMuonEta->Fill (eta);
    if (TRACE) std::cout << "muon eta " << fabs(eta) << std::endl;
    if (fabs(eta) > MU_ETA) continue;
    double hcalIso = muon.hcalIsoDeposit()->depositWithin (MU_R_ISO) / pt;
    mMuonHcalIsoRel->Fill (hcalIso);
    double ecalIso = muon.ecalIsoDeposit()->depositWithin (MU_R_ISO) / pt;
    mMuonEcalIsoRel->Fill (ecalIso);
    double trackIso = muon.trackerIsoDeposit()->depositWithin (MU_R_ISO) / pt;
    mMuonTrackIsoRel->Fill (trackIso);
    double relIso = hcalIso + ecalIso + trackIso;
    mMuonTotalIsoRel->Fill (relIso);
    if (TRACE) std::cout << "muon h/e/tra REL ISO" 
			 << hcalIso << '/' << ecalIso << '/' << trackIso << ' ' << relIso << std::endl;
    if (relIso > MU_RELIS) continue;
    double combinedChi2 = muon.combinedMuon()->chi2();
    mMuonChi2->Fill (combinedChi2);
    double combinedNdf = muon.combinedMuon()->ndof();
    mMuonDof->Fill (combinedNdf);
    double chi2ndof = combinedChi2 / combinedNdf;
    mMuonChi2Dof->Fill (chi2ndof);
    if (TRACE) std::cout << "muon chi2/ndf" << combinedChi2 << '/' 
			 << combinedNdf << '=' << chi2ndof << std::endl;
    if (chi2ndof > MU_CHI2NDOF) continue;
    double d0 = fabs(muon.track()->dxy (beamSpot->position()));
    mMuonD0->Fill (d0);
    if (TRACE) std::cout << "muon d0" << d0 << std::endl;
    if (d0 > MU_D0) continue;
    int trackHits = muon.track()->numberOfValidHits();
    mMuonNhits->Fill (trackHits);
    if (TRACE) std::cout << "muon NHits" << trackHits << std::endl;
    if (trackHits < MU_NHITS) continue;
    double energyHcal = muon.hcalIsoDeposit()->candEnergy();
    mMuonHcalE->Fill (energyHcal);
    if (TRACE) std::cout << "muon HCAL e" << energyHcal << std::endl;
    if (energyHcal > MU_HCAL_E) continue;
    double energyEcal = muon.ecalIsoDeposit()->candEnergy();
    mMuonEcalE->Fill (energyEcal);
    if (TRACE) std::cout << "muon ECAL e" << energyEcal << std::endl;
    if (energyEcal > MU_ECAL_E) continue;
    ++nMuons;
  }
  mNMuonsGood->Fill (nMuons);

  if (TRACE) std::cout << "Processing electrons..." << std::endl;
  // electrons
  int nElectrons = 0;
  edm::Handle<pat::ElectronCollection> electrons;
  fEvent.getByLabel("selectedLayer1Electrons", electrons);
  if (!electrons.isValid()) {
    std::cout << "Can't find pat::ElectronCollection selectedLayer1Electrons" << std::endl;
    return;
  }
  for (unsigned iElectron = 0; iElectron < electrons->size(); iElectron++) {
    const pat::Electron& electron = (*electrons)[iElectron];
    if (TRACE) std::cout << "===> electron #" << iElectron << std::endl;
    
    int good = electron.electronID("eidRobustTight") ? 1 : 0;
    mElectronGood->Fill (good);
    if (!good) continue;
    double et = electron.et();
    mElectronEt->Fill (et);
    if (TRACE) std::cout << "electron et " << et << std::endl;
    if (et < ELE_ET) continue;
    double eta = electron.eta();
    double pt = electron.pt();
    mElectronEta->Fill (eta);
    if (TRACE) std::cout << "electron eta " << fabs(eta) << std::endl;
    if (fabs(eta) > ELE_ETA) continue;
    double hcalIso = electron.hcalIsoDeposit()->depositWithin (ELE_R_ISO) / et;
    mElectronHcalIsoRel->Fill (hcalIso);
    double ecalIso = (electron.ecalIsoDeposit()->depositWithin (ELE_R_ISO) - 
		      electron.caloEnergy()) / et;
    mElectronEcalIsoRel->Fill (ecalIso);
//     double trackIso = (electron.trackerIsoDeposit()->depositWithin (ELE_R_ISO) - 
// 		       electron.gsfTrack()->pt()) / pt;
    double trackIso = 0;
    mElectronTrackIsoRel->Fill (trackIso);
    double relIso = hcalIso + ecalIso + trackIso;
    mElectronTotalIsoRel->Fill (relIso);
    if (TRACE) std::cout << "electron h/e/tra REL ISO" 
			 << hcalIso << '/' << ecalIso << '/' << trackIso << ' ' << relIso << std::endl;
    if (relIso > ELE_RELIS) continue;
    double d0 = fabs(electron.gsfTrack()->dxy (beamSpot->position()));
    mElectronD0->Fill (d0);
    if (TRACE) std::cout << "electron d0" << d0 << std::endl;
    if (d0 > ELE_D0) continue;
    ++nElectrons;
  }
  mNElectronsGood->Fill (nElectrons);

  if (TRACE) std::cout << "MET/muons/jets/electrons: " << goodMET << '/' 
		       << nMuons << '/' << nJets << '/' << nElectrons << std::endl;
  // fill summary
  mStepByStep->Fill (1);
  if (nMuons == 1) {
    mStepByStep->Fill (2);
    if (nElectrons == 0) {
      mStepByStep->Fill (3);
      if (nJets >= 3) {
	mStepByStep->Fill (4);
	if (goodMET) {
	  mStepByStep->Fill (5);
	  mFinalMet->Fill (met); // mu+3jet+MET_noe
	  if (TRACE) std::cout << "===> Event is accepted <===" << std::endl;
	}
      }
    }
  }
}

void MySusyAnalysis::beginJob(const edm::EventSetup& fSetup) {
}

void MySusyAnalysis::endJob(void)
{
  //  mAdcs->Write();
}
