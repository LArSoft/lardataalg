set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare -Wno-pedantic")
cet_test( LArPropertiesStandard_test
  LIBRARIES
  lardataalg_DetectorInfo
  cetlib
  DATAFILES lartest_standard.fcl
  TEST_ARGS ./lartest_standard.fcl
  INSTALL_BIN
  INSTALL_SOURCE
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


cet_test( DetectorClocksStandard_test
  LIBRARIES
  lardataalg_DetectorInfo
  cetlib
  DATAFILES clockstest_standard.fcl
  TEST_ARGS ./clockstest_standard.fcl
  INSTALL_BIN
  INSTALL_SOURCE
)

cet_test( DetectorTimingTypes_test USE_BOOST_UNIT)

cet_test( DetectorTimingsStandard_test
  LIBRARIES
  lardataalg_DetectorInfo
  cetlib
  DATAFILES clockstest_standard.fcl
  TEST_ARGS ./clockstest_standard.fcl
  INSTALL_BIN
  INSTALL_SOURCE
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

cet_make_exec( DetectorPropertiesStandard_test
  LIBRARIES
  lardataalg_DetectorInfo
  cetlib
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

# set FHICL_FILE_PATH and FW_SEARCH_PATH for the case where building without mrb
# this is not generic, we take advantage of knowing the subdirectories for the fcl and gdml files
set( mrb_build_dir $ENV{MRB_BUILDDIR} )
if( NOT mrb_build_dir )
  #message(STATUS "MRB_BUILDDIR is undefined, set FHICL_FILE_PATH and FW_SEARCH_PATH")
  set_tests_properties( LArPropertiesStandard_test
                        LArPropertiesLArTPCdetector_test
                        LArPropertiesBo_test
                        DetectorClocksStandard_test
                        DetectorClocksLArTPCdetector_test
                        DetectorClocksBo_test
                        DetectorClocksCSU40L_test
                        DetectorPropertiesLArTPCdetector_test
                        PROPERTIES ENVIRONMENT
                        "FHICL_FILE_PATH=.:${CMAKE_BINARY_DIR}/job:${larcorealg_fcl_dir};FW_SEARCH_PATH=${CMAKE_BINARY_DIR}/gdml:${larcorealg_gdml_dir}" )
endif()
install_fhicl()
