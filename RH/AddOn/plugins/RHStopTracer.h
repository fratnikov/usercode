#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimG4Core/Notification/interface/Observer.h"
#include "SimG4Core/Watcher/interface/SimWatcher.h"

#include "SimG4Core/Notification/interface/BeginOfRun.h"
#include "SimG4Core/Notification/interface/BeginOfEvent.h"
#include "SimG4Core/Notification/interface/BeginOfTrack.h"
#include "SimG4Core/Notification/interface/EndOfTrack.h"
#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Application/interface/TrackingAction.h"

  

class RHStopTracer :  public SimWatcher,
		      public Observer<const BeginOfRun *>, 
		      public Observer<const BeginOfEvent *>, 
		      public Observer<const BeginOfTrack *>,
		      public Observer<const EndOfTrack *>
{
 public:
  RHStopTracer(edm::ParameterSet const & p);
  ~RHStopTracer() {}
  void update(const BeginOfRun *);
  void update(const BeginOfEvent *);
  void update(const BeginOfTrack *);
  void update(const EndOfTrack *);
 private:
  bool mDebug;
  std::ofstream mStream;
  double mTraceEnergy;
};
