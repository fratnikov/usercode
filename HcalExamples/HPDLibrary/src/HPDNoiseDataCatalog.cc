#include "HcalExamples/HPDLibrary/interface/HPDNoiseDataCatalog.h"

HPDNoiseDataCatalog::~HPDNoiseDataCatalog () {}


std::ostream& operator<< (std::ostream& fStream, const HPDNoiseDataCatalog& fCatalog) {
  fStream << "Name:Rate";
  for (size_t i = 0; i < fCatalog.size(); ++i) fStream << ' ' << fCatalog.getName (i) << ':' << fCatalog.getRate (i);
  return fStream;
}
