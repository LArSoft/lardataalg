/**
 * @file   lardataalg/Utilities/quantities.h
 * @brief  Numeric variable proxies with embedded unit of measurement.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   October 30, 2018
 * @see    lardataalg/Utilities/quantites/spacetime.h
 *
 * Infrastructure for a library for variables with a given unit of measurement.
 *
 *
 * Why not `Boost::units`?
 * ------------------------
 *
 * My attempt to use `Boost::units` for the implementation (or directly) failed
 * because the `quantity` in that library is bound to a dimension without a
 * prefix modifier, e.g. always represents seconds, while we may need
 * nanoseconds. Having a type representing a time in nanoseconds is not trivial,
 * and having two types for representation of time in nanoseconds _and_
 * microseconds respectively is plain complicate.
 *
 * On the converse, that library provides arithmetic which is
 * checked for dimensionality correctness. That is beyond the scope of this
 * set of types.
 *
 * @note This code is C++17 only.
 *
 */

#ifndef LARDATAALG_UTILITIES_QUANTITIES_H
#define LARDATAALG_UTILITIES_QUANTITIES_H

// LArSoft libraries
#include "larcorealg/CoreUtils/MetaUtils.h"      // util::always_true_v<>
#include "larcorealg/CoreUtils/StdUtils.h"       // util::to_string(), ...
#include "lardataalg/Utilities/constexpr_math.h" // util::abs()

// Boost libraries
#include "boost/integer/common_factor_rt.hpp" // boost::integer::gcd()

// C/C++ standard libraries
#include <cctype>     // std::isblank()
#include <functional> // std::hash<>
#include <limits>
#include <map>
#include <ostream>
#include <ratio>
#include <regex>
#include <string>
#include <string_view>
#include <type_traits> // std::is_same<>, std::enable_if_t<>, ...

/**
 * @brief Types of variables with a unit.
 *
 * This library uses the following concepts, vaguely inspired by Boost Units
 * library:
 *
 * * _dimension_ is a category of measurements, e.g. time; there is no
 *     representation of this concept in this library
 * * _unit_ is a scale to represent a dimension, e.g. seconds; there is no
 * *   representation for the generic unit concept in this library, but each
 * *   unit needs its own unique class representing it; an example of this
 * *   simple class is `util::quantities::UnitBase`
 * * _scaled unit_ is a unit with a factor modifying its scale; e.g.,
 *     microsecond; this is represented in this library as `ScaledUnit`
 * * _quantity_ is a value interpreted via a specific scaled unit, e.g. 12.3
 *     microseconds; this is represented here by the `Quantity` class.
 *
 * Differently from Boost Units library, this one does not provide dimensional
 * analysis (hence the lack of need for the "dimension" concept.
 *
 *
 * Usage examples
 * ---------------
 *
 * The following examples use units of time, whose definitions are actually
 * provided elsewhere in this library (see
 * `lardataalg/Utilities/quantites/spacetime.h` and  the unit
 * `util::quantities::units::Second`).
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * void drifting(double driftSpace, double driftVelocity) {
 *
 *   using namespace util::quantities::time_literals; // [1]
 *
 *   util::quantities::microsecond driftTime
 *     { driftSpace / driftVelocity };
 *
 *   std::cout << "Drift time: " << driftTime << std::endl; // writes: "xxx us"
 *
 *   // double the velocity...
 *   driftTime /= 2.0;
 *   std::cout << "halved!! => " << driftTime << std::endl;
 *
 *   // add 350 ns of offset
 *   driftTime += 0.350; // ERROR!! what is 0.350?
 *   driftTime += 350_ns; // needs [1]; convert to microseconds and adds
 *   std::cout << "delayed! => " << driftTime << std::endl;
 *
 *   // ...
 *   auto dual = driftTime * 2.0; // dual is still `microsecond`
 *
 *   double half = driftTime / 2.0; // the result is converted to `double`
 *                                  // and looses its unit forever
 *
 *   double lateTime = 500;
 *   driftTime = lateTime; // ERROR! which unit is `lateTime` in?
 *   driftTime = microsecond(lateTime); // assign from `lateTime` us
 *
 * } // drifting()
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *
 *
 * Organization of the code
 * -------------------------
 *
 * The code of this library is entirely under this namespace
 * (`util::quantities`). It is organised in a few subspaces:
 *
 * * `concepts` contains all the infrastructure for the library, including the
 *     definition of classes representing a unit, a quantity, and more
 *     * `details`: implementation details (users should not bother with them)
 * * `units` contains the definitions of actual units (e.g. seconds, ampere...)
 *
 * The file `lardataalg/Utilities/quantities.h` does not contain the definition
 * of any actual quantity nor unit. Quantity libraries are expected to include
 * this header, extend the namespace `units` to include the units they need,
 * and finally adding the quantites needed directly in this very namespace.
 *
 * An example of adding the definition of "ampere":
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * #include "lardataalg/Utilities/quantities.h"
 *
 * namespace util::quantities {
 *
 *   namespace units {
 *
 *     /// Unit of electric current.
 *     struct ampere {
 *       static constexpr auto symbol = "A"sv;
 *       static constexpr auto name   = "ampere"sv;
 *     }; // ampere
 *
 *   } // namespace units
 *
 *
 *   /// Type of current stored in ampere.
 *   template <typename T = double>
 *   using ampere_as = concepts::Quantity<concepts::ScaledUnit<units::ampere>, T>;
 *
 *   /// Type of current stored in ampere, in double precision.
 *   using ampere = ampere_as<>;
 *
 *   /// Type of current stored in milliampere.
 *   template <typename T = double>
 *   using milliampere_as = concepts::rescale<ampere_as<T>, std::milli>;
 *
 *   /// Type of current stored in milliampere, in double precision.
 *   using milliampere = milliampere_as<>;
 *
 * } // namespace util::quantities
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @note The suffix `sv` converts the literal string into a `std::string_view`,
 *       which is one of the few string types which can be defined at compile
 *       time (`constexpr`) and have a decently complete interface.
 *
 *
 * Another example may be data size:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * #include "lardataalg/Utilities/quantities.h"
 *
 * namespace util::quantities {
 *
 *   namespace units {
 *
 *     /// Unit of data size.
 *     struct byte {
 *       static constexpr auto symbol = "B"sv;
 *       static constexpr auto name   = "byte"sv;
 *     }; // byte
 *
 *   } // namespace units
 *
 *   namespace binary {
 *
 *     // Prefix for binary 2^20 (1048576).
 *     using mebi = std::ratio<(1U << 20), 1U>;
 *
 *   } // namespace binary
 *
 *   /// Type of data size stored bytes.
 *   template <typename T = unsigned long long>
 *   using byte_as = concepts::Quantity<concepts::ScaledUnit<units::byte>, T>;
 *
 *   /// Type of data size stored bytes, typically in 64 bits.
 *   using byte = byte_as<>;
 *
 *   /// Type of data size stored in mibibyte.
 *   template <typename T = unsigned long long>
 *   using mebibyte_as = concepts::rescale<byte_as<T>, binary::mebi>;
 *
 *   /// Type of data size stored in mibibyte, typically in 64 bits.
 *   using mebibyte = mebibyte_as<>;
 *
 * } // namespace util::quantities
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * This example _mostly works_, with the following exceptions:
 *
 * * the prefix is not known to the infrastructure and it will not be printed
 *     (it will show as question marks)
 * * especially when using an integral base type, a lot of care must be put
 *     in the rounding: for example, 2000000 bytes will be converted into
 *     1 mebibyte, and if converted back it will show 1048576 bytes.
 *
 */
namespace util::quantities {

  /**
   * @brief Infrastructure for the quantities library.
   *
   * The namespace `concepts` contains all the infrastructure for the library,
   * including the definition of classes representing a unit, a quantity, and
   * more.
   */
  namespace concepts {

    namespace details {

      //------------------------------------------------------------------------
      //---  Ratio metaprogramming
      //------------------------------------------------------------------------
      /// Applies the specified `Ratio` to the value in `v`.
      template <typename Ratio, typename Value>
      static constexpr auto applyRatioToValue(Value&& v)
      {
        return v * Ratio::num / Ratio::den;
      }

      template <typename R>
      struct invert_ratio;

      template <typename R>
      using invert_t = typename invert_ratio<R>::type;

      template <typename R>
      struct ratio_simplifier;

      template <typename R>
      using simplify_ratio = typename ratio_simplifier<R>::type;

      //------------------------------------------------------------------------
      //--- Unit-related
      //------------------------------------------------------------------------
      /// Implementation of `is_base_unit_v`.
      template <typename T, typename = void>
      struct is_base_unit : std::false_type {};

      /// Implementation of `base_unit_of`.
      template <typename T, typename = void>
      struct base_unit_extactor;

      /// Implementation of `is_quantity_v`.
      template <typename Q>
      struct is_quantity;

      /// Trait: `true_type` if `U` is a `ScaledUnit`-based object.
      template <typename U>
      struct has_unit;

      /// Trait: `true` if `U` is a `ScaledUnit`-based object.
      template <typename U>
      constexpr bool has_unit_v = has_unit<U>();

      /// Trait: `true_type` if `Q` is a `Quantity`-based object.
      template <typename Q>
      struct has_quantity;

      /// Trait: `true` if `Q` is a `Quantity`-based object.
      template <typename Q>
      constexpr bool has_quantity_v = has_quantity<Q>();

      //------------------------------------------------------------------------
      /// Trait: type of value of `T`: may be `T` or, for objects with units,
      /// `T::value_t`.
      template <typename T>
      struct quantity_value_type;

      /// Type of value of `T`: may be `T` or, for objects with units,
      /// `T::value_t`.
      template <typename T>
      using quantity_value_t = typename quantity_value_type<T>::type;

      /**
       * Trait: `true_type` if the type `T` is a value compatible with the value
       * of `Q`.
       */
      template <typename T, typename Q>
      struct is_value_compatible_with;

      /// Trait: `true` if the type `T` is compatible with the value of `Q`.
      template <typename T, typename Q>
      constexpr bool is_value_compatible_with_v = is_value_compatible_with<T, Q>();

      /**
       * Trait: `true_type` if the value type of `T` is compatible with the one
       * of `U`.
       *
       * Note that this tells that `T` is compatible with `U`, but it may still
       * leave `U` not compatible with `T` (for example, if the compatibility
       * criterium were something asymmetric like a non-narrowing conversion).
       */
      template <typename T, typename U>
      struct has_value_compatible_with;

      /// Trait: `true` if the value type of `T` is compatible with `U`'s.
      template <typename T, typename U>
      constexpr bool has_value_compatible_with_v = has_value_compatible_with<T, U>();

      //------------------------------------------------------------------------
      //---  constexpr string concatenation
      //------------------------------------------------------------------------
      // giving up for now; Boost 1.68.0 offers some help, but we don;t use it
      // yet

      //------------------------------------------------------------------------
      template <typename Q>
      class numeric_limits;

      //------------------------------------------------------------------------

    } // namespace details

    using namespace std::string_view_literals;

    //------------------------------------------------------------------------
    /// A ratio product (like `std::ratio_multiply`) with simplified terms.
    template <typename ARatio, typename BRatio>
    using simplified_ratio_multiply = details::simplify_ratio<std::ratio_multiply<ARatio, BRatio>>;

    /// A ratio division (like `std::ratio_divide`) with simplified terms.
    template <typename NumRatio, typename DenRatio>
    using simplified_ratio_divide = details::simplify_ratio<std::ratio_divide<NumRatio, DenRatio>>;

    //------------------------------------------------------------------------
    /**
     * A unit is the definition of a reference quantity to measure a dimension.
     * For example, second or ampere.
     *
     * Units are independent C++ classes which are required to present a minimal
     * interface:
     *
     * * `symbol` of the unit, as a static constant string
     * * `name` of the unit, as a static constant string
     *
     * Here "string" is intended as an object convertible to `std::string` and
     * which itself exposes a `std::string`-like behaviour.
     *
     * Although units can be derived from `UnitBase`, there is currently no
     * necessity to do so since no part is shared.
     * `UnitBase` is therefore provided as an example implementation.
     */
    struct UnitBase {
      /// Symbol of the unit (e.g. "A").
      static constexpr std::string_view symbol = "?"sv;
      /// Long name of unit (e.g. "ampere").
      static constexpr std::string_view name = "unknown"sv;
    }; // struct UnitBase

    template <typename R>
    struct Prefix {
      using ratio = R; ///< The ratio this prefix is about.

      /// Returns the unit symbol (`Long` `false`) or name (`Long` `true`).
      static constexpr auto names(bool Long = false);

      /// Returns the symbol of the prefix.
      static constexpr auto symbol() { return names(false); }

      /// Returns the full name of the prefix.
      static constexpr auto name() { return names(true); }

    }; // struct Prefix

    /// Trait: `T` implements a `BaseUnit` interface.
    template <typename T>
    constexpr bool is_base_unit_v = details::is_base_unit<T>::value;

    /**
     * @brief Trait: base unit of the specified object.
     *
     * Supports `Quantity`, `Unit`, `Interval` and `Point`.
     * It also sort-of-supports `UnitBase` and the like.
     */
    template <typename T>
    using base_unit_of = typename details::base_unit_extactor<T>::type;

    template <typename U, typename R = std::ratio<1>>
    struct ScaledUnit {

      using baseunit_t = U;            ///< Base, unscaled unit.
      using unit_t = ScaledUnit<U, R>; ///< Unit with scale (i.e. this object).
      using ratio = R;                 ///< The ratio to go from the base unit to this one.
      using prefix_t = Prefix<ratio>;  ///< The prefix of the unit.

      // -- BEGIN Conversion to string -----------------------------------------
      /**
       * @name Conversion to string.
       *
       * @note Implementation note: the returned values (currently
       *       `std::string_view`) are not `constexpr` since it takes noticeable
       *       additional effort to make them so.
       */
      /// @{

      /// Returns short symbol of the unit (e.g. "ns") is a string-like object.
      static auto symbol() { return std::string(prefix().symbol()) + baseUnit().symbol.data(); }

      /// Returns full name of the unit (e.g. "nanoseconds") as a string-like
      /// object.
      static auto name() { return std::string(prefix().name()) + baseUnit().name.data(); }

      /// @}
      // -- END Conversion to string -------------------------------------------

      // -- BEGIN Representation conversions -----------------------------------
      /// @{
      /// @name Representation conversions.

      /// Converts a value from the base unit to this one.
      template <typename T>
      static constexpr T scale(T v)
      {
        return details::applyRatioToValue<details::invert_t<ratio>>(v);
      }

      /// Converts a value from this scaled unit to the base one.
      template <typename T>
      static constexpr T unscale(T v)
      {
        return details::applyRatioToValue<ratio>(v);
      }

      /// Converts a value from the scaled unit to a different `TargetRatio`.
      template <typename TargetRatio, typename T>
      static constexpr T scaleTo(T v)
      {
        return details::applyRatioToValue<simplified_ratio_divide<ratio, TargetRatio>>(v);
      }

      /// Converts a value from `TargetRatio` scale to this scaled unit.
      template <typename TargetRatio, typename T>
      static constexpr T fromRepr(T v)
      {
        return details::applyRatioToValue<simplified_ratio_divide<TargetRatio, ratio>>(v);
      }

      /// @}
      // -- END Representation conversions -------------------------------------

      // -- BEGIN Type features ------------------------------------------------
      /// @name Type features
      /// @{

      /// Returns an instance of the `prefix_t` type.
      static constexpr prefix_t prefix() { return {}; }

      /// Returns an instance of the `baseunit_t` type.
      static constexpr baseunit_t baseUnit() { return {}; }

      /// Returns an instance of the `unit_t` type.
      static constexpr unit_t unit() { return {}; }

      /// Returns whether scaled unit `U` has the same base unit as this one.
      template <typename OU>
      static constexpr bool sameBaseUnitAs()
      {
        return std::is_same<baseunit_t, base_unit_of<OU>>();
      }

      /// Returns whether scaled unit `U` has the same base unit as this one.
      template <typename OU>
      static constexpr bool sameUnitAs()
      {
        return std::is_same<unit_t, typename OU::unit_t>();
      }

      /// @}
      // -- END Type features --------------------------------------------------

    }; // ScaledUnit<>

    template <typename U, typename R>
    std::ostream& operator<<(std::ostream& out, ScaledUnit<U, R> const& unit)
    {
      using unit_t = ScaledUnit<U, R>;
      return out << unit_t::prefix_t::symbol() << unit_t::baseunit_t::symbol;
    }

    /// Trait: `true` if `Q` is a `Quantity` specialization.
    template <typename Q>
    constexpr bool is_quantity_v = details::is_quantity<Q>();

    /** ************************************************************************
     * @brief A value measured in the specified unit.
     * @tparam Unit the scaled unit type representing the unit of this quantity
     * @tparam T type of base value
     *
     * A `Quantity` instance is a glorified number of type `T` (or, `value_t`).
     * The `Quantity` class adds to it the explicit documentation of the unit
     * the value is measured in, plus some attempt to preserve that information:
     *
     * * a `Quantity` type will carry the information of its unit with the type
     * * quantities must be assigned other quantities:
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     *     using util::quantities::milliampere;
     *
     *     milliampere i;
     *     i = 2.5; // ERROR! what is 2.5?
     *     i = milliampere(2.5);
     *
     *     milliampere i2 { 2.5 }; // SPECIAL, allowed only in construction
     *     i2 = i1;
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * * can be converted, implicitly or explicitly, to its plain value:
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     *     using util::quantities::milliampere;
     *
     *     milliampere i { 6.0 };
     *     double v = i; // implicit conversion
     *     v = double(i); // explicit conversion
     *     v = i.value(); // even more explicit conversion
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * * weakly resists attempts to mess with units
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     *     milliampere mi { 4.0 };
     *     microampere ui { 500.0 };
     *     mi = ui; // now mi == 0.5
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * * weakly attempts to preserve the unit information
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
     *     using namespace util::quantities;
     *     using namespace util::quantities::electronics_literals;
     *
     *     milliampere mi { 4.0 };
     *     microampere ui { 500.0 };
     *
     *     mi += ui;  // 4.5 mA
     *     mi *= ui;  // ERROR! what does this even mean??
     *     mi += 5.0; // ERROR!
     *     mi += milliampere(3.0); // 7.5 mA
     *     mi += 2.0_ma; // 9.5 mA
     *     mi + ui; // ERROR! (arbitrary whether to represent in mA or uA)
     *     mi + 5.0; // ERROR! (as above)
     *     mi / 5.0; // milliampere{1.9}
     *     mi - 5_mA; // milliampere{4.5}
     *     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *
     * (`milliampere` and `microampere` are hypotetical instantiations of
     * `Quantity` template class from `util::quantities`, and they also have
     * a literal conversion to correctly interpret things like `2.0_ma` or
     * `7_A`)
     *
     *
     * Implementation details
     * -----------------------
     *
     * * method signatures usually pass `Quantity` objects by value; it is
     *     assumed that the type `T` is simple enough that copying it is faster
     *     than passing around its reference, and that any overhead is
     *     optimized out by the compiler
     *
     */
    template <typename Unit, typename T = double>
    struct Quantity {

    public:
      using value_t = T;                    ///< Type of the stored value.
      using unit_t = Unit;                  ///< The unit and scale of this quantity.
      using quantity_t = Quantity<Unit, T>; ///< The type of this class.

      /// Description of the unscaled unit.
      using baseunit_t = typename unit_t::baseunit_t;

      /// Constructor: value is left uninitialized.
      // NOTE: this is not `constexpr` because using it n a constexpr would
      //       yield an uninitialized constant
      explicit Quantity() = default;

      /// Constructor: takes a value in the intended representation.
      explicit constexpr Quantity(value_t v) : fValue(v) {}

      /**
       * @brief Constructor: converts from another quantity.
       * @tparam Q type of the other quantity
       * @param q quantity to be converted from
       *
       * Quantities are required to be in the same unit (unit scale may differ).
       * The value in `q` is converted from its native scale into the one of
       * this quantity.
       *
       * This assignment prevents narrowing. To force narrowing, convert the
       * operand quantity to a number or "cast" it with `asValueType<>()`.
       */
      template <typename Q, typename std::enable_if_t<is_quantity_v<Q>>* = nullptr>
      constexpr Quantity(Q q)
        : fValue{unit_t::template fromRepr<typename Q::unit_t::ratio>(q.value())}
      {
        static_assert(sameBaseUnitAs<Q>(),
                      "Can't construct from quantity with different base unit");
      }

      /// Returns the value of the quantity.
      constexpr value_t value() const { return fValue; }

      /// Explicit conversion to the base quantity.
      explicit constexpr operator value_t() const { return value(); }

      // -- BEGIN Access to the scaled unit ------------------------------------
      /// @name Access to the scaled unit.
      /// @{

      /// Returns an object with as type the scaled unit (`unit_t`).
      static constexpr unit_t unit() { return {}; }

      /// Returns an object with as type the base unit (`baseunit_t`).
      static constexpr baseunit_t baseUnit() { return {}; }

      /// Returns the full name of the unit, in a string-like object.
      static auto unitName() { return unit_t::name(); }

      /// Returns the symbol of the unit, in a string-like object.
      static auto unitSymbol() { return unit_t::symbol(); }

      /**
       * @brief Returns whether this quantity has the same base unit as `OU`.
       * @param OU any type with `baseunit_t` type
       *           (including `ScaledUnit`, `Quantity`, `Interval`...)
       */
      template <typename OU>
      static constexpr bool sameBaseUnitAs()
      {
        return unit_t::template sameBaseUnitAs<OU>();
      }

      /**
       * @brief Returns whether this quantity has same unit and scale as `OU`.
       * @param OU any type with `unit_t` type
       *           (including `ScaledUnit`, `Quantity`, `Interval`...)
       */
      template <typename OU>
      static constexpr bool sameUnitAs()
      {
        return unit_t::template sameUnitAs<OU>();
      }

      /// Whether `U` is a value type compatible with `value_t`.
      template <typename U>
      static constexpr bool is_compatible_value_v = details::is_value_compatible_with_v<U, value_t>;

      /// Whether `U` has (or is) a value type compatible with `value_t`.
      template <typename U>
      static constexpr bool has_compatible_value_v =
        details::has_value_compatible_with_v<U, value_t>;

      /// Returns whether `U` is a value type compatible with `value_t`.
      template <typename U>
      static constexpr bool isCompatibleValue()
      {
        return quantity_t::is_compatible_value_v<U>;
      }

      /// Returns whether `U` has (or is) a value type compatible with
      /// `value_t`.
      template <typename U>
      static constexpr bool hasCompatibleValue()
      {
        return quantity_t::has_compatible_value_v<U>;
      }

      /// @}
      // -- END Access to the scaled unit --------------------------------------

      // -- BEGIN Asymmetric arithmetic operations -----------------------------
      /**
       * @name Asymmetric operand arithmetic operations
       *
       * These arithmetic operations take care of preserving the quantity
       * through them.
       * Not all possible (or reasonable) operations are supported yet.
       * Some operations that may be symmetric (like multiplication by a scalar)
       * are implemented as free functions rather than methods.
       *
       * @note These operations are essentially provided by convenience. There
       *       are many cases (corner and not) where the implicit conversion
       *       to the base type kicks in. This implementation does not aim to
       *       _prevent_ that from happening, but requests are welcome to add
       *       features to _allow_ that not to happen, with some care of the
       *       user.
       *
       * @note Operators `+` and `-` support only quantity operands of the same
       *       type, and no plain number operand. The rational behind this is
       *       that when writing `a + b` with `a` and `b` two homogeneous
       *       quantities, we expect to be able to use `b + a` as well.
       *       But it `a` and `b` are different types (e.g. one is `second`,
       *       the other is `millisecond`), an arbitrary choice has to be made
       *       on which type the result should be, and having different types
       *       for `a + b` and `b + a` is not acceptable.
       *       If the intention is clear, methods `plus()` and `minus()` are
       *       provided with the understanding that e.g. `a.plus(b)` explicitly
       *       requires the result to be of type `a`.
       *
       */
      /// @{

      /// Returns a quantity sum of this and `other` (must have same unit).
      template <typename OU, typename OT>
      constexpr quantity_t operator+(Quantity<OU, OT> const other) const;

      /// Returns a quantity difference of this and `other`
      /// (must have same unit).
      template <typename OU, typename OT>
      constexpr quantity_t operator-(Quantity<OU, OT> const other) const;

      /// Returns a quantity sum of this and `other`.
      template <typename OU, typename OT>
      constexpr quantity_t plus(Quantity<OU, OT> const other) const;

      /// Returns a quantity difference of this and `other`.
      template <typename OU, typename OT>
      constexpr quantity_t minus(Quantity<OU, OT> const other) const;

      /// Division by a quantity, returns a pure number.
      template <typename OU, typename OT>
      constexpr std::enable_if_t<quantity_t::sameBaseUnitAs<OU>(), value_t> operator/(
        Quantity<OU, OT> q) const;

      /// Add the `other` quantity (possibly concerted) to this one.
      template <typename OU, typename OT>
      quantity_t& operator+=(Quantity<OU, OT> const other);

      /// Subtract the `other` quantity (possibly concerted) to this one.
      template <typename OU, typename OT>
      quantity_t& operator-=(Quantity<OU, OT> const other);

      /// Scale this quantity by a factor.
      template <typename OT>
      std::enable_if_t<std::is_arithmetic_v<OT>, quantity_t&> operator*=(OT factor)
      {
        fValue *= factor;
        return *this;
      }

      /// Scale the quantity dividing it by a quotient.
      template <typename OT>
      std::enable_if_t<std::is_arithmetic_v<OT>, quantity_t&> operator/=(OT quot)
      {
        fValue /= quot;
        return *this;
      }

      /// Returns a quantity with same value.
      constexpr quantity_t operator+() const { return quantity_t(value()); }

      /// Returns a quantity with same value but the sign flipped.
      constexpr quantity_t operator-() const { return quantity_t(-value()); }

      /// Returns a quantity with the absolute value of this one.
      constexpr quantity_t abs() const { return quantity_t(util::abs(value())); }

      /// @}
      // -- END Asymmetric arithmetic operations -------------------------------

      // -- BEGIN Comparisons --------------------------------------------------
      /**
       * @name Comparisons.
       *
       * Comparisons with plain numbers are managed by implicit conversion.
       * More care is needed for quantities.
       * Comparisons between two quantity instances `a` and `b` work this way:
       * * if `a` and `b` do not have the same unit, they are _not_ comparable
       * * if `a` and `b` have the same unit, one is converted to the other and
       *     the comparison is performed there
       * * if `a` and `b` have the same scaled unit, their values are compared
       *     directly
       *
       * Value storage types are compared according to C++ rules.
       *
       */
      /// @{

      template <typename OU, typename OT>
      constexpr bool operator==(Quantity<OU, OT> const other) const;

      template <typename OU, typename OT>
      constexpr bool operator!=(Quantity<OU, OT> const other) const;

      template <typename OU, typename OT>
      constexpr bool operator>=(Quantity<OU, OT> const other) const;

      template <typename OU, typename OT>
      constexpr bool operator>(Quantity<OU, OT> const other) const;

      template <typename OU, typename OT>
      constexpr bool operator<=(Quantity<OU, OT> const other) const;

      template <typename OU, typename OT>
      constexpr bool operator<(Quantity<OU, OT> const other) const;

      /// @}
      // -- END Asymmetric arithmetic operations -------------------------------

      /// Convert this quantity into the specified one.
      template <typename OQ>
      constexpr OQ convertInto() const
      {
        return OQ(*this);
      }

      /// Returns a new quantity with same unit and value using value type `OT`.
      template <typename OT>
      constexpr Quantity<unit_t, OT> asValueType() const
      {
        return Quantity<unit_t, OT>{static_cast<OT>(value())};
      }

      /**
       * @brief Returns a new quantity initialized with the specified value.
       * @tparam U type to initialize the quantity with
       * @param value the value to initialize the quantity with
       * @return a new `Quantity` object initialized with `value`
       *
       * The `value` is cast into `value_t` via `static_cast()`.
       *
       * Example: be `Tick` a quantity based on an integral value, like
       * `util::quantities::tick`, and `detClocks` an instance of
       * `detinfo::DetectorClocks`:
       * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
       * double const tickDuration = detClocks.OpticalClock().TickPeriod();
       *
       * auto const triggerTick
       *   = Tick::castFrom(detClocks.TriggerTime() / tickDuration);
       * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       * `triggerTick` will be of type `Tick` and will denote the number of the
       * tick (0-based) within which `detClocks.TriggerTime()` fell.
       */
      template <typename U>
      static constexpr quantity_t castFrom(U value)
      {
        return quantity_t{static_cast<value_t>(value)};
      }

    private:
      value_t fValue{}; ///< Stored value.

    }; // struct Quantity

    template <typename... Args>
    std::ostream& operator<<(std::ostream& out, Quantity<Args...> const q)
    {
      return out << q.value() << " " << q.unit();
    }

    // -- BEGIN Arithmetic operations ------------------------------------------
    /**
     * @name Arithmetic operations on Quantity
     *
     * These operations, as well as the ones implemented as member functions,
     * are provided for convenience.
     *
     * Here the symmetric operations are defined, where different operands can
     * be swapped.
     *
     */
    /// @{

    //@{
    /**
     * Addition and subtraction of a quantity and a plain number are forbidden.
     *
     * The rationale is that it is not acceptable to support `position + 2_m`
     * but not `2_m + position`, and that it is not acceptable to have
     * `2_m + 50_cm` yield a type different than `50_cm + 2_m` (i.e. should
     * both return centimeters, or meters?).
     */
    template <typename U, typename T>
    constexpr Quantity<U, T> operator+(Quantity<U, T> const q, T shift) = delete;
    template <typename U, typename T>
    constexpr Quantity<U, T> operator+(T shift, Quantity<U, T> const q) = delete;
    template <typename U, typename T>
    constexpr Quantity<U, T> operator-(Quantity<U, T> const q, T shift) = delete;
    template <typename U, typename T>
    constexpr Quantity<U, T> operator-(T shift, Quantity<U, T> const q) = delete;
    //@}

    //@{
    /// Multiplication with a scalar.
    template <typename U, typename T, typename OT>
    constexpr std::enable_if_t<Quantity<U, T>::template is_compatible_value_v<OT>, Quantity<U, T>>
    operator*(Quantity<U, T> const q, OT factor)
    {
      return Quantity<U, T>{q.value() * static_cast<T>(factor)};
    }
    template <typename U, typename T, typename OT>
    constexpr std::enable_if_t<Quantity<U, T>::template is_compatible_value_v<OT>, Quantity<U, T>>
    operator*(OT factor, Quantity<U, T> const q)
    {
      return q * factor;
    }
    //@}

    //@{
    // Division by a scalar.
    template <typename U, typename T, typename OT>
    constexpr std::enable_if_t<Quantity<U, T>::template is_compatible_value_v<OT>, Quantity<U, T>>
    operator/(Quantity<U, T> q, OT quot)
    {
      return Quantity<U, T>{q.value() / static_cast<T>(quot)};
    }
    //@}

    /// @}
    // -- END Arithmetic operations --------------------------------------------

    // -------------------------------------------------------------------------
    /**
     * @brief Alias for a quantity based on a scaled unit.
     * @tparam Unit type of unit (unscaled)
     * @tparam Ratio scale of the unit for this quantity (e.g. `std::milli`)
     * @tparam T type of value stored
     */
    template <typename Unit, typename Ratio, typename T>
    using scaled_quantity = Quantity<ScaledUnit<Unit, Ratio>, T>;

    // -------------------------------------------------------------------------
    /// Type of a quantity like `Q`, but with a different unit scale `R`.
    template <typename Q, typename R, typename T = typename Q::value_t>
    using rescale = Quantity<ScaledUnit<typename Q::unit_t::baseunit_t, R>, T>;

    // -------------------------------------------------------------------------

    //
    // extensions STL-style
    // (can't be put in `std` since they are not specializations)
    //

    /// Converts a unit into a string.
    /// @see `util::to_string()`
    template <typename... Args>
    std::string to_string(ScaledUnit<Args...> const& unit)
    {
      return std::string(unit.prefix().symbol()) + unit.baseUnit().symbol.data();
    }

    /// Converts a quantity into a string.
    /// @see `util::to_string()`
    template <typename... Args>
    std::string to_string(Quantity<Args...> const& q)
    {
      return util::to_string(q.value()) + ' ' + util::to_string(q.unit());
    }

    // -------------------------------------------------------------------------

  } // namespace concepts

  // ---------------------------------------------------------------------------
  /**
   * @brief Definitions of additional prefixes.
   *
   * Quantities are based on _scaled_ units, which are units with a scaling
   * factor.
   *
   * Prefixes describe these scaling factors, as a rational number represented
   * via a fraction. In this library, prefix objects must expose the same
   * interface as `std::ratio` template.
   *
   * The standard ratio classes defined in C++ (e.g. `std::milli`, `std::giga`)
   * provide most of the needed prefixes.
   * In this namespace, custom prefixes can be defined.
   *
   * A prefix can be associated with its own symbol. In that case, the prefix
   * should specialize the template `util::quantites::concepts::Prefix` and
   * provide:
   *
   * * static, `constexpr` method for the full name (`name`), e.g. `"milli"`
   * * static, `constexpr` method for the short name (`symbol`), e.g. `"m"`
   *
   * For example, this prefix should work like 1'000, but has its own symbol:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * namespace util::quantities::prefixes {
   *
   *   struct grant: public std::ratio<1000> {};
   *
   *   template <>
   *   struct Prefix<grant> {
   *
   *     /// Returns the symbol of the prefix.
   *     static constexpr auto symbol() { return "k"sv; }
   *
   *     /// Returns the full name of the prefix.
   *     static constexpr auto name() { return "grant"sv; }
   *
   *   }; // struct Prefix<grant>
   *
   * } // namespace util::quantities::prefixes
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * Note however that the current implementation tends to bring back to
   * `std::ratio`. Also note that defining an alias, like in
   * `using grant = std::ratio<1000>`, is not enough to allow for a different
   * symbol.
   *
   * The namespace also imports the prefixes from C++ standard library for
   * convenience.
   */
  namespace prefixes { // we expect other libraries to fill it

    // ratios imported from C++ standard library:
    using std::atto, std::femto, std::pico, std::nano, std::micro, std::milli, std::centi,
      std::deci, std::deca, std::hecto, std::kilo, std::mega, std::giga, std::tera, std::peta,
      std::exa;

  } // namespace prefixes

  // ---------------------------------------------------------------------------
  /**
   * @brief Definitions of actual units.
   *
   * Units describe a reference quantity to measure a dimension.
   * The namespace `units` contains the definitions of actual units (e.g.
   * seconds, ampere...)
   *
   * Each unit is represented by a class. Each class should follow the interface
   * of `util::quantities::concepts::UnitBase`, but it does not have to inherit
   * from it.
   *
   * Each unit must provide its name and its symbol (no locale is supported!).
   * Example:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * namespace util::quantities::units {
   *
   *   /// Unit of data size.
   *   struct byte {
   *     static constexpr auto symbol = "B"sv;
   *     static constexpr auto name   = "byte"sv;
   *   }; // byte
   *
   * } // namespace util::quantities::units
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   */
  namespace units {} // we expect other libraries to fill it

  // -------------------------------------------------------------------------
  // @{
  /**
   * @brief Returns a quantity of the specified type parsed from a string.
   * @tparam Quantity the quantity to be returned
   * @param s the string to be parsed
   * @param unitOptional (default: `false`) whether unit is not required in `s`
   * @return a quantity of the specified type parsed from a string
   * @throw MissingUnit `s` does not contain the required unit
   * @throw ValueError the numerical value in `s` is not parseable
   * @throw ExtraCharactersError spurious characters after the numeric value
   *                             (including an unrecognised unit prefix)
   *
   * A quantity of type `Quantity` is returned, whose value is interpreted from
   * the content of `s`. The standard format includes a real number, a space
   * and a unit symbol. If `unitOptional` is `false`, that unit is required,
   * otherwise it is optional and defaults to the unit and scale in `Quantity`.
   * The base unit in `s`, when present, must exactly match the base unit of
   * `Quantity`; the scale may differ, in which case the proper conversion is
   * applied.
   *
   * Example:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using namespace util::quantities::time_literals;
   * using util::quantities::microsecond;
   *
   * auto const t = util::quantities::makeQuantity<microsecond>("7 ms");
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * will assign to `t` the value `7000`.
   */
  template <typename Quantity>
  Quantity makeQuantity(std::string_view s, bool unitOptional = false);

  template <typename Quantity>
  Quantity makeQuantity(std::string const& s, bool unitOptional = false);

  template <typename Quantity>
  Quantity makeQuantity(char const* s, bool unitOptional = false);

  //@}

  // --- BEGIN -- Specific exceptions ------------------------------------------

  /// String representing a quantity has no unit.
  struct MissingUnit : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  /// String representing a quantity has unsupported unit prefix.
  struct InvalidUnitPrefix : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  /// String representing a quantity has incompatible unit.
  struct MismatchingUnit : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  /// String representing a quantity has an invalid number.
  struct ValueError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  /// String representing a quantity has spurious characters after the number.
  struct ExtraCharactersError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  // --- END -- Specific exceptions --------------------------------------------

  // ---------------------------------------------------------------------------

} // namespace util::quantities

//------------------------------------------------------------------------------
//---  template implementation
//------------------------------------------------------------------------------
namespace util::quantities::concepts::details {

  //----------------------------------------------------------------------------
  template <std::intmax_t Num, std::intmax_t Den>
  struct invert_ratio<std::ratio<Num, Den>> {
    using type = std::ratio<Den, Num>;
  };

  //----------------------------------------------------------------------------
  template <std::intmax_t Num, std::intmax_t Den>
  struct ratio_simplifier<std::ratio<Num, Den>> {
    static constexpr auto gcd = boost::integer::gcd(Num, Den);
    using type = std::ratio<Num / gcd, Den / gcd>;
  }; // ratio_simplifier

  //----------------------------------------------------------------------------
  template <typename U, typename Enable = void>
  struct has_unit_impl : public std::false_type {};

  template <typename U>
  struct has_unit_impl<U, std::enable_if_t<util::always_true_v<typename U::unit_t>>>
    : public std::true_type {};

  template <typename U>
  struct has_unit : public has_unit_impl<U> {};

  //----------------------------------------------------------------------------
  template <typename Q>
  struct is_quantity : public std::false_type {};

  template <typename... Args>
  struct is_quantity<Quantity<Args...>> : public std::true_type {};

  //----------------------------------------------------------------------------
  template <typename Q, typename Enable = void>
  struct has_quantity_impl : public std::false_type {};

  template <typename Q>
  struct has_quantity_impl<Q, std::enable_if_t<util::always_true_v<typename Q::quantity_t>>>
    : public std::true_type {};

  template <typename Q>
  struct has_quantity : public has_quantity_impl<Q> {};

  //----------------------------------------------------------------------------
  template <typename T, typename = void>
  struct quantity_value_type_impl {
    using type = T;
  }; // quantity_value_type

  template <typename T>
  struct quantity_value_type_impl<
    T,
    std::enable_if_t<(has_unit_v<T> && util::always_true_v<typename T::value_t>)>> {
    using type = typename T::value_t;
  }; // quantity_value_type_impl<unit>

  template <typename T>
  struct quantity_value_type : quantity_value_type_impl<T> {};

  //----------------------------------------------------------------------------
  template <typename T, typename Q>
  struct is_value_compatible_with
    : std::bool_constant<std::is_convertible_v<T, quantity_value_t<Q>>> {};

  //----------------------------------------------------------------------------
  template <typename T, typename U>
  struct has_value_compatible_with : is_value_compatible_with<quantity_value_t<T>, U> {};

  //----------------------------------------------------------------------------
  /// Limits of a quantity are the same as the underlying type.
  template <typename Q>
  class numeric_limits : public std::numeric_limits<typename Q::value_t> {

    using quantity_t = Q;
    using value_traits_t = std::numeric_limits<typename quantity_t::value_t>;

  public:
    static constexpr quantity_t min() noexcept { return quantity_t{value_traits_t::min()}; }
    static constexpr quantity_t max() noexcept { return quantity_t{value_traits_t::max()}; }
    static constexpr quantity_t lowest() noexcept { return quantity_t{value_traits_t::lowest()}; }
    static constexpr quantity_t epsilon() noexcept { return quantity_t{value_traits_t::epsilon()}; }
    static constexpr quantity_t round_error() noexcept
    {
      return quantity_t{value_traits_t::round_error()};
    }
    static constexpr quantity_t infinity() noexcept
    {
      return quantity_t{value_traits_t::infinity()};
    }
    static constexpr quantity_t quiet_NaN() noexcept
    {
      return quantity_t{value_traits_t::quiet_NaN()};
    }
    static constexpr quantity_t signaling_NaN() noexcept
    {
      return quantity_t{value_traits_t::signaling_NaN()};
    }
    static constexpr quantity_t denorm_min() noexcept
    {
      return quantity_t{value_traits_t::denorm_min()};
    }

  }; // numeric_limits<Quantity>

  //----------------------------------------------------------------------------

} // namespace util::quantities::concepts::details

//------------------------------------------------------------------------------
//--- template implementation
//------------------------------------------------------------------------------
/// Implementation of `is_base_unit_v` for actual base unit objects
/// (e.g. `BaseUnit`): asks the complete `BaseUnit` mandatory interface.
template <typename U>
struct util::quantities::concepts::details::is_base_unit<
  U,
  std::enable_if_t<std::is_same_v<std::decay_t<decltype(U::name)>, std::string_view> &
                   std::is_same_v<std::decay_t<decltype(U::symbol)>, std::string_view>>>
  : std::true_type {};

/// Implementation of `base_unit_of` supporting `Quantity`, `Unit`, `Interval`
/// and `Point` (and any class accidentally declaring a `baseunit_t` type).
template <typename Q>
struct util::quantities::concepts::details::
  base_unit_extactor<Q, std::void_t<typename Q::baseunit_t>> {
  using type = typename Q::baseunit_t;
};

/// Implementation of `base_unit_of` supporting a base unit itself
/// (e.g. `BaseUnit`) by asking the complete `BaseUnit` mandatory interface.
template <typename U>
struct util::quantities::concepts::details::
  base_unit_extactor<U, std::enable_if_t<util::quantities::concepts::is_base_unit_v<U>>> {
  using type = U;
};

//------------------------------------------------------------------------------
//--- util::quantities::concepts::Prefix
//------------------------------------------------------------------------------
template <typename R>
constexpr auto util::quantities::concepts::Prefix<R>::names(bool Long /* = false */)
{
  if constexpr (std::is_same<ratio, std::exa>()) return Long ? "peta"sv : "E"sv;
  if constexpr (std::is_same<ratio, std::peta>()) return Long ? "peta"sv : "P"sv;
  if constexpr (std::is_same<ratio, std::tera>()) return Long ? "tera"sv : "T"sv;
  if constexpr (std::is_same<ratio, std::giga>()) return Long ? "giga"sv : "G"sv;
  if constexpr (std::is_same<ratio, std::mega>()) return Long ? "mega"sv : "M"sv;
  if constexpr (std::is_same<ratio, std::kilo>()) return Long ? "kilo"sv : "k"sv;
  if constexpr (std::is_same<ratio, std::ratio<1>>()) return ""sv;
  if constexpr (std::is_same<ratio, std::deci>()) return Long ? "deci"sv : "d"sv;
  if constexpr (std::is_same<ratio, std::centi>()) return Long ? "centi"sv : "c"sv;
  if constexpr (std::is_same<ratio, std::milli>()) return Long ? "milli"sv : "m"sv;
  if constexpr (std::is_same<ratio, std::micro>()) return Long ? "micro"sv : "u"sv;
  if constexpr (std::is_same<ratio, std::nano>()) return Long ? "nano"sv : "n"sv;
  if constexpr (std::is_same<ratio, std::pico>()) return Long ? "pico"sv : "p"sv;
  if constexpr (std::is_same<ratio, std::femto>()) return Long ? "femto"sv : "f"sv;
  if constexpr (std::is_same<ratio, std::femto>()) return Long ? "atto"sv : "a"sv;

  // backup; can't use `to_string()` because of `constexpr` requirement
  return Long ? "???"sv : "?"sv;
} // util::quantities::concepts::Prefix<R>::names()

//------------------------------------------------------------------------------
//---  util::quantities::concepts::Quantity
//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr auto util::quantities::concepts::Quantity<U, T>::operator+(
  Quantity<OU, OT> const other) const -> quantity_t
{
  static_assert(std::is_same<Quantity<OU, OT>, quantity_t>(),
                "Only quantities with exactly the same unit can be added.");
  return quantity_t(value() + other.value());
} // util::quantities::concepts::Quantity<>::operator+

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr auto util::quantities::concepts::Quantity<U, T>::operator-(
  Quantity<OU, OT> const other) const -> quantity_t
{
  static_assert(std::is_same<Quantity<OU, OT>, quantity_t>(),
                "Only quantities with exactly the same unit can be subtracted.");
  return quantity_t(value() - other.value());
} // util::quantities::concepts::Quantity<>::operator+

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr auto util::quantities::concepts::Quantity<U, T>::plus(Quantity<OU, OT> const other) const
  -> quantity_t
{
  static_assert(sameBaseUnitAs<OU>(), "Can't add quantities with different base unit");

  // if the two quantities have the same *scaled* unit, add
  if constexpr (sameUnitAs<OU>()) {
    return quantity_t(fValue + other.value());
    return *this;
  }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return (*this + quantity_t(other));
  }
} // util::quantities::concepts::Quantity<>::operator+()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr auto util::quantities::concepts::Quantity<U, T>::minus(Quantity<OU, OT> const other) const
  -> quantity_t
{
  static_assert(sameBaseUnitAs<OU>(), "Can't subtract quantities with different base unit");

  // if the two quantities have the same *scaled* unit, add
  if constexpr (sameUnitAs<OU>()) {
    return quantity_t(fValue - other.value());
    return *this;
  }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return (*this - quantity_t(other));
  }
} // util::quantities::concepts::Quantity<>::minus()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr auto util::quantities::concepts::Quantity<U, T>::operator/(Quantity<OU, OT> const q) const
  -> std::enable_if_t<quantity_t::sameBaseUnitAs<OU>(), value_t>
{
  // if the two quantities have the same *scaled* unit, divide
  if constexpr (sameUnitAs<OU>()) { return value() / q.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return (*this) / quantity_t(q);
  }
} // util::quantities::concepts::Quantity<>::operator/(Quantity)

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
auto util::quantities::concepts::Quantity<U, T>::operator+=(Quantity<OU, OT> const other)
  -> quantity_t&
{
  static_assert(sameBaseUnitAs<OU>(), "Can't add quantities with different base unit");

  // if the two quantities have the same *scaled* unit, add
  if constexpr (sameUnitAs<OU>()) {
    fValue += other.value();
    return *this;
  }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return (*this += quantity_t(other));
  }
} // util::quantities::concepts::Quantity<>::operator+=()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
auto util::quantities::concepts::Quantity<U, T>::operator-=(Quantity<OU, OT> const other)
  -> quantity_t&
{
  static_assert(sameBaseUnitAs<OU>(), "Can't subtract quantities with different base unit");

  // if the two quantities have the same *scaled* unit, add
  if constexpr (sameUnitAs<OU>()) {
    fValue -= other.value();
    return *this;
  }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return (*this -= quantity_t(other));
  }
} // util::quantities::concepts::Quantity<>::operator-=()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr bool util::quantities::concepts::Quantity<U, T>::operator==(
  Quantity<OU, OT> const other) const
{
  static_assert(sameBaseUnitAs<OU>(), "Can't compare quantities with different base unit");

  // if the two quantities have the same *scaled* unit, just compare the values
  if constexpr (sameUnitAs<OU>()) { return value() == other.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return *this == quantity_t(other);
  }
} // util::quantities::concepts::Quantity<>::operator==()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr bool util::quantities::concepts::Quantity<U, T>::operator!=(
  Quantity<OU, OT> const other) const
{
  static_assert(sameBaseUnitAs<OU>(), "Can't compare quantities with different base unit");

  // if the two quantities have the same *scaled* unit, just compare the values
  if constexpr (sameUnitAs<OU>()) { return value() != other.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return *this != quantity_t(other);
  }
} // util::quantities::concepts::Quantity<>::operator!=()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr bool util::quantities::concepts::Quantity<U, T>::operator<=(
  Quantity<OU, OT> const other) const
{
  static_assert(sameBaseUnitAs<OU>(), "Can't compare quantities with different base unit");

  // if the two quantities have the same *scaled* unit, just compare the values
  if constexpr (sameUnitAs<OU>()) { return value() <= other.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return *this <= quantity_t(other);
  }
} // util::quantities::concepts::Quantity<>::operator<=()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr bool util::quantities::concepts::Quantity<U, T>::operator>=(
  Quantity<OU, OT> const other) const
{
  static_assert(sameBaseUnitAs<OU>(), "Can't compare quantities with different base unit");

  // if the two quantities have the same *scaled* unit, just compare the values
  if constexpr (sameUnitAs<OU>()) { return value() >= other.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return *this >= quantity_t(other);
  }
} // util::quantities::concepts::Quantity<>::operator>=()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr bool util::quantities::concepts::Quantity<U, T>::operator<(
  Quantity<OU, OT> const other) const
{
  static_assert(sameBaseUnitAs<OU>(), "Can't compare quantities with different base unit");

  // if the two quantities have the same *scaled* unit, just compare the values
  if constexpr (sameUnitAs<OU>()) { return value() < other.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return *this < quantity_t(other);
  }
} // util::quantities::concepts::Quantity<>::operator<()

//------------------------------------------------------------------------------
template <typename U, typename T>
template <typename OU, typename OT>
constexpr bool util::quantities::concepts::Quantity<U, T>::operator>(
  Quantity<OU, OT> const other) const
{
  static_assert(sameBaseUnitAs<OU>(), "Can't compare quantities with different base unit");

  // if the two quantities have the same *scaled* unit, just compare the values
  if constexpr (sameUnitAs<OU>()) { return value() > other.value(); }
  else {
    // otherwise, they have same base unit but different scale: convert `other`
    return *this > quantity_t(other);
  }
} // util::quantities::concepts::Quantity<>::operator>()

//------------------------------------------------------------------------------
namespace util::quantities::details {

  /**
   * @brief Parses the unit of a string representing a `Quantity`.
   * @tparam Quantity the quantity being represented
   * @param str the string to be parsed
   * @param unitOptional (default: `false`) whether unit is not required
   * @return a pair: the unparsed part of `str` and the factor for parsed unit
   * @throw MissingUnit `s` does not contain the required unit
   * @throw ValueError the numerical value in `s` is not parseable
   * @throw ExtraCharactersError spurious characters after the numeric value
   *                             (including an unrecognised unit prefix)
   */
  template <typename Quantity>
  std::pair<std::string, typename Quantity::value_t> readUnit(std::string const& str,
                                                              bool unitOptional = false);

} // util::quantities::details

//------------------------------------------------------------------------------
template <typename Quantity>
std::pair<std::string, typename Quantity::value_t> util::quantities::details::readUnit(
  std::string const& str,
  bool unitOptional /* = false */)
{
  using Quantity_t = Quantity;
  using value_t = typename Quantity_t::value_t;
  using unit_t = typename Quantity_t::unit_t;
  using baseunit_t = typename unit_t::baseunit_t;

  // --- BEGIN -- static initialization ----------------------------------------
  using namespace std::string_literals;

  using PrefixMap_t = std::map<std::string, value_t>;
  using PrefixValue_t = typename PrefixMap_t::value_type;
  static PrefixMap_t const factors{PrefixValue_t{"a"s, 1e-18},
                                   PrefixValue_t{"f"s, 1e-15},
                                   PrefixValue_t{"p"s, 1e-12},
                                   PrefixValue_t{"n"s, 1e-09},
                                   PrefixValue_t{"u"s, 1e-06},
                                   PrefixValue_t{"m"s, 1e-03},
                                   PrefixValue_t{"c"s, 1e-02},
                                   PrefixValue_t{"d"s, 1e-01},
                                   PrefixValue_t{""s, 1e+00},
                                   PrefixValue_t{"da"s, 1e+01},
                                   PrefixValue_t{"h"s, 1e+02},
                                   PrefixValue_t{"k"s, 1e+03},
                                   PrefixValue_t{"M"s, 1e+06},
                                   PrefixValue_t{"G"s, 1e+09},
                                   PrefixValue_t{"T"s, 1e+12},
                                   PrefixValue_t{"P"s, 1e+15},
                                   PrefixValue_t{"E"s, 1e+18}}; // factors
  static auto const composePrefixPattern = [](auto b, auto e) -> std::string {
    std::string pattern = "(";
    if (b != e) {
      pattern += b->first;
      while (++b != e) {
        pattern += '|';
        pattern += b->first;
      }
    }
    return pattern += ")";
  };
  static std::string const prefixPattern = composePrefixPattern(factors.begin(), factors.end());
  // --- END -- static initialization ------------------------------------------

  std::regex const unitPattern{"[[:blank:]]*(" + prefixPattern + "?" +
                               util::to_string(baseunit_t::symbol) + ")[[:blank:]]*$"};

  std::smatch unitMatch;
  if (!std::regex_search(str, unitMatch, unitPattern)) {
    if (!unitOptional) {
      throw MissingUnit("Unit is mandatory and must derive from '" +
                        util::to_string(baseunit_t::symbol) + "' (parsing: '" + str + "')");
    }
    return {str, value_t{1}};
  }

  //
  // we do have a unit:
  //

  // " 7 cm " => [0] full match (" cm ") [1] unit ("cm") [2] unit prefix ("c")
  auto const iFactor = factors.find(unitMatch.str(2U));
  if (iFactor == factors.end()) {
    throw InvalidUnitPrefix("Unit '" + unitMatch.str(1U) + "' has unsupported prefix '" +
                            unitMatch.str(2U) + "' (parsing '" + str + "')");
  }

  return {str.substr(0U, str.length() - unitMatch.length()),
          static_cast<value_t>(unit_t::scale(iFactor->second))};

} // util::quantities::details::readUnit()

//------------------------------------------------------------------------------
template <typename Quantity>
Quantity util::quantities::makeQuantity(std::string const& s, bool unitOptional /* = false */)
{
  //
  // all this function is horrible;
  // some redesign is needed...
  //
  using value_t = typename Quantity::value_t;

  auto const [num_s, factor] = details::readUnit<Quantity>(s, unitOptional);

  char* parseEnd = nullptr;
  auto const value = static_cast<value_t>(std::strtod(num_s.c_str(), &parseEnd));
  const char* send = num_s.c_str() + num_s.length();
  if (parseEnd == num_s.c_str()) {
    throw ValueError("Could not convert '" + num_s + "' into a number!");
  }
  while (parseEnd != send) {
    if (!std::isblank(static_cast<unsigned char>(*parseEnd))) {
      throw ExtraCharactersError("Spurious characters after value " + std::to_string(value) +
                                 " in '" + num_s + "' ('" + std::string(parseEnd, send - parseEnd) +
                                 "')\n");
    }
    ++parseEnd;
  } // while

  //
  // create and return the quantity
  //
  return Quantity{static_cast<value_t>(value * factor)};
} // util::quantities::makeQuantity(string_view)

//------------------------------------------------------------------------------
template <typename Quantity>
Quantity util::quantities::makeQuantity(std::string_view s, bool unitOptional /* = false */)
{
  return util::quantities::makeQuantity<Quantity>(std::string{s.begin(), s.end()}, unitOptional);
} // util::quantities::makeQuantity(string_view)

//------------------------------------------------------------------------------
template <typename Quantity>
Quantity util::quantities::makeQuantity(char const* s, bool unitOptional /* = false */)
{
  return util::quantities::makeQuantity<Quantity>(std::string_view{s}, unitOptional);
} // util::quantities::makeQuantity(string)

//------------------------------------------------------------------------------
//---  Standard library extensions
//------------------------------------------------------------------------------
namespace std {

  // ---------------------------------------------------------------------------
  /// Hash function of a quantity is delegated to its value
  template <typename... Args>
  struct hash<util::quantities::concepts::Quantity<Args...>> {
  private:
    using quantity_t = util::quantities::concepts::Quantity<Args...>;
    using value_t = typename quantity_t::value_t;

  public:
    constexpr auto operator()(quantity_t key) const
      noexcept(noexcept(std::hash<value_t>()(key.value())))
    {
      return std::hash<value_t>()(key.value());
    }
  };

  // ---------------------------------------------------------------------------
  /// Limits of a quantity are the same as the underlying type.
  template <typename Unit, typename T>
  class numeric_limits<util::quantities::concepts::Quantity<Unit, T>>
    : public util::quantities::concepts::details::numeric_limits<
        util::quantities::concepts::Quantity<Unit, T>> {};

  template <typename Unit, typename T>
  class numeric_limits<util::quantities::concepts::Quantity<Unit, T> const>
    : public util::quantities::concepts::details::numeric_limits<
        util::quantities::concepts::Quantity<Unit, T> const> {};

  template <typename Unit, typename T>
  class numeric_limits<util::quantities::concepts::Quantity<Unit, T> volatile>
    : public util::quantities::concepts::details::numeric_limits<
        util::quantities::concepts::Quantity<Unit, T> volatile> {};

  template <typename Unit, typename T>
  class numeric_limits<util::quantities::concepts::Quantity<Unit, T> const volatile>
    : public util::quantities::concepts::details::numeric_limits<
        util::quantities::concepts::Quantity<Unit, T> const volatile> {};

  // ---------------------------------------------------------------------------

} // namespace std

//------------------------------------------------------------------------------
//--- products of quantities implementation

#include "details/quantity_products.h"

//------------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_H
