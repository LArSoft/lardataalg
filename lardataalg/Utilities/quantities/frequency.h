/**
 * @file   lardataalg/Utilities/quantities/frequency.h
 * @brief  Dimensioned variables representing frequency quantities.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   October 30, 2018
 * @see    lardataalg/Utilities/quantities.h,
 *         lardataalg/Utilities/quantities/spacetime.h
 *
 * Set of quantities related to frequency (inverse of time). Currently,
 * quantities are defined based on the following units:
 * * hertz (Hz, kHz, MHz, GHz)
 *
 * @note If the following headers are loaded, relationship between these and
 *       some of their quantities are registered:
 *       * `spacetime.h` (e.g. s·Hz = 1).
 *
 *
 * This is a header-only library.
 *
 */

#ifndef LARDATAALG_UTILITIES_QUANTITIES_FREQUENCY_H
#define LARDATAALG_UTILITIES_QUANTITIES_FREQUENCY_H

// LArSoft libraries
#include "lardataalg/Utilities/quantities.h"

// C/C++ standard libraries
#include <ratio>
#include <string_view>

//------------------------------------------------------------------------------
namespace util::quantities {

  namespace units {

    using namespace std::string_view_literals; // for operator""sv()

    struct Hertz : public concepts::UnitBase {
      static constexpr auto symbol = "Hz"sv;
      static constexpr auto name = "hertz"sv;
    };

  } // namespace units

  // -- BEGIN Frequency --------------------------------------------------------
  /**
   * @name Frequency quantities
   *
   * These frequency quantities are tied to `util::quantities::units::Hertz`.
   * A few options are provided:
   *
   * * most general template, `scaled_hertz`, allowing to choose both the scale
   *     of the unit (e.g. `std::kilo` for kilohertz) and the type of the
   *     numerical representation
   * * generic template (e.g. `hertz_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `hertz`), ready for use
   *
   * @note If `spacetime.h` header is loaded, additional relationships hold:
   *       * `Second` * `Hertz` = constant (and similar)
   */
  /// @{

  /// The most generic `units::Hertz`-based quantity.
  template <typename R, typename T = double>
  using scaled_hertz = concepts::scaled_quantity<units::Hertz, R, T>;

  //
  // hertz
  //
  /// Type of frequency stored in hertz.
  template <typename T = double>
  using hertz_as = scaled_hertz<std::ratio<1>, T>;

  /// Type of frequency stored in hertz, in double precision.
  using hertz = hertz_as<>;

  //
  // kilohertz
  //
  /// Type of frequency stored in kilohertz.
  template <typename T = double>
  using kilohertz_as = concepts::rescale<hertz_as<T>, std::kilo>;

  /// Type of frequency stored in kilohertz, in double precision.
  using kilohertz = kilohertz_as<>;

  //
  // megahertz
  //
  /// Type of frequency stored in megahertz.
  template <typename T = double>
  using megahertz_as = concepts::rescale<hertz_as<T>, std::mega>;

  /// Type of frequency stored in megahertz, in double precision.
  using megahertz = megahertz_as<>;

  //
  // gigahertz
  //
  /// Type of frequency stored in gigahertz.
  template <typename T = double>
  using gigahertz_as = concepts::rescale<hertz_as<T>, std::giga>;

  /// Type of frequency stored in gigahertz, in double precision.
  using gigahertz = gigahertz_as<>;

  /**
   * @brief Literal constants for frequency quantities.
   *
   * These functions allow a simplified syntax for specifying a frequency
   * quantity. In order to use these, their namespace must be used:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using namespace util::quantities::frequency_literals;
   *
   * // definition of `util::quantities::hertz` constant:
   * constexpr auto f_Hz = 12_Hz;
   *
   * // assignment (likely to a quantity) of `util::quantities::megahertz{50.0}`
   * f_Hz = 50_MHz;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   */
  namespace frequency_literals {

    // @{
    /// Literal hertz value.
    constexpr hertz operator""_Hz(long double v)
    {
      return hertz{static_cast<double>(v)};
    }
    constexpr hertz operator""_Hz(unsigned long long int v)
    {
      return hertz{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal kilohertz value.
    constexpr kilohertz operator""_kHz(long double v)
    {
      return kilohertz{static_cast<double>(v)};
    }
    constexpr kilohertz operator""_kHz(unsigned long long int v)
    {
      return kilohertz{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal megahertz value.
    constexpr megahertz operator""_MHz(long double v)
    {
      return megahertz{static_cast<double>(v)};
    }
    constexpr megahertz operator""_MHz(unsigned long long int v)
    {
      return megahertz{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal gigahertz value.
    constexpr gigahertz operator""_GHz(long double v)
    {
      return gigahertz{static_cast<double>(v)};
    }
    constexpr gigahertz operator""_GHz(unsigned long long int v)
    {
      return gigahertz{static_cast<double>(v)};
    }
    // @}

  } // namespace frequency_literals

  /// @}
  // -- END Frequency ----------------------------------------------------------

} // namespace util::quantities

// --- BEGIN Special relations -------------------------------------------------

// unit relations (they are defined in `util::quantities::concepts::details`,
//   so the `util::quantities::units` namespace can be shortened)

#ifdef LARDATAALG_UTILITIES_QUANTITIES_SPACETIME_H
UTIL_QUANTITIES_UNITPRODUCT(units::Second, units::Hertz, units::Unity);
#endif

// --- END Special relations ---------------------------------------------------

//------------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_FREQUENCY_H
