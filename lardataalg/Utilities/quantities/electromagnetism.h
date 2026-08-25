/**
 * @file   lardataalg/Utilities/quantities/electromagnetism.h
 * @brief  Dimensioned variables representing electromagnetic quantities.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   November 2, 2018
 * @see    lardataalg/Utilities/quantities.h
 *
 * Set of basic quantities related to electromagnetism. Currently, quantities
 * are defined based on the following units:
 * * coulomb (fC, pC, nC, uC, mC, C)
 * * volt (uV, mV, V, kV, MV, GV)
 * * ampere (pA, nA, uA, mA, A, kA)
 * * ohm (Ω, kΩ, MΩ, GΩ)
 * * farad (fF, pF, nF, uF, mF, F)
 *
 * Some relations between them (e.g. Ω·A = V) are also registered.
 *
 * This is a header-only library.
 *
 * @note If the following headers are loaded, relationship between these and
 *       some of their quantities are registered:
 *       * `spacetime.h` (e.g. Ω·F = s).
 *
 */

#ifndef LARDATAALG_UTILITIES_QUANTITIES_ELECTROMAGNETISM_H
#define LARDATAALG_UTILITIES_QUANTITIES_ELECTROMAGNETISM_H

// LArSoft libraries
#include "lardataalg/Utilities/quantities.h"

// C/C++ standard libraries
#include <ratio>
#include <string_view>

//------------------------------------------------------------------------------
namespace util::quantities {

  namespace units {

    using namespace std::string_view_literals; // for operator""sv()

    struct Coulomb : public concepts::UnitBase {
      static constexpr auto symbol = "C"sv;
      static constexpr auto name = "coulomb"sv;
    };

    struct Volt : public concepts::UnitBase {
      static constexpr auto symbol = "V"sv;
      static constexpr auto name = "volt"sv;
    };

    struct Ampere : public concepts::UnitBase {
      static constexpr auto symbol = "A"sv;
      static constexpr auto name = "ampere"sv;
    };

    struct Ohm : public concepts::UnitBase {
      static constexpr auto symbol = "Ω"sv;
      static constexpr auto name = "ohm"sv;
    };

    struct Farad : public concepts::UnitBase {
      static constexpr auto symbol = "F"sv;
      static constexpr auto name = "farad"sv;
    };

  } // namespace units

  // -- BEGIN Charge -----------------------------------------------------------
  /**
   * @name Charge quantities
   *
   * These charge quantities are tied to `util::quantities::units::Coulomb`.
   * A few options are provided:
   *
   * * most general template, `scaled_coulomb`, allowing to choose both the
   *     scale of the unit (e.g. `std::pico` for picocoulomb) and the type of
   *     the numerical representation
   * * generic templates (e.g. `coulomb_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `coulomb`), ready for use
   *
   */
  /// @{

  /// The most generic `units::Coulomb`-based quantity.
  template <typename R, typename T = double>
  using scaled_coulomb = concepts::scaled_quantity<units::Coulomb, R, T>;

  //
  // coulomb
  //
  /// Type of charge stored in coulomb.
  template <typename T = double>
  using coulomb_as = scaled_coulomb<std::ratio<1>, T>;

  /// Type of charge stored in coulombs, in double precision.
  using coulomb = coulomb_as<>;

  //
  // millicoulomb
  //
  /// Type of charge stored in millicoulomb.
  template <typename T = double>
  using millicoulomb_as = concepts::rescale<coulomb_as<T>, std::milli>;

  /// Type of charge stored in millicoulomb, in double precision.
  using millicoulomb = millicoulomb_as<>;

  //
  // microcoulomb
  //
  /// Type of charge stored in microcoulomb.
  template <typename T = double>
  using microcoulomb_as = concepts::rescale<coulomb_as<T>, std::micro>;

  /// Type of charge stored in microcoulomb, in double precision.
  using microcoulomb = microcoulomb_as<>;

  //
  // nanocoulomb
  //
  /// Type of charge stored in nanocoulomb.
  template <typename T = double>
  using nanocoulomb_as = concepts::rescale<coulomb_as<T>, std::nano>;

  /// Type of charge stored in nanocoulomb, in double precision.
  using nanocoulomb = nanocoulomb_as<>;

  //
  // picocoulomb
  //
  /// Type of charge stored in picocoulomb.
  template <typename T = double>
  using picocoulomb_as = concepts::rescale<coulomb_as<T>, std::pico>;

  /// Type of charge stored in picocoulomb, in double precision.
  using picocoulomb = picocoulomb_as<>;

  //
  // femtocoulomb
  //
  /// Type of charge stored in femtocoulomb.
  template <typename T = double>
  using femtocoulomb_as = concepts::rescale<coulomb_as<T>, std::femto>;

  /// Type of charge stored in femtocoulomb, in double precision.
  using femtocoulomb = femtocoulomb_as<>;

  // -- END Charge -------------------------------------------------------------

  // -- BEGIN Electric potential -----------------------------------------------
  /**
   * @name Electric potential quantities
   *
   * These potential quantities are tied to `util::quantities::units::Volt`.
   * A few options are provided:
   *
   * * most general template, `scaled_volt`, allowing to choose both the
   *     scale of the unit (e.g. `std::kilo` for kilovolt) and the type of
   *     the numerical representation
   * * generic templates (e.g. `volt_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `volt`), ready for use
   *
   */
  /// @{

  /// The most generic `units::Volt`-based quantity.
  template <typename R, typename T = double>
  using scaled_volt = concepts::scaled_quantity<units::Volt, R, T>;

  //
  // volt
  //
  /// Type of potential stored in volt.
  template <typename T = double>
  using volt_as = scaled_volt<std::ratio<1>, T>;

  /// Type of potential stored in volts, in double precision.
  using volt = volt_as<>;

  //
  // millivolt
  //
  /// Type of potential stored in millivolt.
  template <typename T = double>
  using millivolt_as = concepts::rescale<volt_as<T>, std::milli>;

  /// Type of potential stored in millivolt, in double precision.
  using millivolt = millivolt_as<>;

  //
  // microvolt
  //
  /// Type of potential stored in microvolt.
  template <typename T = double>
  using microvolt_as = concepts::rescale<volt_as<T>, std::micro>;

  /// Type of potential stored in microvolt, in double precision.
  using microvolt = microvolt_as<>;

  //
  // kilovolt
  //
  /// Type of potential stored in kilovolt.
  template <typename T = double>
  using kilovolt_as = concepts::rescale<volt_as<T>, std::kilo>;

  /// Type of potential stored in kilovolt, in double precision.
  using kilovolt = kilovolt_as<>;

  //
  // megavolt
  //
  /// Type of potential stored in megavolt.
  template <typename T = double>
  using megavolt_as = concepts::rescale<volt_as<T>, std::mega>;

  /// Type of potential stored in megavolt, in double precision.
  using megavolt = megavolt_as<>;

  //
  // gigavolt
  //
  /// Type of potential stored in gigavolt.
  template <typename T = double>
  using gigavolt_as = concepts::rescale<volt_as<T>, std::giga>;

  /// Type of potential stored in gigavolt, in double precision.
  using gigavolt = gigavolt_as<>;

  /// @}
  // -- END Electric potential -------------------------------------------------

  // -- BEGIN Current ----------------------------------------------------------
  /**
   * @name Current quantities
   *
   * These current quantities are tied to `util::quantities::units::Ampere`.
   * A few options are provided:
   *
   * * most general template, `scaled_ampere`, allowing to choose both the
   *     scale of the unit (e.g. `std::pico` for picoampere) and the type of
   *     the numerical representation
   * * generic templates (e.g. `ampere_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `ampere`), ready for use
   *
   */
  /// @{

  /// The most generic `units::Ampere`-based quantity.
  template <typename R, typename T = double>
  using scaled_ampere = concepts::scaled_quantity<units::Ampere, R, T>;

  //
  // ampere
  //
  /// Type of current stored in ampere.
  template <typename T = double>
  using ampere_as = scaled_ampere<std::ratio<1>, T>;

  /// Type of current stored in amperes, in double precision.
  using ampere = ampere_as<>;

  //
  // milliampere
  //
  /// Type of current stored in milliampere.
  template <typename T = double>
  using milliampere_as = concepts::rescale<ampere_as<T>, std::milli>;

  /// Type of current stored in milliampere, in double precision.
  using milliampere = milliampere_as<>;

  //
  // kiloampere
  //
  /// Type of current stored in kiloampere.
  template <typename T = double>
  using kiloampere_as = concepts::rescale<ampere_as<T>, std::kilo>;

  /// Type of current stored in kiloampere, in double precision.
  using kiloampere = kiloampere_as<>;

  //
  // microampere
  //
  /// Type of current stored in microampere.
  template <typename T = double>
  using microampere_as = concepts::rescale<ampere_as<T>, std::micro>;

  /// Type of current stored in microampere, in double precision.
  using microampere = microampere_as<>;

  //
  // nanoampere
  //
  /// Type of current stored in nanoampere.
  template <typename T = double>
  using nanoampere_as = concepts::rescale<ampere_as<T>, std::nano>;

  /// Type of current stored in nanoampere, in double precision.
  using nanoampere = nanoampere_as<>;

  //
  // picoampere
  //
  /// Type of current stored in picoampere.
  template <typename T = double>
  using picoampere_as = concepts::rescale<ampere_as<T>, std::pico>;

  /// Type of current stored in picoampere, in double precision.
  using picoampere = picoampere_as<>;

  // -- END Current ------------------------------------------------------------

  // -- BEGIN Impedance --------------------------------------------------------
  /**
   * @name Impedance and resistance quantities
   *
   * These impedance quantities are tied to `util::quantities::units::Ohm`.
   * A few options are provided:
   *
   * * most general template, `scaled_ohm`, allowing to choose both the
   *     scale of the unit (e.g. `std::kilo` for kiloohm) and the type of
   *     the numerical representation
   * * generic templates (e.g. `ohm_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `ohm`), ready for use
   *
   */
  /// @{

  /// The most generic `units::Ohm`-based quantity.
  template <typename R, typename T = double>
  using scaled_ohm = concepts::scaled_quantity<units::Ohm, R, T>;

  //
  // ohm
  //
  /// Type of potential stored in ohm.
  template <typename T = double>
  using ohm_as = scaled_ohm<std::ratio<1>, T>;

  /// Type of potential stored in ohms, in double precision.
  using ohm = ohm_as<>;

  //
  // kiloohm
  //
  /// Type of potential stored in kiloohm.
  template <typename T = double>
  using kiloohm_as = concepts::rescale<ohm_as<T>, std::kilo>;

  /// Type of potential stored in kiloohm, in double precision.
  using kiloohm = kiloohm_as<>;

  //
  // megaohm
  //
  /// Type of potential stored in megaohm.
  template <typename T = double>
  using megaohm_as = concepts::rescale<ohm_as<T>, std::mega>;

  /// Type of potential stored in megaohm, in double precision.
  using megaohm = megaohm_as<>;

  //
  // gigaohm
  //
  /// Type of potential stored in gigaohm.
  template <typename T = double>
  using gigaohm_as = concepts::rescale<ohm_as<T>, std::giga>;

  /// Type of potential stored in gigaohm, in double precision.
  using gigaohm = gigaohm_as<>;

  /// @}
  // -- END Impedance ----------------------------------------------------------

  // -- BEGIN Capacitance ------------------------------------------------------
  /**
   * @name Capacitance quantities
   *
   * These capacitance quantities are tied to `util::quantities::units::Farad`.
   * A few options are provided:
   *
   * * most general template, `scaled_farad`, allowing to choose both the
   *     scale of the unit (e.g. `std::pico` for picofarad) and the type of
   *     the numerical representation
   * * generic templates (e.g. `farad_as`), allowing to choose which numerical
   *     representation to use
   * * double precision (e.g. `farad`), ready for use
   *
   */
  /// @{

  /// The most generic `units::Farad`-based quantity.
  template <typename R, typename T = double>
  using scaled_farad = concepts::scaled_quantity<units::Farad, R, T>;

  //
  // farad
  //
  /// Type of charge stored in farad.
  template <typename T = double>
  using farad_as = scaled_farad<std::ratio<1>, T>;

  /// Type of charge stored in farads, in double precision.
  using farad = farad_as<>;

  //
  // millifarad
  //
  /// Type of charge stored in millifarad.
  template <typename T = double>
  using millifarad_as = concepts::rescale<farad_as<T>, std::milli>;

  /// Type of charge stored in millifarad, in double precision.
  using millifarad = millifarad_as<>;

  //
  // microfarad
  //
  /// Type of charge stored in microfarad.
  template <typename T = double>
  using microfarad_as = concepts::rescale<farad_as<T>, std::micro>;

  /// Type of charge stored in microfarad, in double precision.
  using microfarad = microfarad_as<>;

  //
  // nanofarad
  //
  /// Type of charge stored in nanofarad.
  template <typename T = double>
  using nanofarad_as = concepts::rescale<farad_as<T>, std::nano>;

  /// Type of charge stored in nanofarad, in double precision.
  using nanofarad = nanofarad_as<>;

  //
  // picofarad
  //
  /// Type of charge stored in picofarad.
  template <typename T = double>
  using picofarad_as = concepts::rescale<farad_as<T>, std::pico>;

  /// Type of charge stored in picofarad, in double precision.
  using picofarad = picofarad_as<>;

  //
  // femtofarad
  //
  /// Type of charge stored in femtofarad.
  template <typename T = double>
  using femtofarad_as = concepts::rescale<farad_as<T>, std::femto>;

  /// Type of charge stored in femtofarad, in double precision.
  using femtofarad = femtofarad_as<>;

  // -- END Capacitance --------------------------------------------------------

  /**
   * @brief Literal constants for quantities.
   *
   * These functions allow a simplified syntax for specifying a charge or
   * potential quantity.
   * In order to use these, their namespace must be used:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
   * using namespace util::quantities::electromagnetism_literals;
   *
   * // definition of `util::quantities::picocoulomb` constant:
   * constexpr auto Q_pC = 230_pC;
   *
   * // assignment (likely to a quantity) of
   * // `util::quantities::femtocoulomb{500.0}`
   * Q = 500_fC;
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *
   */
  namespace electromagnetism_literals {

    // @{
    /// Literal coulomb value.
    constexpr coulomb operator""_C(long double v)
    {
      return coulomb{static_cast<double>(v)};
    }
    constexpr coulomb operator""_C(unsigned long long int v)
    {
      return coulomb{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal millicoulomb value.
    constexpr millicoulomb operator""_mC(long double v)
    {
      return millicoulomb{static_cast<double>(v)};
    }
    constexpr millicoulomb operator""_mC(unsigned long long int v)
    {
      return millicoulomb{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal microcoulomb value.
    constexpr microcoulomb operator""_uC(long double v)
    {
      return microcoulomb{static_cast<double>(v)};
    }
    constexpr microcoulomb operator""_uC(unsigned long long int v)
    {
      return microcoulomb{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal nanocoulomb value.
    constexpr nanocoulomb operator""_nC(long double v)
    {
      return nanocoulomb{static_cast<double>(v)};
    }
    constexpr nanocoulomb operator""_nC(unsigned long long int v)
    {
      return nanocoulomb{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal picocoulomb value.
    constexpr picocoulomb operator""_pC(long double v)
    {
      return picocoulomb{static_cast<double>(v)};
    }
    constexpr picocoulomb operator""_pC(unsigned long long int v)
    {
      return picocoulomb{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal femtocoulomb value.
    constexpr femtocoulomb operator""_fC(long double v)
    {
      return femtocoulomb{static_cast<double>(v)};
    }
    constexpr femtocoulomb operator""_fC(unsigned long long int v)
    {
      return femtocoulomb{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal volt value.
    constexpr volt operator""_V(long double v)
    {
      return volt{static_cast<double>(v)};
    }
    constexpr volt operator""_V(unsigned long long int v)
    {
      return volt{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal millivolt value.
    constexpr millivolt operator""_mV(long double v)
    {
      return millivolt{static_cast<double>(v)};
    }
    constexpr millivolt operator""_mV(unsigned long long int v)
    {
      return millivolt{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal microvolt value.
    constexpr microvolt operator""_uV(long double v)
    {
      return microvolt{static_cast<double>(v)};
    }
    constexpr microvolt operator""_uV(unsigned long long int v)
    {
      return microvolt{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal kilovolt value.
    constexpr kilovolt operator""_kV(long double v)
    {
      return kilovolt{static_cast<double>(v)};
    }
    constexpr kilovolt operator""_kV(unsigned long long int v)
    {
      return kilovolt{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal megavolt value.
    constexpr megavolt operator""_MV(long double v)
    {
      return megavolt{static_cast<double>(v)};
    }
    constexpr megavolt operator""_MV(unsigned long long int v)
    {
      return megavolt{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal gigavolt value.
    constexpr gigavolt operator""_GV(long double v)
    {
      return gigavolt{static_cast<double>(v)};
    }
    constexpr gigavolt operator""_GV(unsigned long long int v)
    {
      return gigavolt{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal ampere value.
    constexpr ampere operator""_A(long double v)
    {
      return ampere{static_cast<double>(v)};
    }
    constexpr ampere operator""_A(unsigned long long int v)
    {
      return ampere{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal kiloampere value.
    constexpr kiloampere operator""_kA(long double v)
    {
      return kiloampere{static_cast<double>(v)};
    }
    constexpr kiloampere operator""_kA(unsigned long long int v)
    {
      return kiloampere{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal milliampere value.
    constexpr milliampere operator""_mA(long double v)
    {
      return milliampere{static_cast<double>(v)};
    }
    constexpr milliampere operator""_mA(unsigned long long int v)
    {
      return milliampere{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal microampere value.
    constexpr microampere operator""_uA(long double v)
    {
      return microampere{static_cast<double>(v)};
    }
    constexpr microampere operator""_uA(unsigned long long int v)
    {
      return microampere{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal nanoampere value.
    constexpr nanoampere operator""_nA(long double v)
    {
      return nanoampere{static_cast<double>(v)};
    }
    constexpr nanoampere operator""_nA(unsigned long long int v)
    {
      return nanoampere{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal picoampere value.
    constexpr picoampere operator""_pA(long double v)
    {
      return picoampere{static_cast<double>(v)};
    }
    constexpr picoampere operator""_pA(unsigned long long int v)
    {
      return picoampere{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal ohm value.
    constexpr ohm operator""_ohm(long double v)
    {
      return ohm{static_cast<double>(v)};
    }
    constexpr ohm operator""_ohm(unsigned long long int v)
    {
      return ohm{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal kiloohm value.
    constexpr kiloohm operator""_kohm(long double v)
    {
      return kiloohm{static_cast<double>(v)};
    }
    constexpr kiloohm operator""_kohm(unsigned long long int v)
    {
      return kiloohm{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal megaohm value.
    constexpr megaohm operator""_Mohm(long double v)
    {
      return megaohm{static_cast<double>(v)};
    }
    constexpr megaohm operator""_Mohm(unsigned long long int v)
    {
      return megaohm{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal gigaohm value.
    constexpr gigaohm operator""_Gohm(long double v)
    {
      return gigaohm{static_cast<double>(v)};
    }
    constexpr gigaohm operator""_Gohm(unsigned long long int v)
    {
      return gigaohm{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal farad value.
    constexpr farad operator""_F(long double v)
    {
      return farad{static_cast<double>(v)};
    }
    constexpr farad operator""_F(unsigned long long int v)
    {
      return farad{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal millifarad value.
    constexpr millifarad operator""_mF(long double v)
    {
      return millifarad{static_cast<double>(v)};
    }
    constexpr millifarad operator""_mF(unsigned long long int v)
    {
      return millifarad{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal microfarad value.
    constexpr microfarad operator""_uF(long double v)
    {
      return microfarad{static_cast<double>(v)};
    }
    constexpr microfarad operator""_uF(unsigned long long int v)
    {
      return microfarad{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal nanofarad value.
    constexpr nanofarad operator""_nF(long double v)
    {
      return nanofarad{static_cast<double>(v)};
    }
    constexpr nanofarad operator""_nF(unsigned long long int v)
    {
      return nanofarad{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal picofarad value.
    constexpr picofarad operator""_pF(long double v)
    {
      return picofarad{static_cast<double>(v)};
    }
    constexpr picofarad operator""_pF(unsigned long long int v)
    {
      return picofarad{static_cast<double>(v)};
    }
    // @}

    // @{
    /// Literal femtofarad value.
    constexpr femtofarad operator""_fF(long double v)
    {
      return femtofarad{static_cast<double>(v)};
    }
    constexpr femtofarad operator""_fF(unsigned long long int v)
    {
      return femtofarad{static_cast<double>(v)};
    }
    // @}

  } // electromagnetism_literals

} // namespace util::quantities

// --- BEGIN Special relations -------------------------------------------------

// unit relations (they are defined in `util::quantities::concepts::details`,
//   so the `util::quantities::units` namespace can be shortened)
UTIL_QUANTITIES_UNITPRODUCT(units::Farad, units::Volt, units::Coulomb);
UTIL_QUANTITIES_UNITPRODUCT(units::Ohm, units::Ampere, units::Volt);

#ifdef LARDATAALG_UTILITIES_QUANTITIES_SPACETIME_H
UTIL_QUANTITIES_UNITPRODUCT(units::Ohm, units::Farad, units::Second);
#endif

// --- END Special relations ---------------------------------------------------

//------------------------------------------------------------------------------

#endif // LARDATAALG_UTILITIES_QUANTITIES_ELECTROMAGNETISM_H
