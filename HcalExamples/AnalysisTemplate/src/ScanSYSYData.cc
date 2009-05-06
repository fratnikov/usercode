#include "ScanSUSYData.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace std;
using namespace reco;

namespace {
  double reweights[300];
}

ScanSUSYData::ScanSUSYData (const edm::ParameterSet& fConfiguration) {
  mVerbose = fConfiguration.getUntrackedParameter<bool> ("verbose", false);
  mReweight = fConfiguration.getUntrackedParameter<bool> ("reweight", false);
  edm::Service<TFileService> fileService;
  // book histograms
  mSusyProcess = fileService->make<TH1F> ("mSusyProcess", "SusyProcess", 100, 200, 300);
  mGenMET = fileService->make<TH1F> ("mGenMET", "GenMET", 250, 0, 500);
  mRecoMET = fileService->make<TH1F> ("mRecoMET", "RecoMET", 250, 0, 500);
  mGenHT = fileService->make<TH1F> ("mGenHT", "GenHT", 250, 0, 500);
  mRecoHT = fileService->make<TH1F> ("mRecoHT", "RecoHT", 250, 0, 500);
  mFirstJetPtGen = fileService->make<TH1F> ("mFirstJetPtGen", "FirstJetPtGen", 250, 0, 500);
  mFirstJetPtReco = fileService->make<TH1F> ("mFirstJetPtReco", "FirstJetPtReco", 250, 0, 500);
  mFirstJetEtaGen = fileService->make<TH1F> ("mFirstJetEtaGen", "FirstJetEtaGen", 100, -5, 5);
  mFirstJetEtaReco = fileService->make<TH1F> ("mFirstJetEtaReco", "FirstJetEtaReco", 100, -5, 5);
  mJetPtGen = fileService->make<TH1F> ("mJetPtGen", "JetPtGen", 250, 0, 500);
  mJetPtReco = fileService->make<TH1F> ("mJetPtReco", "JetPtReco", 250, 0, 500);
  mNJets20GevGen = fileService->make<TH1F> ("mNJets20GevGen", "NJets20GevGen", 20, 0, 20);
  mNJets20GevReco = fileService->make<TH1F> ("mNJets20GevReco", "NJets20GevReco", 20, 0, 20);
  mNMuonsGen = fileService->make<TH1F> ("mNMuonsGen", "NMuonsGen", 20, 0, 20);
  mNMuonsReco = fileService->make<TH1F> ("mNMuonsReco", "NMuonsReco", 20, 0, 20);
  mPtMuonsGen = fileService->make<TH1F> ("mPtMuonsGen", "PtMuonsGen", 250, 0, 500);
  mPtMuonsReco = fileService->make<TH1F> ("mPtMuonsReco", "PtMuonsReco", 250, 0, 500);

  // reweights
  for (unsigned i = 0; i < 300; ++i) reweights[i] = 0.;
reweights[217] = 15.2593;
reweights[218] = 26.5;
reweights[221] = 21;
reweights[222] = 14.0632;
reweights[223] = 8.5;
reweights[224] = 13.8346;
reweights[225] = 5;
reweights[226] = 16.4706;
reweights[227] = 13.5412;
reweights[228] = 13.0109;
reweights[229] = 16.6813;
reweights[230] = 5.80722;
reweights[231] = 4.68268;
reweights[232] = 5.66391;
reweights[233] = 6.05172;
reweights[234] = 4;
reweights[235] = 7.82653;
reweights[236] = 4.6875;
reweights[237] = 4.52961;
reweights[238] = 12.375;
reweights[239] = 15.2727;
reweights[241] = 12.8;
reweights[242] = 19.8214;
reweights[243] = 26.5;
reweights[244] = 13.0125;
reweights[245] = 13.8583;
reweights[248] = 10;
reweights[255] = 16;
reweights[257] = 6.33333;
reweights[259] = 13.5634;
reweights[260] = 12.8;
reweights[262] = 20;
reweights[265] = 10.5;
reweights[272] = 29;
reweights[273] = 10;
reweights[274] = 13;
reweights[276] = 4;
reweights[288] = 4.5;
reweights[289] = 2;
reweights[295] = 14.5556;
reweights[296] = 12.1667;
}

ScanSUSYData::~ScanSUSYData () {
}

void ScanSUSYData::analyze(const edm::Event& fEvent, const edm::EventSetup& fSetup) {
  double w = 1.;
  // process ID
  edm::Handle<int> genProcessID;
  fEvent.getByLabel( "genEventProcID", genProcessID );
  if (genProcessID.isValid()) {
    int processId = *genProcessID;
    if (mReweight) w = reweights [processId];
    mSusyProcess->Fill (double(processId), w);
  }

  // MET
  edm::Handle<CaloMETCollection> caloMET;
  fEvent.getByLabel("met", caloMET);
  if (caloMET.isValid()) {
    mRecoMET->Fill (caloMET->front().pt(), w);
  }
   
  edm::Handle<GenMETCollection> genMET;
  fEvent.getByLabel("genMetNoNuBSM", genMET);
  if (genMET.isValid()) {
    mGenMET->Fill (genMET->front().pt(), w);
  }
  
  // HT
  edm::Handle<METCollection> caloHT;
  fEvent.getByLabel("htMetIC5", caloHT);
  if (caloHT.isValid()) {
    mRecoHT->Fill (caloHT->front().pt(), w);
  }
  
  edm::Handle<METCollection> genHT;
  fEvent.getByLabel("genMetIC5GenJets", genHT);
  if (genHT.isValid()) {
    mGenHT->Fill (genHT->front().pt(), w);
  }
  
  // Jets
  edm::Handle<CaloJetCollection> caloJets;
  fEvent.getByLabel("sisCone5CaloJets", caloJets);
  if (caloJets.isValid()) {
    int nJets = 0;
    if (caloJets->size() > 0) {
      const CaloJet& firstJet = (*caloJets)[0];
      mFirstJetPtReco->Fill (firstJet.pt(), w);
      mFirstJetEtaReco->Fill (firstJet.eta(), w);
    }
    else {
      mFirstJetPtReco->Fill (0., w);
      mFirstJetEtaReco->Fill (0., w);
    }
    for (unsigned iJet = 0; iJet < caloJets->size(); ++iJet) {
      const CaloJet& jet = (*caloJets)[iJet];
      mJetPtReco->Fill (jet.pt(), w);
      if (jet.pt() > 20) ++nJets;
    }
    mNJets20GevReco->Fill (nJets, w);
  }
  
  edm::Handle<GenJetCollection> genJets;
  fEvent.getByLabel("sisCone5GenJets", genJets);
  if (genJets.isValid()) {
    int nJets = 0;
    if (genJets->size() > 0) {
      const GenJet& firstJet = (*genJets)[0];
      mFirstJetPtGen->Fill (firstJet.pt(), w);
      mFirstJetEtaGen->Fill (firstJet.eta(), w);
    }
    else {
      mFirstJetPtReco->Fill (0., w);
      mFirstJetEtaReco->Fill (0., w);
    }
    for (unsigned iJet = 0; iJet < genJets->size(); ++iJet) {
      const GenJet& jet = (*genJets)[iJet];
      mJetPtGen->Fill (jet.pt(), w);
      if (jet.pt() > 20) ++nJets;
    }
    mNJets20GevGen->Fill (nJets, w);
  }
  
  // muons
  edm::Handle<edm::View<reco::Muon> > muons;
  fEvent.getByLabel("muons", muons);
  if (muons.isValid()) {
    int nMuons = 0;
    for (unsigned iMuon = 0; iMuon < muons->size(); iMuon++) {
      mPtMuonsReco->Fill ((*muons)[iMuon].pt(), w);
      ++nMuons;
    }
    mNMuonsReco->Fill (nMuons, w);
  }
  
  edm::Handle<reco::GenParticleCollection> genParticles;
  fEvent.getByLabel("genParticles", genParticles);
  if (genParticles.isValid()) {
    int nMuons = 0;
    for (unsigned iPart = 0; iPart < genParticles->size(); ++iPart) {
      const GenParticle& genPart = (*genParticles)[iPart];
      if (abs (genPart.pdgId() == 15)) {
	int motherId = abs (genPart.mother()->pdgId());
	if (motherId == 23 || // Z
	    motherId == 24 || // W
	    motherId == 25 || // H
	    motherId > 1000000) // SUSY
	  {
	    mPtMuonsGen->Fill (genPart.pt(), w);
	    ++nMuons;
	  }
      }
    }
    mNMuonsGen->Fill (nMuons, w);
  }
}

void ScanSUSYData::beginJob(const edm::EventSetup& fSetup) {
}

void ScanSUSYData::endJob(void)
{
  //  mAdcs->Write();
}
