/**
 * @file   test/Utilities/electromagnetism_test.cc
 * @brief  Unit test for `lardataalg/Utilities/quantities/electromagnetism.h` header.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   January 13, 2020
 * @see    `lardataalg/Utilities/quantities.h`
 *
 * This test covers only the space units of `electromagnetism.h`.
 * It's not overwhelmingly complete, either.
 */

// Boost libraries
#define BOOST_TEST_MODULE (electromagnetism_test)
#include <boost/test/unit_test.hpp>

// LArSoft libraries
#include "lardataalg/Utilities/quantities/electromagnetism.h"
#include "lardataalg/Utilities/quantities/spacetime.h"

// C/C++ standard libraries
#include <iostream>
#include <type_traits> // std::decay_t<>

// -----------------------------------------------------------------------------
template <typename T, typename R>
constexpr bool equalityTest(T v, R ref)
{
  // if the reference value is 0, `v` must be below tolerance;
  // otherwise, `v` relative difference from reference must be below tolerance
  constexpr double tol = 1e-7;

  auto abs = [](auto v) { return v < decltype(v){0} ? -v : v; };

  if (ref == static_cast<R>(0))
    return abs(v) <= static_cast<T>(tol);
  else
    return abs(v / ref - 1.0) <= tol;
}

// -----------------------------------------------------------------------------
void test_charge_literals()
{

  using namespace util::quantities::electromagnetism_literals;

  // the charge out of a single photoelectron of an ICARUS PMT (gain ~4x10^6)
  constexpr auto Q_pC = 0.640870612_pC;
  static_assert(std::is_same<decltype(Q_pC), util::quantities::picocoulomb const>());
  BOOST_TEST(Q_pC.value() == 0.640870612);
  static_assert(equalityTest(Q_pC.value(), 0.640870612));
  std::cout << "Tested " << Q_pC << std::endl;

  constexpr auto Q_C = 6.40870612e-13_C;
  static_assert(std::is_same<decltype(Q_C), util::quantities::coulomb const>());
  BOOST_TEST(Q_C.value() == 6.40870612e-13);
  static_assert(equalityTest(Q_C.value(), 6.40870612e-13));
  static_assert(equalityTest(Q_C, Q_pC));
  std::cout << "Tested " << Q_C << std::endl;

  constexpr auto Q_mC = 6.40870612e-10_mC;
  static_assert(std::is_same<decltype(Q_mC), util::quantities::millicoulomb const>());
  BOOST_TEST(Q_mC.value() == 6.40870612e-10);
  static_assert(equalityTest(Q_mC.value(), 6.40870612e-10));
  static_assert(equalityTest(Q_mC, Q_pC));
  std::cout << "Tested " << Q_mC << std::endl;

  constexpr auto Q_uC = 6.40870612e-7_uC;
  static_assert(std::is_same<decltype(Q_uC), util::quantities::microcoulomb const>());
  BOOST_TEST(Q_uC.value() == 6.40870612e-7);
  static_assert(equalityTest(Q_uC.value(), 6.40870612e-7));
  static_assert(equalityTest(Q_uC, Q_pC));
  std::cout << "Tested " << Q_uC << std::endl;

  constexpr auto Q_nC = 0.000640870612_nC;
  static_assert(std::is_same<decltype(Q_nC), util::quantities::nanocoulomb const>());
  BOOST_TEST(Q_nC.value() == 0.000640870612);
  static_assert(equalityTest(Q_nC.value(), 0.000640870612));
  static_assert(equalityTest(Q_nC, Q_pC));
  std::cout << "Tested " << Q_nC << std::endl;

  constexpr auto Q_fC = 640.870612_fC;
  static_assert(std::is_same<decltype(Q_fC), util::quantities::femtocoulomb const>());
  BOOST_TEST(Q_fC.value() == 640.870612);
  static_assert(equalityTest(Q_fC.value(), 640.870612));
  static_assert(equalityTest(Q_fC, Q_pC));
  std::cout << "Tested " << Q_fC << std::endl;

} // test_charge_literals()

// -----------------------------------------------------------------------------
void test_potential_literals()
{

  using namespace util::quantities::electromagnetism_literals;

  // bias voltage of the collection plane of ICARUS detector
  // (apparently too low so that it affects wire plane "transparency")
  constexpr auto V_V = 250_V;
  static_assert(std::is_same<decltype(V_V), util::quantities::volt const>());
  BOOST_TEST(V_V.value() == 250);
  static_assert(equalityTest(V_V.value(), 250));
  std::cout << "Tested " << V_V << std::endl;

  constexpr auto V_mV = 250'000_mV;
  static_assert(std::is_same<decltype(V_mV), util::quantities::millivolt const>());
  BOOST_TEST(V_mV.value() == 250'000);
  static_assert(equalityTest(V_mV.value(), 250'000));
  static_assert(equalityTest(V_mV, V_V));
  std::cout << "Tested " << V_mV << std::endl;

  constexpr auto V_uV = 250'000'000.0_uV;
  static_assert(std::is_same<decltype(V_uV), util::quantities::microvolt const>());
  BOOST_TEST(V_uV.value() == 250'000'000);
  static_assert(equalityTest(V_uV.value(), 250'000'000));
  static_assert(equalityTest(V_uV, V_V));
  std::cout << "Tested " << V_uV << std::endl;

  constexpr auto V_kV = 0.25_kV;
  static_assert(std::is_same<decltype(V_kV), util::quantities::kilovolt const>());
  BOOST_TEST(V_kV.value() == 0.25);
  static_assert(equalityTest(V_kV.value(), 0.25));
  static_assert(equalityTest(V_kV, V_V));
  std::cout << "Tested " << V_kV << std::endl;

  constexpr auto V_MV = 2.5e-4_MV;
  static_assert(std::is_same<decltype(V_MV), util::quantities::megavolt const>());
  BOOST_TEST(V_MV.value() == 2.5e-4);
  static_assert(equalityTest(V_MV.value(), 2.5e-4));
  static_assert(equalityTest(V_MV, V_V));
  std::cout << "Tested " << V_MV << std::endl;

  constexpr auto V_GV = 2.5e-7_GV;
  static_assert(std::is_same<decltype(V_GV), util::quantities::gigavolt const>());
  BOOST_TEST(V_GV.value() == 2.5e-7);
  static_assert(equalityTest(V_GV.value(), 2.5e-7));
  static_assert(equalityTest(V_GV, V_V));
  std::cout << "Tested " << V_GV << std::endl;

} // test_potential_literals()

// -----------------------------------------------------------------------------
void test_current_literals()
{

  using namespace util::quantities::electromagnetism_literals;

  // current by a single drifting electron in MicroBooNE
  constexpr auto i_pA = -5.681e-5_pA;
  static_assert(std::is_same<decltype(i_pA), util::quantities::picoampere const>());
  BOOST_TEST(i_pA.value() == -5.681e-5);
  static_assert(equalityTest(i_pA.value(), -5.681e-5));
  std::cout << "Tested " << i_pA << std::endl;

  constexpr auto i_nA = -5.681e-8_nA;
  static_assert(std::is_same<decltype(i_nA), util::quantities::nanoampere const>());
  BOOST_TEST(i_nA.value() == -5.681e-8);
  static_assert(equalityTest(i_nA.value(), -5.681e-8));
  static_assert(equalityTest(i_nA, i_pA));
  std::cout << "Tested " << i_nA << std::endl;

  constexpr auto i_uA = -5.681e-11_uA;
  static_assert(std::is_same<decltype(i_uA), util::quantities::microampere const>());
  BOOST_TEST(i_uA.value() == -5.681e-11);
  static_assert(equalityTest(i_uA.value(), -5.681e-11));
  static_assert(equalityTest(i_uA, i_pA));
  std::cout << "Tested " << i_uA << std::endl;

  constexpr auto i_mA = -5.681e-14_mA;
  static_assert(std::is_same<decltype(i_mA), util::quantities::milliampere const>());
  BOOST_TEST(i_mA.value() == -5.681e-14);
  static_assert(equalityTest(i_mA.value(), -5.681e-14));
  static_assert(equalityTest(i_mA, i_pA));
  std::cout << "Tested " << i_mA << std::endl;

  constexpr auto i_A = -5.681e-17_A;
  static_assert(std::is_same<decltype(i_A), util::quantities::ampere const>());
  BOOST_TEST(i_A.value() == -5.681e-17);
  static_assert(equalityTest(i_A.value(), -5.681e-17));
  static_assert(equalityTest(i_A, i_pA));
  std::cout << "Tested " << i_A << std::endl;

} // test_current_literals()

// -----------------------------------------------------------------------------
void test_resistance_literals()
{

  using namespace util::quantities::electromagnetism_literals;

  // series resistance in ICARUS PMT dynode multiplication circuit
  // (M. Babicz et al 2018 JINST 13 P10030)
  constexpr auto R_ohm = 3.76e6_ohm;
  static_assert(std::is_same<decltype(R_ohm), util::quantities::ohm const>());
  BOOST_TEST(R_ohm.value() == 3.76e6);
  static_assert(equalityTest(R_ohm.value(), 3.76e6));
  std::cout << "Tested " << R_ohm << std::endl;

  constexpr auto R_kohm = 3.76e3_kohm;
  static_assert(std::is_same<decltype(R_kohm), util::quantities::kiloohm const>());
  BOOST_TEST(R_kohm.value() == 3.76e3);
  static_assert(equalityTest(R_kohm.value(), 3.76e3));
  static_assert(equalityTest(R_kohm, R_ohm));
  std::cout << "Tested " << R_kohm << std::endl;

  constexpr auto R_Mohm = 3.76_Mohm;
  static_assert(std::is_same<decltype(R_Mohm), util::quantities::megaohm const>());
  BOOST_TEST(R_Mohm.value() == 3.76);
  static_assert(equalityTest(R_Mohm.value(), 3.76));
  static_assert(equalityTest(R_Mohm, R_ohm));
  std::cout << "Tested " << R_Mohm << std::endl;

  constexpr auto R_Gohm = 3.76e-3_Gohm;
  static_assert(std::is_same<decltype(R_Gohm), util::quantities::gigaohm const>());
  BOOST_TEST(R_Gohm.value() == 3.76e-3);
  static_assert(equalityTest(R_Gohm.value(), 3.76e-3));
  static_assert(equalityTest(R_Gohm, R_ohm));
  std::cout << "Tested " << R_Gohm << std::endl;

} // test_resistance_literals()

// -----------------------------------------------------------------------------
void test_capacity_literals()
{

  using namespace util::quantities::electromagnetism_literals;

  // capacitance of a ICARUS TPC (relative permittivity from Shene, William R.,
  // "Measurement of the Dielectric Constants of Liquid Argon and Oxygen" (1966).
  // Theses and Dissertations. 3964.)
  constexpr auto C_pF = 542.5872864_pF;
  static_assert(std::is_same<decltype(C_pF), util::quantities::picofarad const>());
  BOOST_TEST(C_pF.value() == 542.5872864);
  static_assert(equalityTest(C_pF.value(), 542.5872864));
  std::cout << "Tested " << C_pF << std::endl;

  constexpr auto C_fF = 542587.2864_fF;
  static_assert(std::is_same<decltype(C_fF), util::quantities::femtofarad const>());
  BOOST_TEST(C_fF.value() == 542587.2864);
  static_assert(equalityTest(C_fF.value(), 542587.2864));
  static_assert(equalityTest(C_fF, C_pF));
  std::cout << "Tested " << C_fF << std::endl;

  constexpr auto C_nF = 0.5425872864_nF;
  static_assert(std::is_same<decltype(C_nF), util::quantities::nanofarad const>());
  BOOST_TEST(C_nF.value() == 0.5425872864);
  static_assert(equalityTest(C_nF.value(), 0.5425872864));
  static_assert(equalityTest(C_nF, C_pF));
  std::cout << "Tested " << C_nF << std::endl;

  constexpr auto C_uF = 542.5872864e-6_uF;
  static_assert(std::is_same<decltype(C_uF), util::quantities::microfarad const>());
  BOOST_TEST(C_uF.value() == 542.5872864e-6);
  static_assert(equalityTest(C_uF.value(), 542.5872864e-6));
  static_assert(equalityTest(C_uF, C_pF));
  std::cout << "Tested " << C_uF << std::endl;

  constexpr auto C_mF = 542.5872864e-9_mF;
  static_assert(std::is_same<decltype(C_mF), util::quantities::millifarad const>());
  BOOST_TEST(C_mF.value() == 542.5872864e-9);
  static_assert(equalityTest(C_mF.value(), 542.5872864e-9));
  static_assert(equalityTest(C_mF, C_pF));
  std::cout << "Tested " << C_mF << std::endl;

  constexpr auto C_F = 542.5872864e-12_F;
  static_assert(std::is_same<decltype(C_F), util::quantities::farad const>());
  BOOST_TEST(C_F.value() == 542.5872864e-12);
  static_assert(equalityTest(C_F.value(), 542.5872864e-12));
  static_assert(equalityTest(C_F, C_pF));
  std::cout << "Tested " << C_F << std::endl;

} // test_capacity_literals()

// -----------------------------------------------------------------------------
void test_product_RiV()
{

  using namespace util::quantities::electromagnetism_literals;

  constexpr util::quantities::millivolt V = 10.0_V;
  constexpr util::quantities::kiloohm R = 5.0_kohm;
  constexpr util::quantities::microampere i = 2.0_mA;

  constexpr auto const V1 = R * i;
  static_assert(std::is_same_v<decltype(V1), util::quantities::millivolt const>);
  static_assert(equalityTest(V1, V));

  constexpr auto const V2 = i * R;
  static_assert(std::is_same_v<decltype(V2), util::quantities::millivolt const>);
  static_assert(equalityTest(V2, V));

  constexpr auto const i1 = V / R;
  static_assert(std::is_same_v<decltype(i1), util::quantities::microampere const>);
  static_assert(equalityTest(i1, i));

  constexpr auto const R2 = V / i;
  static_assert(std::is_same_v<decltype(R2), util::quantities::kiloohm const>);
  static_assert(equalityTest(R2, R));

} // test_product_RiV()

// -----------------------------------------------------------------------------
void test_product_CVQ()
{

  using namespace util::quantities::electromagnetism_literals;

  constexpr util::quantities::nanocoulomb Q = 100.0_nC;
  constexpr util::quantities::picofarad C = 50.0_pF;
  constexpr util::quantities::kilovolt V = 2.0_kV;

  constexpr auto const Q1 = C * V;
  static_assert(std::is_same_v<decltype(Q1), util::quantities::nanocoulomb const>);
  static_assert(equalityTest(Q1, Q));

  constexpr auto const Q2 = V * C;
  static_assert(std::is_same_v<decltype(Q2), util::quantities::nanocoulomb const>);
  static_assert(equalityTest(Q2, Q));

  constexpr auto const C1 = Q / V;
  static_assert(std::is_same_v<decltype(C1), util::quantities::picofarad const>);
  static_assert(equalityTest(C1, C));

  constexpr auto const V2 = Q / C;
  static_assert(std::is_same_v<decltype(V2), util::quantities::kilovolt const>);
  static_assert(equalityTest(V2, V));

} // test_product_CVQ()

// -----------------------------------------------------------------------------
void test_product_RCt()
{

  using namespace util::quantities::electromagnetism_literals;
  using namespace util::quantities::time_literals;

  constexpr util::quantities::nanosecond t = 10.0_ns;
  constexpr util::quantities::kiloohm R = 5.0_kohm;
  constexpr util::quantities::picofarad C = 2.0_pF;

  constexpr auto const t1 = R * C;
  static_assert(std::is_same_v<decltype(t1), util::quantities::nanosecond const>);
  static_assert(equalityTest(t1, t));

  constexpr auto const t2 = C * R;
  static_assert(std::is_same_v<decltype(t2), util::quantities::nanosecond const>);
  static_assert(equalityTest(t2, t));

  constexpr auto const R1 = t / C;
  static_assert(std::is_same_v<decltype(R1), util::quantities::kiloohm const>);
  static_assert(equalityTest(R1, R));

  constexpr auto const C2 = t / R;
  static_assert(std::is_same_v<decltype(C2), util::quantities::picofarad const>);
  static_assert(equalityTest(C2, C));

} // test_product_RCt()

// -----------------------------------------------------------------------------
// BEGIN Test cases  -----------------------------------------------------------
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(literal_testcase)
{

  test_charge_literals();
  test_potential_literals();
  test_current_literals();
  test_resistance_literals();
  test_capacity_literals();

} // BOOST_AUTO_TEST_CASE(literal_testcase)

// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(products_testcase)
{

  test_product_RiV();
  test_product_CVQ();
  test_product_RCt();

} // BOOST_AUTO_TEST_CASE(literal_testcase)

// -----------------------------------------------------------------------------
// END Test cases  -------------------------------------------------------------
// -----------------------------------------------------------------------------
