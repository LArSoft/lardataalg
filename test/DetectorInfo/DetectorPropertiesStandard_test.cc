/**
 * @file   DetectorPropertiesStandard_test.cc
 * @brief  Simple instantiation-only test for DetectorPropertiesStandard
 * @author Gianluca Petrillo (petrillo@fnal.gov)
 * @date   May 6th, 2016
 */

// LArSoft libraries
#include "larcorealg/CoreUtils/RealComparisons.h"
#include "larcorealg/Geometry/GeoObjectSorterStandard.h"
#include "larcorealg/Geometry/GeometryCore.h"
#include "larcorealg/Geometry/GeometryDataContainers.h"
#include "larcorealg/Geometry/StandaloneGeometrySetup.h"
#include "larcorealg/Geometry/WireReadoutGeom.h"
#include "larcorealg/TestUtils/geometry_unit_test_base.h"
#include "lardataalg/DetectorInfo/DetectorClocksStandardTestHelpers.h"
#include "lardataalg/DetectorInfo/DetectorPropertiesStandard.h"
#include "lardataalg/DetectorInfo/DetectorPropertiesStandardTestHelpers.h"
#include "lardataalg/DetectorInfo/LArPropertiesStandardTestHelpers.h"

// art libraries
#include "fhiclcpp/ParameterSet.h"

// C/C++ standard libraries
#include <array>
#include <iomanip>

//------------------------------------------------------------------------------
//---  The test environment
//---

/*
 * GeometryTesterEnvironment, configured with a geometry-aware configuration object, is
 * used in a non-Boost-unit-test context.
 * It provides:
 * - all the other services configured as dependencies
 */
using TesterConfiguration = testing::BasicGeometryEnvironmentConfiguration;
using TestEnvironment =
  testing::GeometryTesterEnvironment<TesterConfiguration, geo::GeoObjectSorterStandard>;

//------------------------------------------------------------------------------
//---  The tests
//---

/** ****************************************************************************
 * @brief Runs the test
 * @param argc number of arguments in argv
 * @param argv arguments to the function
 * @return number of detected errors (0 on success)
 * @throw cet::exception most of error situations throw
 *
 * The arguments in argv are:
 * 0. name of the executable ("DetectorPropertiesStandard_test")
 * 1. (mandatory) path to the FHiCL configuration file
 * 2. FHiCL path to the configuration of the test (default: physics.analyzers.larptest)
 * 3. FHiCL path to the configuration of DetectorProperties service
 *    (default: services.DetectorPropertiesService)
 *
 */
//------------------------------------------------------------------------------
int main(int argc, char const** argv)
{

  TesterConfiguration config("detp_test");

  //
  // parameter parsing
  //
  int iParam = 0;

  // first argument: configuration file (mandatory)
  if (++iParam < argc)
    config.SetConfigurationPath(argv[iParam]);
  else {
    std::cerr << "FHiCL configuration file path required as first argument!" << std::endl;
    return 1;
  }

  // second argument: path of the parameter set for geometry test configuration (optional;
  // default does not have any tester)
  if (++iParam < argc) config.SetMainTesterParameterSetPath(argv[iParam]);

  // third argument: path of the parameter set for DetectorProperties confi (optional;
  // default: "services.DetectorProperties" from inherited object)
  if (++iParam < argc) {
    config.SetServiceParameterSetPath("DetectorPropertiesService", argv[iParam]);
  }

  unsigned int nErrors = 0 /* Tester.Run() */;

  //
  // testing environment setup
  //
  TestEnvironment TestEnv(config);
  TestEnv.AcquireProvider(lar::standalone::SetupReadout({}, TestEnv.Geometry()));

  // DetectorPropertiesStandard and all its dependencies support the simple set up (see
  // testing::TesterEnvironment::SimpleProviderSetup()), except for Geometry, that has
  // been configured already in the geometry-aware environment. So we invoke a simple set
  // up for each of the dependencies:
  TestEnv.SimpleProviderSetup<detinfo::LArPropertiesStandard>();
  TestEnv.SimpleProviderSetup<detinfo::DetectorClocksStandard>();
  TestEnv.SimpleProviderSetup<detinfo::DetectorPropertiesStandard>();

  //
  // run the test algorithm
  // (I leave it here for reference -- there is no test algorithm here)
  //

  auto const& geom = *TestEnv.Provider<geo::GeometryCore>();
  auto const& wireReadoutGeom = *TestEnv.Provider<geo::WireReadoutGeom>();
  auto const clock_data = TestEnv.Provider<detinfo::DetectorClocks>()->DataForJob();
  auto const& detp = *TestEnv.Provider<detinfo::DetectorProperties>();

  auto const driftVelocity = detp.DriftVelocity();
  auto const TDCtick = sampling_rate(clock_data);
  unsigned int const nWaveformTicks = detp.NumberTimeSamples();
  unsigned int const nReadoutWindowTicks = detp.ReadOutWindowSize();

  mf::LogVerbatim("detp_test") << "Electric field in the active volume: " << detp.Efield()
                               << " kV/cm"
                               << "\nSampling rate:       " << TDCtick << " ns"
                               << "\nArgon temperature:   " << detp.Temperature() << " K"
                               << "\nArgon density:       " << detp.Density() << " kg/dm^3 (at "
                               << detp.Temperature() << " K)"
                               << "\nArgon density:       " << detp.Density(87.0)
                               << " kg/dm^3 (at 87 K)"
                               << "\nDrift velocity:      " << driftVelocity << " cm/us"
                               << "\nReadout window:      " << nReadoutWindowTicks << " ticks ("
                               << (nReadoutWindowTicks * TDCtick / 1000) << " us)"
                               << "\nTPC waveform length: " << nWaveformTicks << " ticks ("
                               << (nWaveformTicks * TDCtick / 1000) << " us)";

  // accumulate the plane IDs; needed just for table formatting
  unsigned int headerColWidth = 0U;
  for (auto planeID : wireReadoutGeom.Iterate<geo::PlaneID>()) {
    auto const l = std::string(planeID).length();
    if (headerColWidth < l) headerColWidth = l;
  }

  // print drift distances
  // collect all drift distances, and check whether they are all equal
  bool allSameDrift = true;
  lar::util::RealComparisons<double> check(1.0); // 1 cm tolerance
  geo::TPCDataContainer<double> driftDistances{geom.Ncryostats(), geom.MaxTPCs()};
  for (auto const& tpc : geom.Iterate<geo::TPCGeo>()) {
    auto const driftDistance = tpc.DriftDistance();
    driftDistances[tpc.ID()] = driftDistance;
    allSameDrift = allSameDrift & check.equal(driftDistance, driftDistances.first());
  } // for

  if (allSameDrift) {
    // print drift distance
    auto const driftDistance = driftDistances.first();
    auto const driftTime = driftDistance / driftVelocity;
    mf::LogVerbatim("detp_test") << "Drift distance:    " << driftDistance << " cm"
                                 << "\nDrift time:        " << driftTime << " us, "
                                 << (driftTime / (TDCtick / 1000.)) << " ticks";
  }
  else {
    // print drift distance table header
    mf::LogVerbatim log("detp_test");
    std::array<unsigned int, 4U> const columnSizes = {{headerColWidth, 9U, 13U, 7U}};
    log << std::setw(columnSizes[0]) << "Drift:"
        << " | " << std::setw(columnSizes[1]) << "time [us]"
        << " | " << std::setw(columnSizes[2]) << "distance [cm]"
        << " | " << std::setw(columnSizes[3]) << "ticks";

    // print drift distances by TPC
    for (auto const& TPCID : geom.Iterate<geo::TPCID>()) {
      auto const driftDistance = driftDistances[TPCID];
      auto const driftTime = driftDistance / driftVelocity;
      log << "\n"
          << std::setw(columnSizes[0]) << TPCID << " | " << std::setw(columnSizes[1]) << driftTime
          << " | " << std::setw(columnSizes[2]) << driftDistance << " | "
          << std::setw(columnSizes[3]) << (driftTime / (TDCtick / 1000.));
    } // for TPC
  }

  // 4. And finally we cross fingers.
  if (nErrors > 0) { mf::LogError("detp_test") << nErrors << " errors detected!"; }

  return nErrors;
} // main()
