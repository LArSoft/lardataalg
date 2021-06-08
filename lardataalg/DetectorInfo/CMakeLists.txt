cet_make(LIBRARIES larcorealg_Geometry
                   larcorealg_CoreUtils
                   messagefacility::MF_MessageLogger
                   cetlib_except
                   fhiclcpp::fhiclcpp fhiclcpp::types
                   ROOT::Core
                   ROOT::Hist)

install_headers()
install_fhicl()
