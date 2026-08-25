cet_make_library(LIBRARY_NAME disable_boost_fpc_tolerance INTERFACE
  USE_BOOST_UNIT NO_INSTALL
  SOURCE disable_boost_fpc_tolerance.hpp
  LIBRARIES INTERFACE
  lardataalg::UtilitiesHeaders
  Boost::unit_test_framework
)

cet_test(constexpr_math_test
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(quantities_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::disable_boost_fpc_tolerance
  lardataalg::UtilitiesHeaders
)

cet_test(quantities_fhicl_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::disable_boost_fpc_tolerance
  lardataalg::UtilitiesHeaders
  fhiclcpp::fhiclcpp
  fhiclcpp::types
  cetlib_except::cetlib_except
)

cet_test(intervals_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::disable_boost_fpc_tolerance
  lardataalg::UtilitiesHeaders
  larcorealg::CoreUtils
)

cet_test(intervals_fhicl_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::disable_boost_fpc_tolerance
  lardataalg::UtilitiesHeaders
  fhiclcpp::types
  fhiclcpp::fhiclcpp
)

cet_test(space_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(frequency_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(energy_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(electromagnetism_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(datasize_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(StatCollector_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

cet_test(MappedContainer_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
  larcorealg::headers
)

cet_test(MultipleChoiceSelection_test USE_BOOST_UNIT
  LIBRARIES PRIVATE
  lardataalg::UtilitiesHeaders
)

install_fhicl()
install_source()
