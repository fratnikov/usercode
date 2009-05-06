/*

	SETUP:
	scramv1 project CMSSW CMSSW_1_3_0
	cd CMSSW_1_3_0/src
	cmscvsroot CMSSW
	cvs login
	cvs co UserCode/FedorRatnikov/HcalExamples/AnalysisTemplate
	ln -s UserCode/FedorRatnikov/HcalExamples ./
	scramv1 b
	eval `scramv1 runtime -csh`
	# customize HcalExamples/AnalysisTemplate/test/runAnalysisTemplate.cfg 
	file:
	# # of events to be processed (-1 to process all events) - line:  
	untracked int32 maxEvents = 1000
	# input file - line:  untracked vstring fileNames = 
	{'file:/bigspool/usc/USC_000206.root'}
	# output data file - line:   untracked string fileName = "events.root"
	# output root file - line:   untracked string rootFile = "histograms.root"
	# mapping file as necessary - line   FileInPath file = 
	"HcalExamples/AnalysisTemplate/data/hfplusmap.txt"

	RECONSTRUCTION AND ANALYSIS:
	for reco: ..... cmsRun HcalExamples/AnalysisTemplate/....
	for analysis: .... 

	CODE DESCRIPTION:
        The access methods to C++ objects(digi,recohit,etc) can be 
	found in the following urls for the CMSSW class definitions:
	Event ID:
	http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/DataFormats/Common/interface/EventID.h
		(this is not valid for versions after CMSSW_1_5_0_pre1)
	HCAL DB SERVICE:
        http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/CalibFormats/HcalObjects/interface/
	HCAL Detector ID:
	http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/DataFormats/HcalDetId   
	HCAL Digi:
        http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/DataFormats/HcalDigi/
	HCAL Rechit:
        http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/DataFormats/HcalRecHit/
	
	for others, look at: 
		http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/{The path given in the #include 
		statements below}

*/

#include "AnalysisTemplate.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;

AnalysisTemplate::AnalysisTemplate (const edm::ParameterSet& fConfiguration) {
  mVerbose = fConfiguration.getUntrackedParameter<bool> ("verbose", false);
  std::string histfile = fConfiguration.getUntrackedParameter<string> ("rootFile", "AnalysisTemplate.root");
  mFile = new TFile (histfile.c_str(), "RECREATE");
  // book histograms
  mAdcs = new TH1F ("adcs", "ADC", 128, 0, 128);
  mCalbVsLinearized = new TH2F ("calibVsLinearized", "Calibrated fC vs linearized fC", 1000, 0, 100, 1000, 0, 100);
  mrechits_ene = new TH1F("rechits_ene","", 100,-4,2);
  mrechits_time = new TH1F("rechits_time","", 1000,0,1870);
  mrechits = new TH2F("rechits","",13,29,42,26,0,26);
}

AnalysisTemplate::~AnalysisTemplate () {
  delete mAdcs;
  delete mCalbVsLinearized;
  delete mrechits_ene;
  delete mrechits_time;
  delete mrechits;
  delete mFile;
}

void AnalysisTemplate::analyze(const edm::Event& fEvent, const edm::EventSetup& fSetup) {
  // get calibrations
  edm::ESHandle<HcalDbService> hcalCalibrations;
  fSetup.get<HcalDbRecord>().get(hcalCalibrations);
  const HcalQIEShape* qieShape = hcalCalibrations->getHcalShape ();

  // event ID
  edm::EventID eventId = fEvent.id();
  int runNumber = eventId.run ();
  int eventNumber = eventId.event ();
  if (verbose()) std::cout << "======================================================================" << std::endl
			 << "AnalysisTemplate-> run/event: " << runNumber << '/' << eventNumber << std::endl;

  // get HCAL digis (samples). Presume single instance per event
  edm::Handle<HBHEDigiCollection> hbhe_digi;
  fEvent.getByType(hbhe_digi);
  edm::Handle<HODigiCollection> ho_digi;
  fEvent.getByType(ho_digi);
  edm::Handle<HFDigiCollection> hf_digi;
  fEvent.getByType(hf_digi);

  // plot something for HF
  if (verbose()) std::cout << "AnalysisTemplate-> total HF digis: : " << hf_digi->size () << std::endl;
  for (unsigned ihit = 0; ihit < hf_digi->size (); ++ihit) {
    const HFDataFrame& frame = (*hf_digi)[ihit];
    HcalDetId detId = frame.id();
    int ieta = detId.ieta();
    int iphi = detId.iphi();
    int depth = detId.depth();
    if (verbose()) std::cout << "AnalysisTemplate->   HF digi # " << ihit  
			   << ": cell eta/phi/depth: " << ieta << '/' << iphi << '/' << depth << std::endl;
    // get QIE calibrations for the cell
    const HcalQIECoder* coder = hcalCalibrations->getHcalCoder (detId);

    for (int isample = 0; isample < frame.size(); ++isample) {
      int adc = frame[isample].adc();
      int capid = frame[isample].capid ();
      double linear_ADC = frame[isample].nominal_fC();
      double nominal_fC = detId.subdet () == HcalForward ? 2.6 *  linear_ADC : linear_ADC;
      // convert to fC using calibrations
      double calibrated_fC = coder->charge (*qieShape, adc, capid);
      mAdcs->Fill (adc);
      mCalbVsLinearized->Fill (nominal_fC, calibrated_fC);

      if (verbose()) std::cout << "AnalysisTemplate->     HF sample # " << isample 
			     << ", capid=" << capid 
			     << ": ADC=" << adc 
			     << ", linearized ADC=" << linear_ADC
			     << ", nominal fC=" << nominal_fC 
			     << ", calibrated fC=" << calibrated_fC << std::endl;
      
    }
  }

  // get HCAL rechits
  edm::Handle<HBHERecHitCollection> hbhe_hits;
  fEvent.getByType(hbhe_hits);
  edm::Handle<HORecHitCollection> ho_hits;
  fEvent.getByType(ho_hits);
  edm::Handle<HFRecHitCollection> hf_hits;
  fEvent.getByType(hf_hits);

  HFRecHitCollection recohit = *hf_hits;
  for (unsigned i = 0; i < hf_hits->size (); ++i) {
	HcalDetId detid = recohit[i].id();
	mrechits_ene->Fill(recohit[i].energy());
	mrechits_time->Fill(recohit[i].time());
	mrechits->Fill(detid.ieta(),detid.iphi());	
  }
}

void AnalysisTemplate::beginJob(const edm::EventSetup& fSetup) {
}

void AnalysisTemplate::endJob(void)
{
  mAdcs->Write();
  mCalbVsLinearized->Write();
  mrechits_ene->Write();
  mrechits_time->Write();
  mrechits->Write();
  mFile->Write();
}
