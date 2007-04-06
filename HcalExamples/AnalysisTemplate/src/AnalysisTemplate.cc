#include "AnalysisTemplate.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/EventID.h"

#include "TFile.h"

using namespace std;

namespace {
  bool verbose = true;
}

AnalysisTemplate::AnalysisTemplate (const edm::ParameterSet& fConfiguration) {
  std::string histfile = fConfiguration.getUntrackedParameter<string> ("rootFile", "AnalysisTemplate.root");
  mFile = new TFile (histfile.c_str());
}

AnalysisTemplate::~AnalysisTemplate () {
  delete mFile;
}

void AnalysisTemplate::analyze(const edm::Event& fEvent, const edm::EventSetup& fSetup) {

  // event ID
  edm::EventID eventId = fEvent.id();
  int runNumber = eventId.run ();
  int eventNumber = eventId.event ();
  if (verbose) std::cout << "======================================================================" << std::endl
			 << "AnalysisTemplate-> run/event: " << runNumber << '/' << eventNumber << std::endl;

  // get HCAL digis (samples). Presume single instance per event
  edm::Handle<HBHEDigiCollection> hbhe_digi;
  fEvent.getByType(hbhe_digi);
  edm::Handle<HODigiCollection> ho_digi;
  fEvent.getByType(ho_digi);
  edm::Handle<HFDigiCollection> hf_digi;
  fEvent.getByType(hf_digi);

  // plot something for HF
  if (verbose) std::cout << "AnalysisTemplate-> total HF digis: : " << hf_digi->size () << std::endl;
  for (unsigned ihit = 0; ihit < hf_digi->size (); ++ihit) {
    const HFDataFrame& frame = (*hf_digi)[ihit];
    HcalDetId detId = frame.id();
    int ieta = detId.ieta();
    int iphi = detId.iphi();
    int depth = detId.depth();
    if (verbose) std::cout << "AnalysisTemplate->   HF digi # " << ihit  
			   << ": cell eta/phi/depth: " << ieta << '/' << iphi << '/' << depth << std::endl;
    for (int isample = 0; isample < frame.size(); ++isample) {
      int adc = frame[isample].adc();
      int capid = frame[isample].capid ();
      if (verbose) std::cout << "AnalysisTemplate->     HF sample # " << isample 
			     << ": ADC=" << adc << ", capid=" << capid << std::endl;
      
    }
  }

  // get HCAL hits
  edm::Handle<HBHERecHitCollection> hbhe_hits;
  fEvent.getByType(hbhe_hits);
  edm::Handle<HORecHitCollection> ho_hits;
  fEvent.getByType(ho_hits);
  edm::Handle<HFRecHitCollection> hf_hits;
  fEvent.getByType(hf_hits);
}

void AnalysisTemplate::beginJob(const edm::EventSetup& fSetup) {
}

void AnalysisTemplate::endJob(void)
{
}
