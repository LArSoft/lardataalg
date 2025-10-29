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
 * template <> struct UnitBinaryOpResult<A, B, +1> {
 *   using type = P;
 * };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * The `+1` is the exponent of `B`, that means that `A` and `B` are being
 * multiplied (`A * B = P`). For the ratio relationship (`A / B = P`), `-1`
 * needs to be used instead.
 * A complete set of relations should be declared: A * B = P, B * A = P,
 * P / A = B and P / B = A. The macro `UTIL_QUANTITIES_UNITPRODUCT()` does that.
 *
 *
 * Naming conventions
 * -------------------
 *
 * In this file:
 *  * data structures (`struct`, `class`) are one-word camelcase;
 *  * type trait aliases (`using`, also when templated) have
 *    underscore-separated, all small letter identifiers, and suffix `_t`;
 *  * boolean traits (`constexpr bool`) have the same conventions as type traits
 *    but with suffix `_v` instead of `_t`;
 *  * macros have all capital, underscore-separated names starting with
 *    `UTIL_QUANTITIES_`.
 *
 * Note that other files (noticeably, `quantities.h`) are not bound to these
 * rules.
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

// -----------------------------------------------------------------------------
namespace util::quantities::units {

  using namespace std::string_view_literals;

  /// Special unit representing a pure number (of unspecified type).
  struct Unity : public concepts::UnitBase {

    /// Symbol of the unit (e.g. "A").
    static constexpr std::string_view symbol = "1"sv;
    /// Long name of unit (e.g. "ampere").
    static constexpr std::string_view name = "unity"sv;
  };

} // namespace util::quantities::units

namespace util::quantities::concepts::details {

  // ---------------------------------------------------------------------------
  /// Special quantity-like object representing a pure number of specified type.
  template <typename T = double, typename R = std::ratio<1>, typename = void>
  struct TypedUnity;

  /// Trait: `T` is a `TypedUnit` template specialization.
  template <typename T>
  struct IsTypedUnit : std::false_type {};

  /// Implementation of `IsTypedUnit` for `TypedUnit` types.
  template <typename T>
  struct IsTypedUnit<TypedUnity<T>> : std::true_type {};

  /// Implementation of `TypedUnit` (preventing recursiveness).
  template <typename T, typename R>
  struct TypedUnity<T, R, std::enable_if_t<!IsTypedUnit<T>::value>> {

  public:
    // mock-up of needed Quantity interface:
    using value_t = T;
    struct unit_t { // ScaledUnit mockup
      using baseunit_t = units::Unity;
      using ratio = R;
      using prefix_t = Prefix<ratio>; ///< The prefix of the unit.
    };
    using quantity_t = value_t; // special: decays to a plain number
    using baseunit_t = typename unit_t::baseunit_t;

    constexpr TypedUnity(value_t value = static_cast<value_t>(1)) noexcept : fValue{value} {}

    /// Returns the contained value (always just the ratio).
    constexpr value_t value() const { return fValue; }

    template <typename OU>
    static constexpr bool sameBaseUnitAs()
    {
      return std::is_same<baseunit_t, base_unit_of<OU>>();
    }

  private:
    value_t fValue; ///< Value of the

  }; // TypedUnity

  /**
   * @brief Type describing a binary relation between three units.
   * @tparam UA type of the first unit in the product
   * @tparam UB type of the second unit in the product
   * @tparam OpCat type of operation (e.g. '*', '/')
   * @see `UTIL_QUANTITIES_UNITPRODUCT()`
   *
   * The existence of a class specialization of this type declares a relation
   * between three base units: `UA op UB = P`. The "op" operation is encoded
   * in `OpCat`, but this type does not define how the operation works: it only
   * sanctions the legitimacy of that operation between types and declares its
   * result type.
   * As an example, `OpCat` value `'*'` is here used to describe a product,
   * `UA * UB = P`, and value `'/'` is used to describe the ratio `UA / UB = P`.
   *
   * The types `UA` and `UB` are specified (only) as template parameters, while
   * the type `P` must be defined in the body of the template specialization,
   * as a type called... `type`:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * template<> struct BinaryOpType<UA, UB, '*'> {
   *   using type = P;
   * };
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * For example:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * template<> struct BinaryOpType<Ampere, Ohm, '*'> {
   *   using type = Volt;
   * };
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * Note that this does not declares related relationships (like
   * `Ohm` * `Ampere` = `Volt`, `Volt` / `Ohm` = `Ampere` etc.), which need to
   * be explicitly specified (that is `UTIL_QUANTITIES_UNITPRODUCT()` for you).
   *
   */
  template <typename UA, typename UB, char OpCat>
  struct UnitBinaryOpResult;

  /// Type trait: `T` is the type `units::Unity`.
  template <typename T>
  constexpr bool is_unity_v = std::is_same_v<T, units::Unity>;

  /// Trait: `T` is either a `Quantity`) or a `TypedUnity` template instance.
  template <typename T>
  constexpr bool is_typed_unity_or_quantity_v = IsTypedUnit<T>::value || is_quantity_v<T>;

  /**
   * Trait: `T` is either `Quantity` or a `TypedUnity` instance, `U` is quantity,
   * and they do not share the same base unit.
   *
   * This is useful to enable "special" operations declared by
   * `UnitBinaryOpResult` instances.
   * The request of not sharing the base unit originates by the fact that
   * typically same-unit operations are implemented directly in the `Quantity`
   * object, and it is necessary to avoid collisions with more generic/special
   * implementations.
   * Preventing the second type from being a unit type limits the usefulness of
   * this trait, excluding hypothetical operations where the second operand is
   * a constant and the result is not a quantity with the same base unit as the
   * first type. However, as of LArSoft v10_10, no operations of this type are
   * implemented nor known. There _is_ one operation of that type implemented
   * with the first operant being a constant and the result being a different
   * type, that is the "inverse" operation `'/'` (for example, the inverse
   * `1 / hz` of a frequency giving a time quantity).
   */
  template <typename T, typename U>
  constexpr bool are_two_different_typed_unities_or_quantities_v =
    is_typed_unity_or_quantity_v<T> && is_quantity_v<U> && !(T::template sameBaseUnitAs<U>());

  /// Trait: all template types are `Quantity` or `TypedUnity` objects.
  template <typename... Ts>
  constexpr bool all_quantities_or_types_units_v =
    (true && ... && is_typed_unity_or_quantity_v<Ts>);

  /// Whether the binary operation is acting on quantities and is "special"
  /// (meaning that is supported only by `UnitBinaryOpResult` registered types).
  template <typename OpA, typename OpB, char OpCat, typename = void>
  struct IsSpecialQuantityOperation : std::false_type {};

  /**
   * Implementation for "special" operations on valid quantity-like operands.
   *
   * Operations that are not "special" include division between same-unit
   * quantities and product of a quantity and a unit.
   * These are supported directly in `Quantity` object.
   */
  template <typename OpA, typename OpB, char OpCat>
  struct IsSpecialQuantityOperation<OpA,
                                    OpB,
                                    OpCat,
                                    std::enable_if_t<all_quantities_or_types_units_v<OpA, OpB>>>
    : std::bool_constant<(OpCat != '/') || !(OpA::template sameBaseUnitAs<OpB>())> {};

  // ---------------------------------------------------------------------------
  // The dance to access `UnitBinaryOpResult` properties is open...

  /// Direct access to the base unit of the product of `QA` and `QB`
  /// (`OpCat`: `*` for product, `/` for ratio).
  template <typename QA, typename QB, char OpCat = '*'>
  using BaseUnitBinaryOpResult_t =
    typename UnitBinaryOpResult<base_unit_of<QA>, base_unit_of<QB>, OpCat>::type;

  /// Trait: there is an operation `OpCat` defined between the units of `QA` and `QB`.
  template <typename QA, typename QB, char OpCat, typename = void>
  struct CanCombineQuantities : std::false_type {};

  /// Implementation of `CanCombineQuantities` for supported quantities.
  template <typename QA, typename QB, char OpCat>
  struct CanCombineQuantities<QA, QB, OpCat, std::void_t<BaseUnitBinaryOpResult_t<QA, QB, OpCat>>>
    : std::true_type {};

  /**
   * @brief Full set of traits for a binary operation between two quantities.
   * @tparam QA first quantity (or `TypedUnit` type) operand
   * @tparam QB second quantity operand
   * @tparam OpCat operation category (e.g. `'*'` for product, `'/'` for ratio)
   *
   * See `quantity_binaryop_result_t` for details.
   */
  template <typename QA, typename QB, char OpCat = '*', typename = void>
  struct QuantityBinaryOpResult;

  /// Implementation of `QuantityBinaryOpResult` when the spacial operation is
  /// not registered.
  template <typename QA, typename QB, char OpCat>
  struct QuantityBinaryOpResult<QA,
                                QB,
                                OpCat,
                                std::enable_if_t<IsSpecialQuantityOperation<QA, QB, OpCat>::value &&
                                                 !(CanCombineQuantities<QA, QB, OpCat>::value)>> {
    // specific error messages for different known cases;
    // either way, a long list of compilation errors will follow these messages.
    static_assert((OpCat != '*'), "No product is registered between these quantities.");
    static_assert((OpCat != '/') || IsTypedUnit<QA>::value,
                  "No ratio is registered between the first and the second quantity.");
    static_assert((OpCat != '/') || !IsTypedUnit<QA>::value,
                  "No inversion operation is registered for this quantity.");
  };

  /// Implementation of `QuantityBinaryOpResult` for product (`OpCat` `'*'`) when it is valid.
  template <typename QA, typename QB>
  struct QuantityBinaryOpResult<QA,
                                QB,
                                '*',
                                std::enable_if_t<IsSpecialQuantityOperation<QA, QB, '*'>::value &&
                                                 CanCombineQuantities<QA, QB, '*'>::value>> {
    using baseunit_t = BaseUnitBinaryOpResult_t<QA, QB, '*'>;
    using ratio_t = std::ratio_multiply<typename QA::unit_t::ratio, typename QB::unit_t::ratio>;
    using value_t =
      decltype(std::declval<typename QA::value_t>() * std::declval<typename QB::value_t>());
    using type = std::conditional_t<is_unity_v<baseunit_t>,
                                    typename TypedUnity<value_t>::value_t,
                                    Quantity<ScaledUnit<baseunit_t, ratio_t>, value_t>>;
  }; // QuantityBinaryOpResult<'*'>

  /// Implementation of `QuantityBinaryOpResult` for ratio (`OpCat` `'/'`) when it is valid.
  template <typename QN, typename QD>
  struct QuantityBinaryOpResult<QN,
                                QD,
                                '/',
                                std::enable_if_t<CanCombineQuantities<QN, QD, '/'>::value>> {
    using baseunit_t = BaseUnitBinaryOpResult_t<QN, QD, '/'>;
    using ratio_t = std::ratio_divide<typename QN::unit_t::ratio, typename QD::unit_t::ratio>;
    using value_t =
      decltype(std::declval<typename QN::value_t>() / std::declval<typename QD::value_t>());
    using type = Quantity<ScaledUnit<baseunit_t, ratio_t>, value_t>;
  }; // QuantityBinaryOpResult<'/'>

  /**
   * @brief Full set of traits for a binary operation between two quantities.
   * @tparam QA first quantity (or `TypedUnit` type) operand
   * @tparam QB second quantity operand
   * @tparam OpCat operation category (e.g. `'*'` for product, `'/'` for ratio)
   *
   * The class is defined only if the requested operation is valid (i.e. if it
   * was declared with `UTIL_QUANTITIES_UNITPRODUCT()` or equivalent).
   *
   * The information of the result of the operation includes:
   *  * `type`: type of the `Quantity` result of the product or ratio.
   *  * `baseunit_t`: type of the base unit of the result.
   *  * `ratio_t`: the scaling of the base unit.
   *  * `value_t`: type of the value contained in the quantity.
   *
   * Each operation (including the operation category and its two operant types)
   * has an independent implementation. Operation categories include
   *  * `'*'`: product.
   *  * `'/'`: ratio.
   */
  template <typename QA, typename QB, char OpCat>
  using quantity_binaryop_result_t = typename QuantityBinaryOpResult<QA, QB, OpCat>::type;

  /// Trait: type of result of quantity combination, or `AltT` if none.
  /// Because sometimes we still need a type no matter what.
  /// @see `quantities_combo_result_or_void_t`
  template <typename QA, typename QB, char OpCat, typename AltT = void, typename = void>
  struct QuantitiesComboResultOrVoid {
    using type = AltT;
  };

  /// Implementation of `QuantitiesComboResultOrVoid` for sanctioned operations.
  template <typename QA, typename QB, char OpCat, typename AltT>
  struct QuantitiesComboResultOrVoid<QA,
                                     QB,
                                     OpCat,
                                     AltT,
                                     std::enable_if_t<CanCombineQuantities<QA, QB, OpCat>::value>> {
    using type = quantity_binaryop_result_t<QA, QB, OpCat>;
  };

  /// Trait: type of result of quantity combination, or `AltT` if none.
  /// Because sometimes we still need a type no matter what.
  template <typename QA, typename QB, char OpCat, typename AltT = void>
  using quantities_combo_result_or_void_t =
    typename QuantitiesComboResultOrVoid<QA, QB, OpCat, AltT>::type;

  /**
   * @brief Type of result of special operation between two quantities.
   * @tparam QA first quantity (or `TypedUnit` type) operand
   * @tparam QB second quantity operand
   * @tparam OpCat operation category (e.g. `'*'` for product, `'/'` for ratio)
   * @tparam ExtraReq (default: `true`) additional requirement (disabled if `false`)
   * @tparam AltT (default: `void`) special type for unsanctioned operations
   *
   * This construct can effectively have three states: disabled (that is,
   * incomplete type failing to compile), placeholder type (`AltT`) or special
   * operation result type.
   *
   * The concept is that trying to combine two quantities in an unsanctioned way
   * is a straight error, while combination that are not about quantities or
   * not special should not be not attempted at all.
   *
   * Using this type as a return value for a function allows to enable the
   * function only for quantity operations, but still passing through an
   * unsanctioned combination so that the implementation may report an error
   * message specific to the way the operation is not sanctioned.
   *
   * It yield a "disabled" state if QA o QB are not quantity-like
   * (in which case no special operation should be considered)
   * or if they can be combined with a "standard" operation `OpCat`
   * (in which case special operations should be ignored and not compete with the standard),
   * or if `ExtraReq` is `false` (there is a user-provided reason to ignore this);
   * type `AltT` (`void` by default) if they can't be combined with operation `OpCat`;
   * and the type of the result of the combination if they instead can.
   *
   * Implementation
   * ---------------
   *
   * This is implemented in two layers (`quantities_combo_result_or_void_t`)
   * because of the necessity to separate the decision on enable/disable from
   * the one on sanctioned/unsanctioned. Because I could not find a way to have
   * it both in single place.
   */
  template <typename QA, typename QB, char OpCat, bool ExtraReq = true, typename AltT = void>
  using special_quantities_combo_result_or_void_t =
    std::enable_if_t<ExtraReq && all_quantities_or_types_units_v<QA, QB>,
                     quantities_combo_result_or_void_t<QA, QB, OpCat, AltT>>;

  // ---------------------------------------------------------------------------

} // namespace util::quantities::concepts::details

namespace util::quantities::concepts {

  // ---------------------------------------------------------------------------
  /// Returns the product of two quantity objects, if that's declared.
  /// @see `UTIL_QUANTITIES_UNITPRODUCT()`
  template <typename QA, typename QB>
  constexpr details::special_quantities_combo_result_or_void_t<QA, QB, '*'> operator*(QA a, QB b)
  {
    using ResultInfo = details::QuantityBinaryOpResult<QA, QB, '*'>;
    using value_t = typename ResultInfo::value_t;
    using quantity_t = typename ResultInfo::type;

    value_t const p = a.value() * b.value();
    if constexpr (details::is_unity_v<typename ResultInfo::baseunit_t>) {
      // product to scalar: need to explicitly take ratio into account
      using ratio_t = typename ResultInfo::ratio_t;
      return static_cast<value_t>(details::applyRatioToValue<ratio_t>(p));
    }
    else
      return quantity_t{static_cast<value_t>(p)}; // actual quantity

  } // operator* (quantity, quantity)

  /// Returns the ratio of two quantity objects, if that's declared.
  /// @see `UTIL_QUANTITIES_UNITPRODUCT()`
  template <typename QN, typename QD>
  constexpr details::special_quantities_combo_result_or_void_t<QN, QD, '/'> operator/(QN num,
                                                                                      QD den)
  {
    using ResultInfo = details::QuantityBinaryOpResult<QN, QD, '/'>;
    using value_t = typename ResultInfo::value_t;
    using quantity_t = typename ResultInfo::type;

    return quantity_t{static_cast<value_t>(num.value() / den.value())};
  } // operator/ (quantity, quantity)

  /// Returns the ratio of a scalar and a quantity object, if that's declared.
  /// @see `UTIL_QUANTITIES_UNITPRODUCT()`
  template <typename T, typename QU, typename QT>
  constexpr details::special_quantities_combo_result_or_void_t<
    details::TypedUnity<T>,
    Quantity<QU, QT>,
    '/',
    !details::is_typed_unity_or_quantity_v<T>>
  operator/(T num, Quantity<QU, QT> const den)
  {
    return details::TypedUnity<T>{num} / den; // delegate to the other operator/
  }
  // ---------------------------------------------------------------------------

} // namespace util::quantities::concepts

// -----------------------------------------------------------------------------
/**
 * @brief Declares a product relation between units.
 * @tparam UA type of the first unit in the product (factor)
 * @tparam UB type of the second unit in the product (factor)
 * @tparam UP type of the unit of the product `UA * UB`
 *
 * It defines `BinaryOpType` template specialization like:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * template<> struct BinaryOpType<UA, UB> { using type = UP; };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * that sanctions as valid the four relations between the three units
 * (`UA * UB = UP`, `UB * UA = UP`, `UP / UA = UB`, `UP / UB = UA`).
 *
 * It requires all the operands to be base unit types (no `ScaledUnit`),
 * and that the product unit is neither of the factor ones.
 *
 * The declarations are inserted in `util::quantities::concepts::details`
 * namespace, and as such namespace shortcuts are possible (for example,
 * to access `util::quantities::units::Second` it is enough to write
 * `units::Second`);
 *
 * The product unit can be a scalar (`units::Unit`), in which case the factor
 * units are defined to be inverse one of the other.
 *
 * @note For product of the same unit, use `UTIL_QUANTITIES_UNITSQUARE()`
 *       instead.
 */
#define UTIL_QUANTITIES_UNITPRODUCT(UA, UB, UP)                                                    \
  namespace util::quantities::concepts::details {                                                  \
    static_assert(is_base_unit_v<UA>,                                                              \
                  "The first type does not meet base unit class requirements.");                   \
    static_assert(is_base_unit_v<UB>,                                                              \
                  "The second type does not meet base unit class requirements.");                  \
    static_assert(is_base_unit_v<UP>,                                                              \
                  "The third type does not meet base unit class requirements.");                   \
    static_assert(!is_unity_v<UA>, "The first type can't be a pure number (Unity)");               \
    static_assert(!is_unity_v<UB>, "The second type can't be a pure number (Unity)");              \
    static_assert(!std::is_same_v<UP, UA>, "A factor can't have the same unit as the product.");   \
    static_assert(!std::is_same_v<UP, UB>, "B factor can't have the same unit as the product.");   \
    static_assert(!std::is_same_v<UA, UB>,                                                         \
                  "Use UTIL_QUANTITIES_UNITSQUARE() for square relation.");                        \
    template <>                                                                                    \
    struct UnitBinaryOpResult<UA, UB, '*'> {                                                       \
      using type = UP;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitBinaryOpResult<UB, UA, '*'> {                                                       \
      using type = UP;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitBinaryOpResult<UP, UA, '/'> {                                                       \
      using type = UB;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitBinaryOpResult<UP, UB, '/'> {                                                       \
      using type = UA;                                                                             \
    };                                                                                             \
  }

/**
 * @brief Declares that a unit is the square of another.
 * @tparam UF type of the unit to be squared
 * @tparam UP type of the unit of the product `UA * UB`
 * @see `UTIL_QUANTITIES_UNITPRODUCT()`
 *
 * It defines `BinaryOpType` template specialization like:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * template<> struct BinaryOpType<UF, UF> { using type = UP; };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * that describe the two relations between the two units (`UF * UF = UP`,
 * and `UP / UF = UF`).
 *
 * It requires all the operands to be base unit types (no `ScaledUnit`),
 * and that the product unit is not the same as the factor one.
 *
 * @note For product of different units, use `UTIL_QUANTITIES_UNITPRODUCT()`
 *       instead.
 *
 * See `UTIL_QUANTITIES_UNITPRODUCT()` for more details: the macros are
 * functionally equivalent, with this one just dealing with the different
 * combinatorics of its case.
 *
 */
#define UTIL_QUANTITIES_UNITSQUARE(UF, UP)                                                            \
  namespace util::quantities::concepts::details {                                                     \
    static_assert(is_base_unit_v<UF>,                                                                 \
                  "The factor type does not meet base unit class requirements.");                     \
    static_assert(is_base_unit_v<UP>,                                                                 \
                  "The product type does not meet base unit class requirements.");                    \
    static_assert(!is_unity_v<UF>, "The first type can't be a pure number (Unity)";                  \
    static_assert(!std::is_same_v<UP, UF>, "The factor can't have the same unit as the product."); \
    template <>                                                                                    \
    struct UnitBinaryOpResult<UF, UF, +1> {                                                         \
      using type = UP;                                                                             \
    };                                                                                             \
    template <>                                                                                    \
    struct UnitBinaryOpResult<UP, UF, -1> {                                                         \
      using type = UF;                                                                             \
    };                                                                                                \
  }

// -----------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_QUANTITY_PRODUCTS_H
