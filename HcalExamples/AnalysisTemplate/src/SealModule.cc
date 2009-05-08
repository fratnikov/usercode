#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "AnalysisTemplate.h"
#include "ScanSUSYData.h"
#include "MySusyAnalysis.h"
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(AnalysisTemplate);
DEFINE_ANOTHER_FWK_MODULE(ScanSUSYData);
DEFINE_ANOTHER_FWK_MODULE(MySusyAnalysis);
