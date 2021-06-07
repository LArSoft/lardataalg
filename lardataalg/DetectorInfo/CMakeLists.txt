cet_make(LIBRARIES larcorealg_Geometry
                   larcorealg_CoreUtils
                   ${MF_MESSAGELOGGER}
                   cetlib_except
                   ${FHICLCPP}
                   ROOT::Core
                   ROOT::Hist)

install_headers()
install_fhicl()
