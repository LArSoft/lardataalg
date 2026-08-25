/**
 * @file  lardataalg/Utilities/TrackingTypeUtils.h
 * @brief A few utility functions dealing with `tracking` data types.
 * @date  October 30, 2025
 *
 * The original content of this library was moved from `lardataobj` in LArSoft `v10_12_01`.
 */

#ifndef LARDATAALG_UTILITIES_TRACKINGTYPEUTILS_H
#define LARDATAALG_UTILITIES_TRACKINGTYPEUTILS_H

// LArSoft libraries
#include "larcorealg/Geometry/geo_vectors_utils.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"
#include "lardataobj/RecoBase/TrackingTypes.h"

// ROOT libraries
#include "TVector3.h"

namespace recob::tracking {

  /// Tools to aide the conversion from TVector3 to Point_t and Vector_t
  /// @{

  /// Copies a vector of points of type `From` into a new vector of points of type `To`.
  template <typename To, typename From>
  std::vector<To> convertVec(std::vector<From> const& in)
  {
    std::vector<To> out;
    out.reserve(in.size());
    for (auto& i : in)
      out.push_back(To(i));
    return out;
  }

  /// Returns a copy of the point-like object as a `recob::tracking::Point_t`.
  template <typename Point>
  Point_t toPoint(Point const& p)
  {
    return geo::vect::convertTo<Point_t>(p);
  }

  /// Copies a vector of points of type `Point` into a new vector of `recob::tracking::Point_t`.
  template <typename Point>
  std::vector<Point_t> convertCollToPoint(std::vector<Point> const& coll)
  {
    return geo::vect::convertCollTo<Point_t>(coll);
  }

  /// Returns a copy of the vector-like object as a `recob::tracking::Vector_t`.
  template <typename Vector>
  Vector_t toVector(Vector const& p)
  {
    return geo::vect::convertTo<Vector_t>(p);
  }

  /// Copies a vector of vector-like objects of type `Vector` into a new vector of `recob::tracking::Vector_t`.
  template <typename Vector>
  std::vector<Vector_t> convertCollToVector(std::vector<Vector> const& coll)
  {
    return geo::vect::convertCollTo<Vector_t>(coll);
  }

  /// @}

}

#endif // LARDATAALG_UTILITIES_TRACKINGTYPEUTILS_H
