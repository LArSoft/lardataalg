cet_make_library(SOURCE
  DetectorClocksStandard.cxx
  DetectorPropertiesData.cc
  DetectorPropertiesStandard.cxx
  ElecClock.cxx
  LArPropertiesStandard.cxx
  RunHistoryStandard.cxx
  LIBRARIES
  PUBLIC
  lardataobj::RawData
  larcorealg::Geometry
  larcoreobj::SimpleTypesAndConstants
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::types
  fhiclcpp::fhiclcpp
  cetlib_except::cetlib_except
  PRIVATE
  cetlib::cetlib
  range-v3::range-v3
  ROOT::Core
  ROOT::Hist
)

cet_make_library(LIBRARY_NAME DetectorInfo_TestHelpers INTERFACE
  SOURCE
  DetectorClocksStandardTestHelpers.h
  DetectorPropertiesStandardTestHelpers.h
  LArPropertiesStandardTestHelpers.h
  LIBRARIES INTERFACE
  lardataalg::DetectorInfo
  larcorealg::TestUtils
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
)

install_headers()
install_fhicl()
install_source()
