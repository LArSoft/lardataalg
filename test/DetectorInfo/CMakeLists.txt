cet_transitive_paths(FHICL_DIR BINARY IN_TREE)
cet_test_env_prepend(FHICL_FILE_PATH "." ${TRANSITIVE_PATHS_WITH_FHICL_DIR})

cet_transitive_paths(GDML_DIR BINARY IN_TREE)
cet_test_env_prepend(FW_SEARCH_PATH ${TRANSITIVE_PATHS_WITH_GDML_DIR})

cet_test(LArPropertiesStandard_test
  DATAFILES lartest_standard.fcl
  TEST_ARGS ./lartest_standard.fcl
  INSTALL_BIN
  INSTALL_SOURCE
  LIBRARIES PRIVATE
  lardataalg::DetectorInfo_TestHelpers
  lardataalg::DetectorInfo
  larcorealg::unit_test_base
)

cet_test( LArPropertiesLArTPCdetector_test
  HANDBUILT
  DATAFILES lartest_lartpcdetector.fcl
  TEST_EXEC LArPropertiesStandard_test
  TEST_ARGS ./lartest_lartpcdetector.fcl
)

cet_test( LArPropertiesBo_test
  HANDBUILT
  DATAFILES lartest_bo.fcl
  TEST_EXEC LArPropertiesStandard_test
  TEST_ARGS ./lartest_bo.fcl
)

cet_test(DetectorClocksStandard_test
  DATAFILES clockstest_standard.fcl
  TEST_ARGS ./clockstest_standard.fcl
  INSTALL_BIN
  INSTALL_SOURCE
  LIBRARIES PRIVATE
  lardataalg::DetectorInfo_TestHelpers
  lardataalg::DetectorInfo
  larcorealg::TestUtils
)

cet_test(DetectorTimingTypes_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::disable_boost_fpc_tolerance
  lardataalg::DetectorInfo
  lardataalg::UtilitiesHeaders
  larcorealg::CoreUtils
)

cet_test(DetectorTimingsStandard_test
  DATAFILES clockstest_standard.fcl
  TEST_ARGS ./clockstest_standard.fcl
  INSTALL_BIN
  INSTALL_SOURCE
  LIBRARIES PRIVATE
  lardataalg::DetectorInfo_TestHelpers
  lardataalg::DetectorInfo
  lardataalg::UtilitiesHeaders
  larcorealg::CoreUtils
  messagefacility::MF_MessageLogger
)

cet_test( DetectorClocksLArTPCdetector_test
  HANDBUILT
  DATAFILES clockstest_lartpcdetector.fcl
  TEST_EXEC DetectorClocksStandard_test
  TEST_ARGS ./clockstest_lartpcdetector.fcl
)

cet_test( DetectorClocksBo_test
  HANDBUILT
  DATAFILES clockstest_bo.fcl
  TEST_EXEC DetectorClocksStandard_test
  TEST_ARGS ./clockstest_bo.fcl
)

cet_test( DetectorClocksCSU40L_test
  HANDBUILT
  DATAFILES clockstest_csu40l.fcl
  TEST_EXEC DetectorClocksStandard_test
  TEST_ARGS ./clockstest_csu40l.fcl
)

cet_make_exec(NAME DetectorPropertiesStandard_test
  LIBRARIES PRIVATE
  lardataalg::DetectorInfo_TestHelpers
  lardataalg::DetectorInfo
  larcorealg::geometry_unit_test_base
  larcorealg::Geometry
)

cet_test( DetectorPropertiesLArTPCdetector_test
  HANDBUILT
  DATAFILES dettest_lartpcdetector.fcl
  TEST_EXEC DetectorPropertiesStandard_test
  TEST_ARGS ./dettest_lartpcdetector.fcl
)

# this test requires larcore/Geometry/geometry_bo.fcl
##cet_test( DetectorPropertiesBo_test
##  HANDBUILT
##  DATAFILES dettest_bo.fcl
##  TEST_EXEC DetectorPropertiesStandard_test
##  TEST_ARGS ./dettest_bo.fcl
##)

install_source()
install_fhicl()
