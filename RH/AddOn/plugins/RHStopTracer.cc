#include "RHStopTracer.h"
  
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4Event.hh"

RHStopTracer::RHStopTracer(edm::ParameterSet const & p) {
  edm::ParameterSet parameters = p.getParameter<edm::ParameterSet>("RHStopTracer");
  std::string saveFile = parameters.getUntrackedParameter<std::string>("stoppedFile", "/dev/null");
  mStream.open (saveFile.c_str());
  mDebug = parameters.getUntrackedParameter<bool>("verbose", false);
  mTraceEnergy = 1000 * parameters.getUntrackedParameter<double>("traceEnergy", 1.e20); // GeV->KeV
  std::cout << "RHStopTracer::RHStopTracer->" << saveFile << '/' << mDebug << '/' << mTraceEnergy << std::endl;
}

void RHStopTracer::update (const BeginOfRun * fRun) {
  if (mDebug) 
    std::cout << "RHStopTracer::update-> begin of the run " << (*fRun)()->GetRunID () << std::endl; 
}

void RHStopTracer::update (const BeginOfEvent * fEvent) {
  if (mDebug) 
    std::cout << "RHStopTracer::update-> begin of the event " << (*fEvent)()->GetEventID () << std::endl; 
}

void RHStopTracer::update (const BeginOfTrack * fTrack) {
  const G4Track* track = (*fTrack)();
  if ((track->GetMomentum().mag()> mTraceEnergy) || track->GetDefinition()->GetParticleName().find ("~g") != std::string::npos) {
    if (mDebug)
    std::cout << "RHStopTracer::update-> new track: ID/Name/mass/Parent: " 
	      << track->GetTrackID() << '/' << track->GetDefinition()->GetParticleName() << '/' 
	      << track->GetDefinition()->GetPDGMass() << '/' << track->GetParentID()
	      << std::endl
	      << " position X/Y/Z: " << track->GetPosition().x() << '/' 
	      << track->GetPosition().y() << '/' <<  track->GetPosition().z()
	      << " R/phi: " << track->GetPosition().perp() << '/' << track->GetPosition().phi()
	      << std::endl
	      << "    px/py/pz/p=" << track->GetMomentum().x() << '/' 
	      << track->GetMomentum().y() << '/' << track->GetMomentum().z() << '/'<< track->GetMomentum().mag() 
	      << std::endl;
  }
  if (track->GetDefinition()->GetParticleName().find ("~g") == std::string::npos) { // kill regular particles
    const_cast<G4Track*>(track)->SetTrackStatus(fStopAndKill);
  }
}

void RHStopTracer::update (const EndOfTrack * fTrack) {
  const G4Track* track = (*fTrack)();
  if ((track->GetMomentum().mag()> mTraceEnergy) || track->GetDefinition()->GetParticleName().find ("~g") != std::string::npos) {
    if (mDebug)
    std::cout << "RHStopTracer::update-> stop track: ID/Name/mass/Parent: " 
	      << track->GetTrackID() << '/' << track->GetDefinition()->GetParticleName() << '/' 
	      << track->GetDefinition()->GetPDGMass() << '/' << track->GetParentID()
	      << std::endl
	      << " position X/Y/Z: " << track->GetPosition().x() << '/' 
	      << track->GetPosition().y() << '/' <<  track->GetPosition().z()
	      << " R/phi: " << track->GetPosition().perp() << '/' << track->GetPosition().phi()
	      << std::endl 
	      << "    px/py/pz/p=" << track->GetMomentum().x() << '/' 
	      << track->GetMomentum().y() << '/' << track->GetMomentum().z() << '/'<< track->GetMomentum().mag() 
	      << std::endl;
    if (track->GetMomentum().mag () < 0.001) {
      mStream << track->GetDefinition()->GetParticleName() << " " 
	      << track->GetPosition().x() << " " << track->GetPosition().y() << " " << track->GetPosition().z()
	      << std::endl; 
    }
  }
}
