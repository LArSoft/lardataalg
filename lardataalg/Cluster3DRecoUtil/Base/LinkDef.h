//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


#pragma link C++ namespace cluster3D+;

#pragma link C++ class ::cluster3D::Point3D+;
#pragma link C++ class std::vector<::cluster3D::Point3D>+;


#pragma link C++ class cluster3D::cluster3D_params+;
#pragma link C++ class std::vector<::cluster3D::cluster3D_params>+;

#pragma link C++ class cluster3D::Cluster3DParamsAlg+;

#pragma link C++ class cluster3D::CRU3DException+;
#pragma link C++ class cluster3D::CRU3DHelper+;


//#pragma link C++ class cluster::DefaultParamsAlg+;
//ADD_NEW_CLASS ... do not change this line
#endif




















