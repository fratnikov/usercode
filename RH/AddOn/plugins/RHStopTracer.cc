#include "RHStopTracer.h"
  
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4Event.hh"

namespace {
  bool debug = false;
  std::ofstream out ("stopped_rhadrons.txt");
}

void RHStopTracer::update (const BeginOfRun * fRun) {
  std::cout << "RHStopTracer::update-> begin of the run " << (*fRun)()->GetRunID () << std::endl; 
}

void RHStopTracer::update (const BeginOfEvent * fEvent) {
  std::cout << "RHStopTracer::update-> begin of the event " << (*fEvent)()->GetEventID () << std::endl; 
}

void RHStopTracer::update (const BeginOfTrack * fTrack) {
  const G4Track* track = (*fTrack)();
  if ((track->GetMomentum().mag()> 100.) || track->GetDefinition()->GetParticleName().find ("~g") != std::string::npos) {
    if (debug)
    std::cout << "RHStopTracer::update-> new track: ID/Name/Parent: " 
	      << track->GetTrackID() << '/' << track->GetDefinition()->GetParticleName() << '/' 
	      << track->GetParentID()
	      << std::endl
	      << " position X/Y/Z: " << track->GetPosition().x() << '/' 
	      << track->GetPosition().y() << '/' <<  track->GetPosition().z()
	      << " R/phi: " << track->GetPosition().perp() << '/' << track->GetPosition().phi()
	      << std::endl
	      << "    px/py/pz/p=" << track->GetMomentum().x() << '/' 
	      << track->GetMomentum().y() << '/' << track->GetMomentum().z() << '/'<< track->GetMomentum().mag() 
	      << std::endl;
  }
 
}

void RHStopTracer::update (const EndOfTrack * fTrack) {
  const G4Track* track = (*fTrack)();
  if ((track->GetMomentum().mag()> 100.) || track->GetDefinition()->GetParticleName().find ("~g") != std::string::npos) {
    if (debug)
    std::cout << "RHStopTracer::update-> stop track: ID/Name/Parent: " 
	      << track->GetTrackID() << '/' << track->GetDefinition()->GetParticleName() << '/' << track->GetParentID()
	      << std::endl
	      << " position X/Y/Z: " << track->GetPosition().x() << '/' 
	      << track->GetPosition().y() << '/' <<  track->GetPosition().z()
	      << " R/phi: " << track->GetPosition().perp() << '/' << track->GetPosition().phi()
	      << std::endl 
	      << "    px/py/pz/p=" << track->GetMomentum().x() << '/' 
	      << track->GetMomentum().y() << '/' << track->GetMomentum().z() << '/'<< track->GetMomentum().mag() 
	      << std::endl;
    if (track->GetMomentum().mag () < 0.001) {
      out << track->GetDefinition()->GetParticleName() << " " 
	  << track->GetPosition().x() << " " << track->GetPosition().y() << " " << track->GetPosition().z()
	  << std::endl; 
    }
  }
}
