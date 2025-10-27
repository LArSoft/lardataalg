/**
 * @file   lardataalg/Utilities/details/quantity_products.h
 * @brief  Infrastructure to describe products of quantity objects
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   October 25, 2025
 * @see    lardataalg/Utilities/quantities.h
 *
 * Purpose
 * --------
 *
 * The idea here is that with a simple declaration by the users of a relation
 * between three units, e.g. that Volt is product of Ohm and Ampere units,
 * mathematical operators are enabled to allow the two products (commutative)
 * and the two ratios between quantity objects of those types.
 *
 * The declaration is performed via a preprocessor macro,
 * `DECLARE_PRODUCT_INFO()`.
 *
 * Because C++ operators are binary, three-unit relations are in principle
 * enough to describe all is needed. In case of relations between two groups
 * of units of arbitrary size (for example, a one-to-three like W &Omega; = V V)
 * intermediate opaque quantities may be needed (in the example, to express
 * watts W = ((V &Omega;) V) we may need an intermediate X = (V &Omega;), which
 * may be an actual quantity, like ampere in this case, or just a bridge to the
 * next product W = X V). The automatic implementation is very tricky, if at all
 * possible, due to the increasing combinatorics. If such use case is really
 * needed, the current workaround is to declare all the intermediate quantities
 * needed as actual quantities and then their relations.
 *
 *
 * Implementation details
 * -----------------------
 *
 * The core element is an object that describes the relation between three
 * quantities (A times B equal to P). This is a template specialization of
 * `util::quantities::concepts::details::ProductDataType`, which includes the
 * type of the product of its arguments:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * template <> struct UnitProductResult<A, B, +1> {
 *   using type = P;
 * };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * The `+1` is the exponent of `B`, that means that `A` and `B` are being
 * multiplied (`A * B = P`). For the ratio relationship (`A / B = P`), `-1`
 * needs to be used instead.
 * A complete set of relations should be declared: A * B = P, B * A = P,
 * P / A = B and P / B = A. The macro `DECLARE_PRODUCT_INFO()` does that.
 *
 *
 *
 *
 */

#ifndef LARDATAALG_UTILITIES_QUANTITIES_H
#error                                                                                             \
  "lardataalg/Utilities/details/quantity_products.h should only be included by lardataalg/Utilities/quantities.h"
#endif

#ifndef LARDATAALG_UTILITIES_QUANTITIES_QUANTITY_PRODUCTS_H
#define LARDATAALG_UTILITIES_QUANTITIES_QUANTITY_PRODUCTS_H

// C/C++ standard libraries
#include <string_view>
#include <type_traits>
#include <utility> // std::declval()

namespace util::quantities::concepts::details {

  // ---------------------------------------------------------------------------
  /**
   * @brief Type describing a product relation between three units.
   * @tparam UA type of the first unit in the product
   * @tparam UB type of the second unit in the product
   * @tparam BExp (default: `+1`) exponent of `UB` in the product
   * @see `UTIL_QUANTITIES_UNITPRODUCT()`
   *
   * The existence of a class specialization of this type declares a relation
   * between three base units. If `BExp` is `+1`, the relation is `UA * UB = P`,
   * while if `BExp` is `-1` the relation is `UA / UB = P`.
   * The types `UA` and `UB` are specified (only) as template parameters, while
   * the type `P` must be defined in the body of the template specialization,
   * as a type called... `type`:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * template<> struct ProductType<UA, UB, +1> {
   *   using type = P;
   * };
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * For example:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * template<> struct ProductType<Ampere, Ohm, +1> {
   *   using type = Volt;
   * };
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * Note that this does not implies related relationships (like
   * `Ohm` * `Ampere` = `Volt`, `Volt` / `Ohm` = `Ampere` etc.), which need to
   * be explicitly specified (that is `UTIL_QUANTITIES_UNITPRODUCT()` for you).
   *
   */
  template <typename UA, typename UB, int BExp = +1>
  struct UnitProductResult;

  // ---------------------------------------------------------------------------
  // The dance to access `UnitProductResult` properties is open...

  /// Direct access to the base unit of the product of `QA` and `QB`
  /// (ratio if `BExp == -1`).
  template <typename QA, typename QB, int BExp = +1>
  using BaseUnitProductResult_t =
    typename UnitProductResult<base_unit_of<QA>, base_unit_of<QB>, BExp>::type;

  /// Direct access to the base unit of the ratio of `QN` and `QD`.
  template <typename QN, typename QD>
  using BaseUnitRatioResult_t = BaseUnitProductResult_t<QN, QD, -1>;

  /// Trait: there is a product/ratio defined between the units of `QA` and `QB`.
  template <typename QA, typename QB, int BExp = +1, typename = void>
  struct CanMultiplyQuantities : std::false_type {};

  /// Implementation of `CanMultiplyQuantities` for supported quantities.
  template <typename QA, typename QB, int BExp>
  struct CanMultiplyQuantities<QA, QB, BExp, std::void_t<BaseUnitProductResult_t<QA, QB, BExp>>>
    : std::true_type {};

  /// Shortcut trait to query of quantity ratio.
  template <typename QN, typename QD>
  using CanDivideQuantities = CanMultiplyQuantities<QN, QD, -1>;

  /**
   * @brief Full set of traits for the product between two quantities.
   * @tparam QA first quantity to be multiplied
   * @tparam QB second quantity to be multiplied (or denominator if `BExp == -1`)
   * @tparam BExp (default: `+1`) `+1` for product, `-1` for ratio
   *
   * See `QuantitiesProductResult_t` and `QuantitiesRatioResult_t` for details.
   */
  template <typename QA, typename QB, int BExp = +1, typename = void>
  struct QuantitiesProductResult;

  /// Implementation of `QuantitiesProductResult` when the product is valid.
  template <typename QA, typename QB>
  struct QuantitiesProductResult<QA,
                                 QB,
                                 +1,
                                 std::enable_if_t<CanMultiplyQuantities<QA, QB, +1>::value>> {
    // I needed to separate division and product because GCC 12.1 instantiating
    // std::conditional<false, pico*nano, pico/nano> would complain about
    // the true branch being out of range...
    using baseunit_t = BaseUnitProductResult_t<QA, QB, +1>;
    using ratio_t = std::ratio_multiply<typename QA::unit_t::ratio, typename QB::unit_t::ratio>;
    using value_t =
      decltype(std::declval<typename QA::value_t>() * std::declval<typename QB::value_t>());
    using type = Quantity<ScaledUnit<baseunit_t, ratio_t>, value_t>;
  }; // QuantitiesProductResult

  /// Implementation of `QuantitiesProductResult` when the ratio is valid.
  template <typename QA, typename QB>
  struct QuantitiesProductResult<QA,
                                 QB,
                                 -1,
                                 std::enable_if_t<CanMultiplyQuantities<QA, QB, -1>::value>> {
    using baseunit_t = BaseUnitProductResult_t<QA, QB, -1>;
    using ratio_t = std::ratio_divide<typename QA::unit_t::ratio, typename QB::unit_t::ratio>;
    using value_t =
      decltype(std::declval<typename QA::value_t>() / std::declval<typename QB::value_t>());
    using type = Quantity<ScaledUnit<baseunit_t, ratio_t>, value_t>;
  }; // QuantitiesProductResult

  /// Shortcut trait to query of quantity ratio result information.
  template <typename QN, typename QD>
  using QuantitiesRatioResult = QuantitiesProductResult<QN, QD, -1>;

  /**
   * @brief Full set of traits for the product between two quantities.
   * @tparam QA first quantity to be multiplied
   * @tparam QB second quantity to be multiplied (or denominator if `BExp == -1`)
   * @tparam BExp (default: `+1`) `+1` for product, `-1` for ratio
   *
   * The class is defined only if the requested operation (`QA*QB` or `QA/QB`,
   * depending on `BExp`) is valid (i.e. if it was declared with
   * `UTIL_QUANTITIES_UNITPRODUCT()` or equivalent).
   *
   * The information of the result of the operation includes:
   *  * `type`: type of the `Quantity` result of the product or ratio.
   *  * `baseunit_t`: type of the base unit of the result.
   *  * `ratio_t`: the scaling of the base unit.
   *  * `value_t`: type of the value contained in the quantity.
   *
   */
  template <typename QA, typename QB, int BExp = +1>
  using QuantitiesProductResult_t = typename QuantitiesProductResult<QA, QB, BExp>::type;

  /// Shortcut trait to query of quantity ratio `QA/QB`.
  /// @see `QuantitiesProductResult_t`
  template <typename QA, typename QB>
  using QuantitiesRatioResult_t = QuantitiesProductResult_t<QA, QB, -1>;

  // ---------------------------------------------------------------------------

} // namespace util::quantities::concepts::details

namespace util::quantities::concepts {

  // ---------------------------------------------------------------------------
  /// Evaluates to `T` if that's a valid type, otherwise fails to compile.
  template <typename T>
  using only_if_exists_t = std::enable_if_t<std::is_same_v<std::void_t<T>, void>, T>;

  // ---------------------------------------------------------------------------
  /// Returns the product of two quantity objects, if that's declared.
  /// @see `UTIL_QUANTITIES_UNITPRODUCT()`
  template <typename QA, typename QB>
  constexpr only_if_exists_t<details::QuantitiesProductResult_t<QA, QB>> operator*(QA a, QB b)
  {

    using ResultInfo = details::QuantitiesProductResult<QA, QB>;
    using value_t = typename ResultInfo::value_t;
    using quantity_t = typename ResultInfo::type;

    return quantity_t{static_cast<value_t>(a.value() * b.value())};

  } // operator* (quantity, quantity)

  /// Returns the ratio of two quantity objects, if that's declared.
  /// @see `UTIL_QUANTITIES_UNITPRODUCT()`
  template <typename QN, typename QD>
  constexpr only_if_exists_t<details::QuantitiesRatioResult_t<QN, QD>> operator/(QN num, QD den)
  {

    using ResultInfo = details::QuantitiesRatioResult<QN, QD>;
    using value_t = typename ResultInfo::value_t;
    using quantity_t = typename ResultInfo::type;

    return quantity_t{static_cast<value_t>(num.value() / den.value())};

  } // operator/ (quantity, quantity)

  // ---------------------------------------------------------------------------

} // namespace util::quantities::concepts

// -----------------------------------------------------------------------------
/**
 * @brief Declares a relation between units.
 * @tparam UA type of the first unit in the product
 * @tparam UB type of the second unit in the product
 * @tparam UP type of the unit of the product `UA * UB`
 *
 * It defines `ProductType` template specialization like:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * template<> struct ProductType<UA, UB> { using type = UP; };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * that describe the four relations between the three units (`UA * UB = UP`,
 * `UB * UA = UP`, `UP / UA = UB`, `UP / UB = UA`).
 *
 * It requires all the operands to be base unit types (no `ScaledUnit`),
 * and that the product unit is neither of the factor ones.
 */
#define UTIL_QUANTITIES_UNITPRODUCT(UA, UB, UP)                                                    \
  namespace util::quantities::concepts::details {                                                  \
    static_assert(is_base_unit_v<UA>,                                                              \
                  "The first type does not meet base unit class requirements.");                   \
    static_assert(is_base_unit_v<UB>,                                                              \
                  "The second type does not meet base unit class requirements.");                  \
    static_assert(is_base_unit_v<UP>,                                                              \
                  "The third type does not meet base unit class requirements.");                   \
    static_assert(!std::is_same_v<UP, UA>, "A factor can't have the same unit as the product.");   \
    static_assert(!std::is_same_v<UP, UB>, "B factor can't have the same unit as the product.");   \
    template <>                                                                                    \
    struct UnitProductResult<UA, UB, +1> {                                                         \
      using type = UP;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitProductResult<UB, UA, +1> {                                                         \
      using type = UP;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitProductResult<UP, UA, -1> {                                                         \
      using type = UB;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitProductResult<UP, UB, -1> {                                                         \
      using type = UA;                                                                             \
    };                                                                                             \
  }

// -----------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_QUANTITY_PRODUCTS_H
