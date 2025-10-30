#ifndef LARDATAALG_UTILITIES_TRACKINGTYPEUTILS_H
#define LARDATAALG_UTILITIES_TRACKINGTYPEUTILS_H

// LArSoft libraries
#include "larcorealg/Geometry/geo_vectors_utils.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"
#include "lardataobj/RecoBase/TrackingTypes.h"

// ROOT libraries
#include "TVector3.h"

namespace recob::tracking {

  /// @{
  /// Tools to aide the conversion from TVector3 to Point_t and Vector_t
  template <typename To, typename From>
  std::vector<To> convertVec(std::vector<From> const& in)
  {
    std::vector<To> out;
    out.reserve(in.size());
    for (auto& i : in)
      out.push_back(To(i));
    return out;
  }
  template <typename From>
  std::vector<TVector3> convertVecPointToTVec3(std::vector<From> const& in)
  {
    std::vector<TVector3> out;
    out.reserve(in.size());
    for (auto& i : in)
      out.push_back(TVector3(i.X(), i.Y(), i.Z()));
    return out;
  }
  template <typename Point>
  Point_t toPoint(Point const& p)
  {
    return geo::vect::convertTo<Point_t>(p);
  }
  template <typename Point>
  std::vector<Point_t> convertCollToPoint(std::vector<Point> const& coll)
  {
    return geo::vect::convertCollTo<Point_t>(coll);
  }
  template <typename Vector>
  Vector_t toVector(Vector const& p)
  {
    return geo::vect::convertTo<Vector_t>(p);
  }
  template <typename Vector>
  std::vector<Vector_t> convertCollToVector(std::vector<Vector> const& coll)
  {
    return geo::vect::convertCollTo<Vector_t>(coll);
  }
  /// @}

}

#endif // LARDATAALG_UTILITIES_TRACKINGTYPEUTILS_H
