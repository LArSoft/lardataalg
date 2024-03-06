/**
 * @file   lardataalg/Utilities/quantities/spacetime.h
 * @brief  Dimensioned variables representing space or time quantities.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   October 30, 2018
 * @see    lardataalg/Utilities/quantities.h
 *
 * Set of basic quantities related to space and time. Currently, quantities are
 * defined based on the following units:
 * * seconds (ps, ns, us, ms, s), with intervals in `intervals` namespace
 *
 * This is a header-only library.
 *
 */

#ifndef LARDATAALG_UTILITIES_QUANTITIES_SPACETIME_H
#define LARDATAALG_UTILITIES_QUANTITIES_SPACETIME_H

// LArSoft libraries
#include "lardataalg/Utilities/intervals.h"
#include "lardataalg/Utilities/quantities.h"

// C/C++ standard libraries
#include <ratio>
#include <string_view>

//------------------------------------------------------------------------------
namespace util::quantities {

  namespace units {

    using namespace std::string_view_literals; // for operator""sv()

    struct Second : public concepts::UnitBase {
      static constexpr auto symbol = "s"sv;
      static constexpr auto name = "second"sv;
    };

    struct Meter : public concepts::UnitBase {
      static constexpr auto symbol = "m"sv;
      static constexpr auto name = "meter"sv;
    };

  } // namespace units

  // -- BEGIN Time -------------------------------------------------------------
  /**
   * @name Time quantities
   *
   * These time quantities are tied to `util::quantities::units::Second`.
   * A few options are provided:
   *
   * * most general template, `scaled_second`, allowing to choose both the scale
   *     of the unit (e.g. `std::milli` for milliseconds) and the type of the
   *     numerical representation
   * * generic templates (e.g. `second_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `second`), ready for use
   *
   * For this unit in particular, additional options are provided to accommodate
   * the custom of using the unit in plural form: `seconds_as` and `seconds`
   * are exactly equivalent to the singular-named counterparts.
   */
  /// @{

  /// The most generic `units::Second`-based quantity.
  template <typename R, typename T = double>
  using scaled_second = concepts::scaled_quantity<units::Second, R, T>;

  //
  // seconds
  //
  /// Type of time stored in seconds.
  template <typename T = double>
  using second_as = scaled_second<std::ratio<1>, T>;

  /// Alias for common language habits.
  template <typename T = double>
  using seconds_as = second_as<T>;

  /// Type of time stored in seconds, in double precision.
  using second = second_as<>;

  /// Alias for common language habits.
  using seconds = second;

  //
  // milliseconds
  //
  /// Type of time stored in milliseconds.
  template <typename T = double>
  using millisecond_as = concepts::rescale<second_as<T>, std::milli>;

  /// Alias for common language habits.
  template <typename T = double>
  using milliseconds_as = millisecond_as<T>;

  /// Type of time stored in milliseconds, in double precision.
  using millisecond = millisecond_as<>;

  /// Alias for common language habits.
  using milliseconds = millisecond;

  //
  // microseconds
  //
  /// Type of time stored in microseconds.
  template <typename T = double>
  using microsecond_as = concepts::rescale<second_as<T>, std::micro>;

  /// Alias for common language habits.
  template <typename T = double>
  using microseconds_as = microsecond_as<T>;

  /// Type of time stored in microseconds, in double precision.
  using microsecond = microsecond_as<>;

  /// Alias for common language habits.
  using microseconds = microsecond;

  //
  // nanoseconds
  //
  /// Type of time stored in nanoseconds.
  template <typename T = double>
  using nanosecond_as = concepts::rescale<second_as<T>, std::nano>;

  /// Alias for common language habits.
  template <typename T = double>
  using nanoseconds_as = nanosecond_as<T>;

  /// Type of time stored in nanoseconds, in double precision.
  using nanosecond = nanosecond_as<>;

  /// Alias for common language habits.
  using nanoseconds = nanosecond;

  //
  // picoseconds
  //
  /// Type of time stored in picoseconds.
  template <typename T = double>
  using picosecond_as = concepts::rescale<second_as<T>, std::pico>;

  /// Alias for common language habits.
  template <typename T = double>
  using picoseconds_as = picosecond_as<T>;

  /// Type of time stored in picoseconds, in double precision.
  using picosecond = picosecond_as<>;

  /// Alias for common language habits.
  using picoseconds = picosecond;

  /**
   * @brief Literal constants for time quantities.
   *
   * These functions allow a simplified syntax for specifying a time quantity.
   * In order to use these, their namespace must be used:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using namespace util::quantities::time_literals;
   *
   * // definition of `util::quantities::second` constant:
   * constexpr auto t_s = 12_s;
   *
   * // assignment (likely to a quantity) of
   * // `util::quantities::millisecond{500.0}`
   * t_s = 500_ms;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   */
  namespace time_literals {

    // @{
    /// Literal second value.
    constexpr second operator""_s(long double v)
    {
      return second{static_cast<double>(v)};
    }
    constexpr second operator""_s(unsigned long long int v)
    {
      return second{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal millisecond value.
    constexpr millisecond operator""_ms(long double v)
    {
      return millisecond{static_cast<double>(v)};
    }
    constexpr millisecond operator""_ms(unsigned long long int v)
    {
      return millisecond{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal microsecond value.
    constexpr microsecond operator""_us(long double v)
    {
      return microsecond{static_cast<double>(v)};
    }
    constexpr microsecond operator""_us(unsigned long long int v)
    {
      return microsecond{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal nanosecond value.
    constexpr nanosecond operator""_ns(long double v)
    {
      return nanosecond{static_cast<double>(v)};
    }
    constexpr nanosecond operator""_ns(unsigned long long int v)
    {
      return nanosecond{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal picosecond value.
    constexpr picosecond operator""_ps(long double v)
    {
      return picosecond{static_cast<double>(v)};
    }
    constexpr picosecond operator""_ps(unsigned long long int v)
    {
      return picosecond{static_cast<double>(v)};
    }
    // @}

  } // time_literals

  // --- BEGIN Time intervals --------------------------------------------------

  namespace intervals {

    /// The most generic `units::Second`-based interval.
    template <typename R, typename T = double>
    using scaled_seconds = concepts::Interval<util::quantities::scaled_second<R, T>>;

    //
    // seconds
    //
    /// Type of time interval stored in seconds.
    template <typename T = double>
    using seconds_as = scaled_seconds<std::ratio<1>, T>;

    /// Type of time stored in seconds, in double precision.
    using seconds = seconds_as<>;

    //
    // milliseconds
    //

    /// Type of time interval stored in milliseconds.
    template <typename T = double>
    using milliseconds_as = scaled_seconds<std::milli, T>;

    /// Type of time interval stored in milliseconds, in double precision.
    using milliseconds = milliseconds_as<>;

    //
    // microseconds
    //

    /// Type of time interval stored in microseconds.
    template <typename T = double>
    using microseconds_as = scaled_seconds<std::micro, T>;

    /// Type of time interval stored in microseconds, in double precision.
    using microseconds = microseconds_as<>;

    //
    // nanoseconds
    //

    /// Type of time interval stored in nanoseconds.
    template <typename T = double>
    using nanoseconds_as = scaled_seconds<std::nano, T>;

    /// Type of time interval stored in nanoseconds, in double precision.
    using nanoseconds = nanoseconds_as<>;

    //
    // picoseconds
    //

    /// Type of time interval stored in picoseconds.
    template <typename T = double>
    using picoseconds_as = scaled_seconds<std::pico, T>;

    /// Type of time interval stored in picoseconds, in double precision.
    using picoseconds = picoseconds_as<>;

  } // namespace intervals

  // --- END Time intervals ----------------------------------------------------

  // --- BEGIN Time points -----------------------------------------------------

  namespace points {

    /// The most generic `units::Second`-based point.
    template <typename R, typename T = double, typename Cat = NoCategory>
    using scaled_second = concepts::Point<util::quantities::scaled_second<R, T>, Cat>;

    //
    // second
    //
    /// Type of time point stored in seconds.
    template <typename T = double, typename Cat = NoCategory>
    using second_as = scaled_second<std::ratio<1>, T, Cat>;

    /// Type of time point stored in seconds, in double precision.
    using second = second_as<>;

    //
    // millisecond
    //

    /// Type of time point stored in milliseconds.
    template <typename T = double, typename Cat = NoCategory>
    using millisecond_as = scaled_second<std::milli, T, Cat>;

    /// Type of time point stored in milliseconds, in double precision.
    using millisecond = millisecond_as<>;

    //
    // microsecond
    //

    /// Type of time point stored in microseconds.
    template <typename T = double, typename Cat = NoCategory>
    using microsecond_as = scaled_second<std::micro, T, Cat>;

    /// Type of time point stored in microseconds, in double precision.
    using microsecond = microsecond_as<>;

    //
    // nanosecond
    //

    /// Type of time point stored in nanoseconds.
    template <typename T = double, typename Cat = NoCategory>
    using nanosecond_as = scaled_second<std::nano, T, Cat>;

    /// Type of time point stored in nanoseconds, in double precision.
    using nanosecond = nanosecond_as<>;

    //
    // picosecond
    //

    /// Type of time point stored in picoseconds.
    template <typename T = double, typename Cat = NoCategory>
    using picosecond_as = scaled_second<std::pico, T, Cat>;

    /// Type of time point stored in picoseconds, in double precision.
    using picosecond = picosecond_as<>;

  } // namespace points

  // --- END Time points -------------------------------------------------------

  /// @}
  // -- END Time ---------------------------------------------------------------

  // -- BEGIN Space ------------------------------------------------------------
  /**
   * @name Linear space quantities
   *
   * These space quantities are tied to `util::quantities::units::Meter`.
   * A few options are provided:
   *
   * * most general template, `scaled_meter`, allowing to choose both the scale
   *     of the unit (e.g. `std::milli` for millimeters) and the type of the
   *     numerical representation
   * * generic templates (e.g. `meter_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `meter`), ready for use
   *
   * For this unit in particular, additional options are provided to accommodate
   * the custom of using the unit in plural form: `meters_as` and `meters`
   * are exactly equivalent to the singular-named counterparts.
   */
  /// @{

  /// The most generic `units::Meter`-based quantity.
  template <typename R, typename T = double>
  using scaled_meter = concepts::scaled_quantity<units::Meter, R, T>;

  //
  // meters
  //
  /// Type of space stored in meters.
  template <typename T = double>
  using meter_as = scaled_meter<std::ratio<1>, T>;

  /// Alias for common language habits.
  template <typename T = double>
  using meters_as = meter_as<T>;

  /// Type of space stored in meters, in double precision.
  using meter = meter_as<>;

  /// Alias for common language habits.
  using meters = meter;

  //
  // kilometers
  //
  /// Type of space stored in kilometers.
  template <typename T = double>
  using kilometer_as = concepts::rescale<meter_as<T>, std::kilo>;

  /// Alias for common language habits.
  template <typename T = double>
  using kilometers_as = kilometer_as<T>;

  /// Type of space stored in kilometers, in double precision.
  using kilometer = kilometer_as<>;

  /// Alias for common language habits.
  using kilometers = kilometer;

  //
  // centimeters
  //
  /// Type of space stored in centimeters.
  template <typename T = double>
  using centimeter_as = concepts::rescale<meter_as<T>, std::centi>;

  /// Alias for common language habits.
  template <typename T = double>
  using centimeters_as = centimeter_as<T>;

  /// Type of space stored in centimeters, in double precision.
  using centimeter = centimeter_as<>;

  /// Alias for common language habits.
  using centimeters = centimeter;

  //
  // millimeters
  //
  /// Type of space stored in millimeters.
  template <typename T = double>
  using millimeter_as = concepts::rescale<meter_as<T>, std::milli>;

  /// Alias for common language habits.
  template <typename T = double>
  using millimeters_as = millimeter_as<T>;

  /// Type of space stored in millimeters, in double precision.
  using millimeter = millimeter_as<>;

  /// Alias for common language habits.
  using millimeters = millimeter;

  //
  // micrometers
  //
  /// Type of space stored in micrometers.
  template <typename T = double>
  using micrometer_as = concepts::rescale<meter_as<T>, std::micro>;

  /// Alias for common language habits.
  template <typename T = double>
  using micrometers_as = micrometer_as<T>;

  /// Type of space stored in micrometers, in double precision.
  using micrometer = micrometer_as<>;

  /// Alias for common language habits.
  using micrometers = micrometer;

  //
  // nanometers
  //
  /// Type of space stored in nanometers.
  template <typename T = double>
  using nanometer_as = concepts::rescale<meter_as<T>, std::nano>;

  /// Alias for common language habits.
  template <typename T = double>
  using nanometers_as = nanometer_as<T>;

  /// Type of space stored in nanometers, in double precision.
  using nanometer = nanometer_as<>;

  /// Alias for common language habits.
  using nanometers = nanometer;

  //
  // picometers
  //
  /// Type of space stored in picometers.
  template <typename T = double>
  using picometer_as = concepts::rescale<meter_as<T>, std::pico>;

  /// Alias for common language habits.
  template <typename T = double>
  using picometers_as = picometer_as<T>;

  /// Type of space stored in picometers, in double precision.
  using picometer = picometer_as<>;

  /// Alias for common language habits.
  using picometers = picometer;

  //
  // femtometers
  //
  /// Type of space stored in femtometers.
  template <typename T = double>
  using femtometer_as = concepts::rescale<meter_as<T>, std::femto>;

  /// Alias for common language habits.
  template <typename T = double>
  using femtometers_as = femtometer_as<T>;

  /// Type of space stored in femtometers, in double precision.
  using femtometer = femtometer_as<>;

  /// Alias for common language habits.
  using femtometers = femtometer;

  /**
   * @brief Literal constants for space quantities.
   *
   * These functions allow a simplified syntax for specifying a space quantity.
   * In order to use these, their namespace must be used:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using namespace util::quantities::space_literals;
   *
   * // definition of `util::quantities::meter` constant:
   * constexpr auto d_m = 12_m;
   *
   * [...]
   *
   * // assignment (likely to a quantity) of
   * // `util::quantities::centimeter{500.0}`
   * d_m = 500_cm;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   */
  namespace space_literals {

    // @{
    /// Literal meter value.
    constexpr meter operator""_m(long double v)
    {
      return meter{static_cast<double>(v)};
    }
    constexpr meter operator""_m(unsigned long long int v)
    {
      return meter{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal kilometer value.
    constexpr kilometer operator""_km(long double v)
    {
      return kilometer{static_cast<double>(v)};
    }
    constexpr kilometer operator""_km(unsigned long long int v)
    {
      return kilometer{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal centimeter value.
    constexpr centimeter operator""_cm(long double v)
    {
      return centimeter{static_cast<double>(v)};
    }
    constexpr centimeter operator""_cm(unsigned long long int v)
    {
      return centimeter{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal millimeter value.
    constexpr millimeter operator""_mm(long double v)
    {
      return millimeter{static_cast<double>(v)};
    }
    constexpr millimeter operator""_mm(unsigned long long int v)
    {
      return millimeter{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal micrometer value.
    constexpr micrometer operator""_um(long double v)
    {
      return micrometer{static_cast<double>(v)};
    }
    constexpr micrometer operator""_um(unsigned long long int v)
    {
      return micrometer{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal nanometer value.
    constexpr nanometer operator""_nm(long double v)
    {
      return nanometer{static_cast<double>(v)};
    }
    constexpr nanometer operator""_nm(unsigned long long int v)
    {
      return nanometer{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal picometer value.
    constexpr picometer operator""_pm(long double v)
    {
      return picometer{static_cast<double>(v)};
    }
    constexpr picometer operator""_pm(unsigned long long int v)
    {
      return picometer{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal femtometer value.
    constexpr femtometer operator""_fm(long double v)
    {
      return femtometer{static_cast<double>(v)};
    }
    constexpr femtometer operator""_fm(unsigned long long int v)
    {
      return femtometer{static_cast<double>(v)};
    }
    // @}

  } // space_literals

  // --- BEGIN Space intervals -------------------------------------------------

  namespace intervals {

    /// The most generic `units::Meter`-based interval.
    template <typename R, typename T = double>
    using scaled_meters = concepts::Interval<util::quantities::scaled_meter<R, T>>;

    //
    // meters
    //
    /// Type of space interval stored in meters.
    template <typename T = double>
    using meters_as = scaled_meters<std::ratio<1>, T>;

    /// Type of space stored in meters, in double precision.
    using meters = meters_as<>;

    //
    // kilometers
    //

    /// Type of space interval stored in kilometers.
    template <typename T = double>
    using kilometers_as = scaled_meters<std::kilo, T>;

    /// Type of space interval stored in kilometers, in double precision.
    using kilometers = kilometers_as<>;

    //
    // centimeters
    //

    /// Type of space interval stored in centimeters.
    template <typename T = double>
    using centimeters_as = scaled_meters<std::centi, T>;

    /// Type of space interval stored in centimeters, in double precision.
    using centimeters = centimeters_as<>;

    //
    // millimeters
    //

    /// Type of space interval stored in millimeters.
    template <typename T = double>
    using millimeters_as = scaled_meters<std::milli, T>;

    /// Type of space interval stored in millimeters, in double precision.
    using millimeters = millimeters_as<>;

    //
    // micrometers
    //

    /// Type of space interval stored in micrometers.
    template <typename T = double>
    using micrometers_as = scaled_meters<std::micro, T>;

    /// Type of space interval stored in micrometers, in double precision.
    using micrometers = micrometers_as<>;

    //
    // nanometers
    //

    /// Type of space interval stored in nanometers.
    template <typename T = double>
    using nanometers_as = scaled_meters<std::nano, T>;

    /// Type of space interval stored in nanometers, in double precision.
    using nanometers = nanometers_as<>;

    //
    // picometers
    //

    /// Type of space interval stored in picometers.
    template <typename T = double>
    using picometers_as = scaled_meters<std::pico, T>;

    /// Type of space interval stored in picometers, in double precision.
    using picometers = picometers_as<>;

    //
    // femtometers
    //

    /// Type of space interval stored in femtometers.
    template <typename T = double>
    using femtometers_as = scaled_meters<std::femto, T>;

    /// Type of space interval stored in femtometers, in double precision.
    using femtometers = femtometers_as<>;

  } // namespace intervals

  // --- END Space intervals ---------------------------------------------------

  // --- BEGIN Space points ----------------------------------------------------

  namespace points {

    /// The most generic `units::Meter`-based point.
    template <typename R, typename T = double, typename Cat = NoCategory>
    using scaled_meter = concepts::Point<util::quantities::scaled_meter<R, T>, Cat>;

    //
    // meter
    //
    /// Type of space point stored in meters.
    template <typename T = double, typename Cat = NoCategory>
    using meter_as = scaled_meter<std::ratio<1>, T, Cat>;

    /// Type of space point stored in meters, in double precision.
    using meter = meter_as<>;

    //
    // kilometer
    //

    /// Type of space point stored in kilometers.
    template <typename T = double, typename Cat = NoCategory>
    using kilometer_as = scaled_meter<std::kilo, T, Cat>;

    /// Type of space point stored in kilometers, in double precision.
    using kilometer = kilometer_as<>;

    //
    // centimeter
    //

    /// Type of space point stored in centimeters.
    template <typename T = double, typename Cat = NoCategory>
    using centimeter_as = scaled_meter<std::centi, T, Cat>;

    /// Type of space point stored in centimeters, in double precision.
    using centimeter = centimeter_as<>;

    //
    // millimeter
    //

    /// Type of space point stored in millimeters.
    template <typename T = double, typename Cat = NoCategory>
    using millimeter_as = scaled_meter<std::milli, T, Cat>;

    /// Type of space point stored in millimeters, in double precision.
    using millimeter = millimeter_as<>;

    //
    // micrometer
    //

    /// Type of space point stored in micrometers.
    template <typename T = double, typename Cat = NoCategory>
    using micrometer_as = scaled_meter<std::micro, T, Cat>;

    /// Type of space point stored in micrometers, in double precision.
    using micrometer = micrometer_as<>;

    //
    // nanometer
    //

    /// Type of space point stored in nanometers.
    template <typename T = double, typename Cat = NoCategory>
    using nanometer_as = scaled_meter<std::nano, T, Cat>;

    /// Type of space point stored in nanometers, in double precision.
    using nanometer = nanometer_as<>;

    //
    // picometer
    //

    /// Type of space point stored in picometers.
    template <typename T = double, typename Cat = NoCategory>
    using picometer_as = scaled_meter<std::pico, T, Cat>;

    /// Type of space point stored in picometers, in double precision.
    using picometer = picometer_as<>;

    //
    // femtometer
    //

    /// Type of space point stored in femtometers.
    template <typename T = double, typename Cat = NoCategory>
    using femtometer_as = scaled_meter<std::femto, T, Cat>;

    /// Type of space point stored in femtometers, in double precision.
    using femtometer = femtometer_as<>;

  } // namespace points

  // --- END Space points -------------------------------------------------------

  /// @}
  // -- END Space --------------------------------------------------------------

} // namespace util::quantities

//------------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_SPACETIME_H
