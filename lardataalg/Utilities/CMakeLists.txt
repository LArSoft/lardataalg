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
  details/quantity_products.h
  LIBRARIES INTERFACE
  larcorealg::headers
  fhiclcpp::fhiclcpp
  Boost::boost
)

install_headers(SUBDIRS quantities details)
install_source(SUBDIRS quantities details)
