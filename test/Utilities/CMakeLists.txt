set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare -Wno-error=pedantic")
cet_test(constexpr_math_test USE_BOOST_UNIT)
cet_test(quantities_test USE_BOOST_UNIT)
cet_test(quantities_fhicl_test USE_BOOST_UNIT
  LIBRARIES
    ${FHICLCPP}
    ${CETLIB_EXCEPT}
  )
cet_test(intervals_test USE_BOOST_UNIT)
cet_test(intervals_fhicl_test USE_BOOST_UNIT
  LIBRARIES
    ${FHICLCPP}
    ${CETLIB_EXCEPT}
  )
cet_test(space_test USE_BOOST_UNIT)
cet_test(frequency_test USE_BOOST_UNIT)
cet_test(energy_test USE_BOOST_UNIT)
cet_test(datasize_test USE_BOOST_UNIT)
cet_test(StatCollector_test USE_BOOST_UNIT)
cet_test(MappedContainer_test USE_BOOST_UNIT)
cet_test(MultipleChoiceSelection_test USE_BOOST_UNIT)

install_fhicl()
install_source()
