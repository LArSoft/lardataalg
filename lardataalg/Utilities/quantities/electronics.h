/**
 * @file   lardataalg/Utilities/quantities/electronics.h
 * @brief  Dimensioned variables related to electronics.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   November 2, 2018
 * @see    lardataalg/Utilities/quantities.h
 *
 * Set of basic quantities related to electronics. Currently, quantities are
 * defined based on the following units:
 * * tick
 *
 * This is a header-only library.
 *
 */

#ifndef LARDATAALG_UTILITIES_QUANTITIES_ELECTRONICS_H
#define LARDATAALG_UTILITIES_QUANTITIES_ELECTRONICS_H

// LArSoft libraries
#include "lardataalg/Utilities/intervals.h"
#include "lardataalg/Utilities/quantities.h"

// C/C++ standard libraries
#include <cstddef> // std::ptrdiff_t
#include <ratio>
#include <string_view>

//------------------------------------------------------------------------------
namespace util::quantities {

  namespace units {

    using namespace std::string_view_literals; // for operator""sv()

    struct Tick : public concepts::UnitBase {
      static constexpr auto symbol = "#"sv;
      static constexpr auto name = "tick"sv;
    };

    struct Counts : public concepts::UnitBase {
      static constexpr auto symbol = "#"sv;
      static constexpr auto name = "counts"sv;
    };

  } // namespace units

  // -- BEGIN Ticks ------------------------------------------------------------
  /**
   * @name Ticks
   *
   * These tick quantities are tied to `util::quantities::units::Tick`.
   * A few options are provided:
   *
   * * generic template (`tick_as`), allowing to choose which numerical
   *     representation to use
   * * unsigned integer (`tick`), based on `std::ptrdiff_t`, ready for use
   *
   * For this unit in particular, additional options are provided to accommodate
   * the custom of using the unit in plural form: `ticks_as` and `ticks`
   * are exactly equivalent to the singular-named counterparts.
   */
  /// @{

  /// Tick number, represented by the specified type `T`.
  template <typename T = std::ptrdiff_t>
  using tick_as = concepts::scaled_quantity<units::Tick, std::ratio<1>, T>;

  /// Alias for common language habits.
  template <typename T = tick_as<>::value_t>
  using ticks_as = tick_as<T>;

  /// Tick number, represented by `std::ptrdiff_t`.
  using tick = tick_as<>;

  /// Alias for common language habits.
  using ticks = tick;

  /// Tick number, represented by `float`.
  using tick_f = tick_as<float>;

  /// Alias for common language habits.
  using ticks_f = tick_f;

  /// Tick number, represented by `double`.
  using tick_d = tick_as<double>;

  /// Alias for common language habits.
  using ticks_d = tick_d;

  /// @}
  // -- END Ticks --------------------------------------------------------------

  // -- BEGIN ADC counts -------------------------------------------------------
  /**
   * @name ADC counts
   *
   * These ADC count quantities are tied to `util::quantities::units::Counts`.
   * A few options are provided:
   *
   * * generic template (`counts_as`), allowing to choose which numerical
   *     representation to use
   * * unsigned integer (`counts`), based on `signed short int`, ready for use
   *
   */
  /// @{

  /// Number of ADC counts, represented by the specified type `T`.
  template <typename T = signed short int>
  using counts_as = concepts::scaled_quantity<units::Counts, std::ratio<1>, T>;

  /// Number of ADC counts, represented by `signed short int`.
  using counts = counts_as<>;

  /// Number of ADC counts, represented by `float`.
  using counts_f = counts_as<float>;

  // -- END ADC counts ---------------------------------------------------------

  /**
   * @brief Literal constants for electronics quantities.
   *
   * These functions allow a simplified syntax for specifying a tick quantity.
   * In order to use these, their namespace must be used:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using namespace util::quantities::electronics_literals;
   *
   * // definition of `util::quantities::tick` constant:
   * constexpr auto i = 56_tick;
   *
   * // definition of `util::quantities::counts` constant:
   * constexpr auto q = 675_ADC;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   */
  namespace electronics_literals {

    // @{
    /// Literal tick value.
    constexpr tick operator""_tick(long double v)
    {
      return tick::castFrom(v);
    }
    constexpr tick operator""_tick(unsigned long long int v)
    {
      return tick::castFrom(v);
    }
    // @}

    // @{
    /// Literal tick (`double`-based, `tick_d`) value.
    constexpr tick_d operator""_tickd(long double v)
    {
      return tick_d::castFrom(v);
    }
    constexpr tick_d operator""_tickd(unsigned long long int v)
    {
      return tick_d::castFrom(v);
    }
    // @}

    // @{
    /// Literal ADC count value.
    constexpr counts operator""_ADC(long double v)
    {
      return counts{static_cast<signed short int>(v)};
    }
    constexpr counts operator""_ADC(unsigned long long int v)
    {
      return counts{static_cast<signed short int>(v)};
    }
    // @}

    // @{
    /// Literal ADC count value (single precision floating points).
    constexpr counts_f operator""_ADCf(long double v)
    {
      return counts_f{static_cast<float>(v)};
    }
    constexpr counts_f operator""_ADCf(unsigned long long int v)
    {
      return counts_f{static_cast<float>(v)};
    }
    // @}

  } // electronics_literals

  // --- BEGIN Tick intervals --------------------------------------------------

  namespace intervals {

    /// A `units::Ticks`-based interval.
    template <typename T = util::quantities::tick_as<>::value_t>
    using ticks_as = concepts::Interval<util::quantities::tick_as<T>>;

    /// A tick interval based on `std::ptrdiff_t`.
    using ticks = ticks_as<>;

    /// A tick interval based on single precision real number.
    using ticks_f = ticks_as<float>;

    /// A tick interval based on double precision real number.
    using ticks_d = ticks_as<double>;

  } // namespace intervals

  // --- END Time intervals ----------------------------------------------------

  // --- BEGIN Time points -----------------------------------------------------

  namespace points {

    /// A `units::Ticks`-based point.
    template <typename T = util::quantities::tick_as<>::value_t, typename Cat = NoCategory>
    using tick_as = concepts::Point<util::quantities::tick_as<T>, Cat>;

    /// A tick value based on `std::ptrdiff_t`.
    using tick = tick_as<>;

    /// A tick value based on single precision real number.
    using tick_f = tick_as<float>;

    /// A tick value based on double precision real number.
    using tick_d = tick_as<double>;

  } // namespace points

  // --- END Tick points -------------------------------------------------------

  /// @}

} // namespace util::quantities

//------------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_ELECTRONICS_H
