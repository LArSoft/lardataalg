////////////////////////////////////////////////////////////////////////
//
//  RunHistory
//
//  jpaley@fnal.gov
//
////////////////////////////////////////////////////////////////////////
// Framework includes

// C++ language includes

// LArSoft includes
#include "lardataalg/DetectorInfo/RunHistoryStandard.h"

namespace detinfo {
  //-----------------------------------------------
  RunHistoryStandard::RunHistoryStandard() : fRun(-1), fNSubruns(0), fRunType(kUnknownRunType) {}

  //-----------------------------------------------
  RunHistoryStandard::RunHistoryStandard(int run)
  {
    fRun = run;
  }

  //------------------------------------------------
  RunHistoryStandard::~RunHistoryStandard() {}

  //------------------------------------------------
  bool RunHistoryStandard::Update(uint64_t ts)
  {
    if (ts == 0) return false;

    return true;
  }

  //------------------------------------------------
  std::string RunHistoryStandard::RunTypeAsString() const
  {
    switch (fRunType) {
    case (kProductionRun): return std::string("Production");
    case (kCommissioningRun): return std::string("Commissioning");
    case (kTestRun): return std::string("Test");
    case (kPedestalRun): return std::string("Pedestal");
    case (kCalibrationRun): return std::string("Calibration");
    case (kUnknownRunType):
    default: return std::string("Uknown");
    }
  }
}
