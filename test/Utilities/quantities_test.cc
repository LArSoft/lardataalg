/**
 * @file   test/Utilities/quantities_test.cc
 * @brief  Unit test for `quantities.h` header
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   October 30, 2018
 * @see    lardataalg/Utilities/quantities.h
 *
 */

// Boost libraries
#define BOOST_TEST_MODULE (quantities_test)
#include <boost/test/unit_test.hpp>

// LArSoft libraries
#include "larcorealg/CoreUtils/StdUtils.h" // util::to_string()
#include "lardataalg/Utilities/quantities.h"

// C/C++ standard libraries
#include <string_view>
#include <type_traits> // std::decay_t<>

using boost::test_tools::tolerance;

// -----------------------------------------------------------------------------
// test units
namespace util::quantities {

  namespace units {

    using namespace std::string_view_literals; // for operator""sv()

    struct TestSecond : public concepts::UnitBase {
      static constexpr auto symbol = "s"sv;
      static constexpr auto name = "second"sv;
    };

    struct TestHertz : public concepts::UnitBase {
      static constexpr auto symbol = "Hz"sv;
      static constexpr auto name = "hertz"sv;
    };

    struct TestOhm : public concepts::UnitBase {
      static constexpr auto symbol = "Ω"sv;
      static constexpr auto name = "ohm"sv;
    };

    struct TestFarad : public concepts::UnitBase {
      static constexpr auto symbol = "F"sv;
      static constexpr auto name = "farad"sv;
    };

  }

  // The most generic `units::TestSecond`-based quantity.
  template <typename R, typename T = double>
  using scaled_second = concepts::scaled_quantity<units::TestSecond, R, T>;

  /// Type of time stored in second.
  template <typename T = double>
  using second_as = scaled_second<std::ratio<1>, T>;

  /// Type of time stored in second, in double precision.
  using second = second_as<>;

  /// Type of time stored in millisecond.
  template <typename T = double>
  using millisecond_as = concepts::rescale<second_as<T>, std::milli>;

  /// Type of time stored in millisecond, in double precision.
  using millisecond = millisecond_as<>;

  /// Type of time stored in microsecond.
  template <typename T = double>
  using microsecond_as = concepts::rescale<second_as<T>, std::micro>;

  /// Type of time stored in microsecond, in double precision.
  using microsecond = microsecond_as<>;

  template <typename T = double>
  using nanosecond_as = concepts::rescale<second_as<T>, std::nano>;

  /// Type of time stored in nanosecond, in double precision.
  using nanosecond = nanosecond_as<>;

  // hertz
  // The most generic `units::TestHertz`-based quantity.
  template <typename R, typename T = double>
  using scaled_hertz = concepts::scaled_quantity<units::TestHertz, R, T>;

  /// Type of frequency stored in hertz.
  template <typename T = double>
  using hertz_as = scaled_hertz<std::ratio<1>, T>;

  /// Type of frequency stored in hertz, in double precision.
  using hertz = hertz_as<>;

  /// Type of frequency stored in kilohertz.
  template <typename T = double>
  using kilohertz_as = concepts::rescale<hertz_as<T>, std::kilo>;

  /// Type of frequency stored in kilohertz, in double precision.
  using kilohertz = kilohertz_as<>;

  // ohm
  // The most generic `units::TestOhm`-based quantity.
  template <typename R, typename T = double>
  using scaled_ohm = concepts::scaled_quantity<units::TestOhm, R, T>;

  /// Type of frequency stored in ohm.
  template <typename T = double>
  using ohm_as = scaled_ohm<std::ratio<1>, T>;

  /// Type of frequency stored in ohm, in double precision.
  using ohm = ohm_as<>;

  /// Type of frequency stored in kiloohm.
  template <typename T = double>
  using kiloohm_as = concepts::rescale<ohm_as<T>, std::kilo>;

  /// Type of frequency stored in kiloohm, in double precision.
  using kiloohm = kiloohm_as<>;

  // farad
  // The most generic `units::TestFarad`-based quantity.
  template <typename R, typename T = double>
  using scaled_farad = concepts::scaled_quantity<units::TestFarad, R, T>;

  /// Type of frequency stored in farad.
  template <typename T = double>
  using farad_as = scaled_farad<std::ratio<1>, T>;

  /// Type of frequency stored in farad, in double precision.
  using farad = farad_as<>;

  /// Type of frequency stored in nanofarad.
  template <typename T = double>
  using nanofarad_as = concepts::rescale<farad_as<T>, std::nano>;

  /// Type of frequency stored in nanofarad, in double precision.
  using nanofarad = nanofarad_as<>;

  namespace unit_literals {

    // Literal second value.
    constexpr second operator""_s(long double v)
    {
      return second{static_cast<double>(v)};
    }
    constexpr second operator""_s(unsigned long long int v)
    {
      return second{static_cast<double>(v)};
    }

    // Literal microsecond value.
    constexpr millisecond operator""_ms(long double v)
    {
      return millisecond{static_cast<double>(v)};
    }
    constexpr millisecond operator""_ms(unsigned long long int v)
    {
      return millisecond{static_cast<double>(v)};
    }

    // Literal microsecond value.
    constexpr microsecond operator""_us(long double v)
    {
      return microsecond{static_cast<double>(v)};
    }
    constexpr microsecond operator""_us(unsigned long long int v)
    {
      return microsecond{static_cast<double>(v)};
    }

    // Literal nanosecond value.
    constexpr nanosecond operator""_ns(long double v)
    {
      return nanosecond{static_cast<double>(v)};
    }
    constexpr nanosecond operator""_ns(unsigned long long int v)
    {
      return nanosecond{static_cast<double>(v)};
    }

    // Literal hertz value.
    constexpr hertz operator""_Hz(long double v)
    {
      return hertz{static_cast<double>(v)};
    }
    constexpr hertz operator""_Hz(unsigned long long int v)
    {
      return hertz{static_cast<double>(v)};
    }

    // Literal kilohertz value.
    constexpr kilohertz operator""_kHz(long double v)
    {
      return kilohertz{static_cast<double>(v)};
    }
    constexpr kilohertz operator""_kHz(unsigned long long int v)
    {
      return kilohertz{static_cast<double>(v)};
    }

    // Literal kiloohm value.
    constexpr kiloohm operator""_kohm(long double v)
    {
      return kiloohm{static_cast<double>(v)};
    }
    constexpr kiloohm operator""_kohm(unsigned long long int v)
    {
      return kiloohm{static_cast<double>(v)};
    }

    // Literal nanofarad value.
    constexpr nanofarad operator""_nF(long double v)
    {
      return nanofarad{static_cast<double>(v)};
    }
    constexpr nanofarad operator""_nF(unsigned long long int v)
    {
      return nanofarad{static_cast<double>(v)};
    }

  } // unit_literals

} // util::quantities::units

UTIL_QUANTITIES_UNITPRODUCT(units::TestHertz, units::TestSecond, units::Unity);
UTIL_QUANTITIES_UNITPRODUCT(units::TestOhm, units::TestFarad, units::TestSecond);

// Because util::quantites::seconds (etc.) has std::numeric_limits<>
// specializations, the Boost unit test suite assumes they are
// suitable for floating-point comparisons, particularly tolerance
// testing (due to the inexact nature of the representation).  The
// following specializations disable such tolerance testing.
namespace boost::math::fpc {
  template <>
  struct tolerance_based<util::quantities::second> : std::false_type {};
  template <>
  struct tolerance_based<util::quantities::millisecond> : std::false_type {};
  template <>
  struct tolerance_based<util::quantities::microsecond> : std::false_type {};
  template <>
  struct tolerance_based<util::quantities::nanosecond> : std::false_type {};
}

// -----------------------------------------------------------------------------
// --- implementation detail tests

template <typename>
struct EmptyClass {};

namespace util::quantities::concepts::details {

  static_assert(!has_unit_v<double>);
  static_assert(!has_unit_v<EmptyClass<int>>);
  static_assert(has_unit_v<ScaledUnit<units::TestSecond>>);
  static_assert(has_unit_v<second>);
  static_assert(has_unit_v<microsecond>);
  static_assert(has_unit_v<microsecond_as<float>>);

  static_assert(!is_quantity_v<double>);
  static_assert(!is_quantity_v<EmptyClass<int>>);
  static_assert(!is_quantity_v<ScaledUnit<units::TestSecond>>);
  static_assert(is_quantity_v<second>);
  static_assert(is_quantity_v<microsecond>);
  static_assert(is_quantity_v<microsecond_as<float>>);

  static_assert(!has_quantity_v<double>);
  static_assert(!has_quantity_v<EmptyClass<int>>);
  static_assert(!has_quantity_v<ScaledUnit<units::TestSecond>>);
  static_assert(has_quantity_v<second>);
  static_assert(has_quantity_v<microsecond>);
  static_assert(has_quantity_v<microsecond_as<float>>);

  static_assert(second::isCompatibleValue<double>());
  static_assert(second::isCompatibleValue<float>());
  static_assert(second::isCompatibleValue<int>());
  static_assert(!second::isCompatibleValue<second>());
  static_assert(!second::isCompatibleValue<microsecond>());
  static_assert(!second::isCompatibleValue<EmptyClass<int>>());

  static_assert(second::hasCompatibleValue<double>());
  static_assert(second::hasCompatibleValue<float>());
  static_assert(second::hasCompatibleValue<int>());
  static_assert(second::hasCompatibleValue<second>());
  static_assert(second::hasCompatibleValue<microsecond>());
  static_assert(!second::hasCompatibleValue<EmptyClass<int>>());

  static_assert(microsecond::sameBaseUnitAs<second>());
  static_assert(!microsecond::sameBaseUnitAs<kilohertz>());

  static_assert(CanCombineQuantities<microsecond, kilohertz, '*'>::value);

  static_assert(std::is_same_v<base_unit_of<microsecond>, units::TestSecond>);
  static_assert(std::is_same_v<base_unit_of<kilohertz>, units::TestHertz>);
  static_assert(std::is_same_v<UnitBinaryOpResult<units::TestSecond, units::TestHertz, '*'>::type,
                               units::Unity>);

  static_assert(std::is_same_v<quantity_binaryop_result_t<microsecond, kilohertz, '*'>, double>);
  static_assert(IsSpecialQuantityOperation<microsecond, kilohertz, '*'>::value);

  static_assert(
    std::is_same_v<QuantitiesComboResultOrVoid<microsecond, kilohertz, '*'>::type, double>);
  static_assert(
    std::is_same_v<quantities_combo_result_or_void_t<microsecond, kilohertz, '*'>, double>);

  static_assert(all_quantities_or_types_units_v<microsecond, kilohertz>);

  static_assert(
    std::is_same_v<special_quantities_combo_result_or_void_t<microsecond, kilohertz, '*'>, double>);

} // namespace util::quantities::concepts::details

// -----------------------------------------------------------------------------
// --- Quantity tests
// -----------------------------------------------------------------------------
static_assert(util::quantities::microsecond::sameBaseUnitAs<util::quantities::second>());
static_assert(
  util::quantities::microsecond::sameBaseUnitAs<util::quantities::microsecond_as<float>>());
static_assert(!util::quantities::microsecond::sameUnitAs<util::quantities::second>());

// -----------------------------------------------------------------------------
void test_quantities_sign()
{

  using namespace util::quantities::unit_literals;

  util::quantities::microsecond t{-4.0};

  BOOST_TEST(t == -4_us); // just to be safe
  static_assert(std::is_same<decltype(+t), util::quantities::microsecond>(),
                "Positive sign converts to a different type!");
  BOOST_TEST(+t == -4_us);
  static_assert(std::is_same<decltype(-t), util::quantities::microsecond>(),
                "Negative sign converts to a different type!");
  BOOST_TEST(-t == 4_us);
  static_assert(std::is_same<decltype(t.abs()), util::quantities::microsecond>(),
                "Negative sign converts to a different type!");
  BOOST_TEST(t.abs() == 4.0_us);

} // test_quantities_sign()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_conversions()
{

  using namespace util::quantities::unit_literals;

  //
  // conversions to other scales
  //
  constexpr util::quantities::second t_s{7.0};

  BOOST_TEST(t_s.value() == 7.0);

  util::quantities::microsecond t_us(t_s);
  t_us = t_s;
  BOOST_TEST(t_us == 7'000'000.0_us);

  util::quantities::second t(t_us);
  BOOST_TEST(t == 7.0_s);

  static_assert(std::is_same<decltype(t.convertInto<util::quantities::microsecond>()),
                             util::quantities::microsecond>());
  BOOST_TEST(t_s.convertInto<util::quantities::microsecond>() == 7'000'000_us);

  static_assert(
    std::is_same_v<decltype(t_s.asValueType<float>()), util::quantities::second_as<float>>);
  static_assert(t_s.asValueType<float>() == t_s);

} // test_quantities_conversions()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_comparisons()
{
  //
  // comparisons between quantities
  //
  util::quantities::microsecond t_us{7.0};
  BOOST_TEST(t_us == t_us);
  BOOST_TEST(!(t_us != t_us));
  BOOST_TEST((t_us >= t_us));
  BOOST_TEST((t_us <= t_us));
  BOOST_TEST(!(t_us > t_us));
  BOOST_TEST(!(t_us < t_us));

  util::quantities::nanosecond t_ns{7.0};
  BOOST_TEST(t_us != t_ns);
  BOOST_TEST(!(t_us == t_ns));
  BOOST_TEST((t_us != t_ns));
  BOOST_TEST((t_us >= t_ns));
  BOOST_TEST(!(t_us <= t_ns));
  BOOST_TEST((t_us > t_ns));
  BOOST_TEST(!(t_us < t_ns));

  util::quantities::nanosecond t2_ns{7000.0};
  BOOST_TEST(t_us == t2_ns);
  BOOST_TEST(!(t_us != t2_ns));
  BOOST_TEST((t_us >= t2_ns));
  BOOST_TEST((t_us <= t2_ns));
  BOOST_TEST(!(t_us > t2_ns));
  BOOST_TEST(!(t_us < t2_ns));

  BOOST_TEST(t_ns != t2_ns);
  BOOST_TEST(!(t_ns == t2_ns));
  BOOST_TEST((t_ns != t2_ns));
  BOOST_TEST(!(t_ns >= t2_ns));
  BOOST_TEST((t_ns <= t2_ns));
  BOOST_TEST(!(t_ns > t2_ns));
  BOOST_TEST((t_ns < t2_ns));

} // test_quantities_conversions()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_multiply_scalar()
{
  //
  // multiplication and division by scalar
  //

  using namespace util::quantities::unit_literals;

  //   5_s * 6_s; // ERROR
  //   5_s * 6_us; // ERROR

  util::quantities::second const t{3.0};
  auto const twice_t = 2.0 * t;
  static_assert(std::is_same<std::decay_t<decltype(twice_t)>, util::quantities::second>(),
                "Multiplication by a scalar converts to a different type!");
  BOOST_TEST(twice_t == 6.0_s);

  auto const t_twice = t * 2.0;
  static_assert(std::is_same<std::decay_t<decltype(t_twice)>, util::quantities::second>(),
                "Multiplication by a scalar converts to a different type!");
  BOOST_TEST(twice_t == 6.0_s);

  static_assert(std::is_same<decltype(twice_t / 2.0), util::quantities::second>(),
                "Division by a scalar converts to a different type!");
  BOOST_TEST(twice_t / 2.0 == 3.0_s);

  static_assert(std::is_same<decltype(twice_t / t), double>(),
                "Division by same quantity is not the base type!");
  BOOST_TEST((twice_t / t) == 2.0); // TODO are parentheses still necessary?

  static_assert(std::is_same<decltype(t / 300_us), double>(),
                "Division by a same-unit quantity is not the base type!");
  BOOST_TEST((t / 300_us) == 10'000.0); // TODO are parentheses still necessary?

} // test_quantities_multiply_scalar()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_addition()
{

  using namespace util::quantities::unit_literals;

  //
  // sum and difference
  //

  //  5_s + 700_ms; // ERROR!
  //  5_s + 0.7; // ERROR!

  static_assert(std::is_same<std::decay_t<decltype(45_s + 5_s)>, util::quantities::second>(),
                "Addition converts to a different type!");
  BOOST_TEST(45_s + 5_s == 50_s);

  static_assert(std::is_same<decltype(5_s - 55_s), util::quantities::second>(),
                "Subtraction converts to a different type!");
  BOOST_TEST(5_s - 55_s == -50_s);

  constexpr util::quantities::second t = 45_s;
  static_assert(std::is_same<std::decay_t<decltype(t.plus(5000_ms))>, util::quantities::second>(),
                "Addition converts to a different type!");
  BOOST_TEST(t.plus(5000_ms) == 50_s);
  BOOST_TEST(t == 45_s);

  static_assert(std::is_same<decltype(t.minus(55000_ms)), util::quantities::second>(),
                "Subtraction converts to a different type!");
  BOOST_TEST(t.minus(55000_ms) == -10_s);
  BOOST_TEST(t == 45_s);

} // test_quantities_addition()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_increment()
{

  using namespace util::quantities::unit_literals;

  //
  // increment and decrement by a quantity
  //
  util::quantities::second t{0.05};

  t += 0.05_s;
  static_assert(std::is_same<decltype(t += 0.05_s), util::quantities::second&>(),
                "Increment converts to a different type!");
  BOOST_TEST(t == 0.1_s);

  t -= 0.05_s;
  static_assert(std::is_same<decltype(t -= 0.05_s), util::quantities::second&>(),
                "Decrement converts to a different type!");
  BOOST_TEST(t == 0.05_s);

  t += 50_ms;
  static_assert(std::is_same<decltype(t += 50_ms), util::quantities::second&>(),
                "Increment converts to a different type!");
  BOOST_TEST(t == 0.1_s);

  t -= 50_ms;
  static_assert(std::is_same<decltype(t -= 50_ms), util::quantities::second&>(),
                "Decrement converts to a different type!");
  BOOST_TEST(t == 0.05_s);

} // test_quantities_multiply_scalar()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_scale()
{

  using namespace util::quantities::unit_literals;

  util::quantities::microsecond t{11.0};
  //
  // scaling
  //
  t *= 2.0;
  static_assert(std::is_same<decltype(t *= 2.0), util::quantities::microsecond&>(),
                "Scaling converts to a different type!");
  BOOST_TEST(t == 22.0_us);

  t /= 2.0;
  static_assert(std::is_same<decltype(t /= 2.0), util::quantities::microsecond&>(),
                "Scaling (division) converts to a different type!");
  BOOST_TEST(t == 11.0_us);

} // test_quantities_scale()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities_literals()
{

  using namespace util::quantities::unit_literals;

  constexpr util::quantities::second t1 = 7_s;
  static_assert(t1.value() == 7.0, "Literal assignment failed.");

  constexpr util::quantities::microsecond t2 = 7_s;
  static_assert(t2.value() == 7000000.0, "Literal conversion failed.");

  util::quantities::microsecond t3;
  t3 = 7.0_s;
  BOOST_TEST(t3.value() == 7000000.0);
  BOOST_TEST(t3 == 7000000_us);

  static_assert(7000000_us == 7_s, "Literal conversion failed.");

} // test_quantities_literals()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_quantities()
{

  using namespace util::quantities::unit_literals;

  // ---------------------------------------------------------------------------
  // default constructor
  //
  //  BOOST_TEST_CHECKPOINT("Default constructor");
  util::quantities::microsecond t1; // can't do much with this except assigning

  // ---------------------------------------------------------------------------
  // assignment
  //
  //  t1 = 4.0; // error!
  t1 = util::quantities::microsecond{4.0};
  BOOST_TEST(util::to_string(t1.unit()) == "us");
  BOOST_TEST(util::to_string(t1) == "4.000000 us");
  BOOST_TEST(t1.value() == 4.0);

  // ---------------------------------------------------------------------------
  // value constructor
  //
  util::quantities::microsecond t2{7.0};
  BOOST_TEST(t2 == 7.0_us);

  // ---------------------------------------------------------------------------

} // test_quantities()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_constexpr_operations()
{

  using namespace util::quantities::unit_literals;

  constexpr util::quantities::microsecond t1{10.0};
  constexpr util::quantities::microsecond t2{20.0};
  constexpr util::quantities::nanosecond t_ns{500.0};
  constexpr util::quantities::nanosecond t1_ns = t1; // convert

  static_assert(t1.value() == 10.0, "value()");
  static_assert(double(t1) == 10.0, "explicit conversion to plain number");
  static_assert(+t1 == 10_us, "unary +");
  static_assert(-t1 == -10_us, "unary -");
  static_assert(t1.abs() == 10_us, "abs()");

  static_assert((t1 == t1), "comparison");
  static_assert(!(t1 == t2), "comparison");
  static_assert(!(t1 == t_ns), "comparison");
  static_assert((t1 == t1_ns), "comparison"); // rounding?

  static_assert(!(t1 != t1), "comparison");
  static_assert((t1 != t2), "comparison");
  static_assert((t1 != t_ns), "comparison");
  static_assert(!(t1 != t1_ns), "comparison"); // rounding?

  static_assert((t1 >= t1), "comparison");
  static_assert(!(t1 >= t2), "comparison");
  static_assert((t1 >= t_ns), "comparison");
  static_assert((t1 >= t1_ns), "comparison"); // rounding?

  static_assert(!(t1 < t1), "comparison");
  static_assert((t1 < t2), "comparison");
  static_assert(!(t1 < t_ns), "comparison");
  static_assert(!(t1 < t1_ns), "comparison"); // rounding?

  static_assert((t1 <= t1), "comparison");
  static_assert((t1 <= t2), "comparison");
  static_assert(!(t1 <= t_ns), "comparison");
  static_assert((t1 <= t1_ns), "comparison"); // rounding?

  static_assert(!(t1 > t1), "comparison");
  static_assert(!(t1 > t2), "comparison");
  static_assert((t1 > t_ns), "comparison");
  static_assert(!(t1 > t1_ns), "comparison"); // rounding?

  static_assert(t1 * 2.0 == 20.0_us, "scaling");
  static_assert(2.0 * t1 == 20.0_us, "scaling");
  static_assert(t1 / 2.0 == 5.0_us, "scaling");

  constexpr util::quantities::kilohertz f1{5.0};
  constexpr double counts = 0.05; // 10 us * 5 kHz

  static_assert(t1 * f1 == counts);
  static_assert(f1 * t1 == counts);
  static_assert(std::is_same_v<decltype(counts / f1), util::quantities::millisecond>);
  static_assert(counts / f1 == t1);
  static_assert(counts / t1 == f1);

  constexpr util::quantities::kiloohm R1{5.0};
  constexpr util::quantities::nanofarad C1{2.0};

  static_assert(R1 * C1 == t1);

  // ---------------------------------------------------------------------------

} // test_constexpr_operations()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void test_makeQuantity()
{

  using namespace util::quantities::unit_literals;
  using util::quantities::millisecond;

  constexpr auto expected = 3.0_ms;
  static_assert(std::is_same<std::decay_t<decltype(expected)>, millisecond>());

  auto q = util::quantities::makeQuantity<millisecond>("3.0 ms");
  static_assert(std::is_same<std::decay_t<decltype(q)>, millisecond>());

  auto const tol = 1e-7 % tolerance();
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("  3.0ms  ");
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("3ms");
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("3000 us");
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("0.03e+2 ms");
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("+3ms");
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("+3E-3s");
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("3", true);
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("3.0", true);
  BOOST_TEST(q.value() == expected.value(), tol);

  q = util::quantities::makeQuantity<millisecond>("30e-1", true);
  BOOST_TEST(q.value() == expected.value(), tol);

  BOOST_CHECK_THROW(util::quantities::makeQuantity<millisecond>("3"),
                    util::quantities::MissingUnit);

  BOOST_CHECK_THROW(util::quantities::makeQuantity<millisecond>("3 kg"),
                    util::quantities::MissingUnit);

  BOOST_CHECK_THROW(util::quantities::makeQuantity<millisecond>("3 dumbs"),
                    util::quantities::ExtraCharactersError);

  BOOST_CHECK_THROW(util::quantities::makeQuantity<millisecond>("three ms"),
                    util::quantities::ValueError);

  BOOST_CHECK_THROW(util::quantities::makeQuantity<millisecond>("3.zero ms"),
                    util::quantities::ExtraCharactersError);

} // test_makeQuantity()

// -----------------------------------------------------------------------------
// BEGIN Test cases  -----------------------------------------------------------
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(quantities_testcase)
{

  test_quantities();
  test_quantities_sign();
  test_quantities_multiply_scalar();
  test_quantities_addition();
  test_quantities_increment();
  test_quantities_scale();
  test_quantities_conversions();
  test_quantities_comparisons();

  test_quantities_literals();

  test_constexpr_operations();

  test_makeQuantity();

} // BOOST_AUTO_TEST_CASE(quantities_testcase)

// -----------------------------------------------------------------------------
// END Test cases  -------------------------------------------------------------
// -----------------------------------------------------------------------------
