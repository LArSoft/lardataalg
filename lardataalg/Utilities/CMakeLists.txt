cet_make_library(LIBRARY_NAME UtilitiesHeaders INTERFACE
  SOURCE
  constexpr_math.h
  intervals_fhicl.h
  intervals.h
  MappedContainer.h
  MultipleChoiceSelection.h
  quantities_fhicl.h
  quantities.h
  StatCollector.h
  LIBRARIES INTERFACE
  larcorealg::headers
  fhiclcpp::fhiclcpp
  Boost::boost
)

install_headers(SUBDIRS quantities)
install_source(SUBDIRS quantities)
