#include "lardataalg/DetectorInfo/DetectorPropertiesData.h"
#include "lardataalg/DetectorInfo/DetectorProperties.h"

detinfo::DetectorPropertiesData::DetectorPropertiesData(
  DetectorProperties const& properties,
  double const x_ticks_coefficient,
  std::vector<std::vector<std::vector<double>>>&& x_ticks_offsets,
  std::vector<std::vector<double>>&& drift_direction)

  : fProperties{properties}
  , fXTicksCoefficient{x_ticks_coefficient}
  , fXTicksOffsets{move(x_ticks_offsets)}
  , fDriftDirection{move(drift_direction)}
{}

double detinfo::DetectorPropertiesData::Efield(unsigned int const planegap) const
{
  return fProperties.Efield(planegap);
}

geo::Vector_t detinfo::DetectorPropertiesData::NomEfieldDir(geo::TPCID const& tpcid) const
{
  return fProperties.NomEfieldDir(tpcid);
}

double detinfo::DetectorPropertiesData::DriftVelocity(double const efield,
                                                      double const temperature) const
{
  return fProperties.DriftVelocity(efield, temperature);
}

double detinfo::DetectorPropertiesData::BirksCorrection(double const dQdX) const
{
  return fProperties.BirksCorrection(dQdX, Efield());
}
double detinfo::DetectorPropertiesData::BirksCorrection(double const dQdX,
                                                        double const EField) const
{
  return fProperties.BirksCorrection(dQdX, EField);
}

double detinfo::DetectorPropertiesData::ModBoxCorrection(double const dQdX) const
{
  return fProperties.ModBoxCorrection(dQdX, Efield());
}
double detinfo::DetectorPropertiesData::ModBoxCorrection(double const dQdX,
                                                         double const EField) const
{
  return fProperties.ModBoxCorrection(dQdX, EField);
}

double detinfo::DetectorPropertiesData::ElectronsToADC() const
{
  return fProperties.ElectronsToADC();
}

unsigned int detinfo::DetectorPropertiesData::NumberTimeSamples() const
{
  return fProperties.NumberTimeSamples();
}

unsigned int detinfo::DetectorPropertiesData::ReadOutWindowSize() const
{
  return fProperties.ReadOutWindowSize();
}

double detinfo::DetectorPropertiesData::ElectronLifetime() const
{
  return fProperties.ElectronLifetime();
}

double detinfo::DetectorPropertiesData::Density(double const temperature) const
{
  return fProperties.Density(temperature);
}

double detinfo::DetectorPropertiesData::Temperature() const
{
  return fProperties.Temperature();
}

double detinfo::DetectorPropertiesData::Eloss(double const mom,
                                              double const mass,
                                              double const tcut) const
{
  return fProperties.Eloss(mom, mass, tcut);
}

double detinfo::DetectorPropertiesData::ElossVar(double const mom, double const mass) const
{
  return fProperties.ElossVar(mom, mass);
}

double detinfo::DetectorPropertiesData::ConvertXToTicks(double const X,
                                                        int const p,
                                                        int const t,
                                                        int const c) const
{
  return (X / (fXTicksCoefficient * fDriftDirection.at(c).at(t)) +
          fXTicksOffsets.at(c).at(t).at(p));
}

double detinfo::DetectorPropertiesData::ConvertXToTicks(double const X,
                                                        geo::PlaneID const& planeid) const
{
  return ConvertXToTicks(X, planeid.Plane, planeid.TPC, planeid.Cryostat);
}

double detinfo::DetectorPropertiesData::ConvertTicksToX(double const ticks,
                                                        int const p,
                                                        int const t,
                                                        int const c) const
{
  return (ticks - fXTicksOffsets.at(c).at(t).at(p)) * fXTicksCoefficient *
         fDriftDirection.at(c).at(t);
}

double detinfo::DetectorPropertiesData::ConvertTicksToX(double const ticks,
                                                        geo::PlaneID const& planeid) const
{
  return ConvertTicksToX(ticks, planeid.Plane, planeid.TPC, planeid.Cryostat);
}

double detinfo::DetectorPropertiesData::TimeOffsetU() const
{
  return fProperties.TimeOffsetU();
}
double detinfo::DetectorPropertiesData::TimeOffsetV() const
{
  return fProperties.TimeOffsetV();
}
double detinfo::DetectorPropertiesData::TimeOffsetZ() const
{
  return fProperties.TimeOffsetZ();
}

double detinfo::DetectorPropertiesData::GetXTicksOffset(int const p, int const t, int const c) const
{
  return fXTicksOffsets.at(c).at(t).at(p);
}

double detinfo::DetectorPropertiesData::GetXTicksOffset(geo::PlaneID const& planeid) const
{
  return GetXTicksOffset(planeid.Plane, planeid.TPC, planeid.Cryostat);
}

double detinfo::DetectorPropertiesData::GetXTicksCoefficient(int const t, int const c) const
{
  return fXTicksCoefficient * fDriftDirection.at(c).at(t);
}

double detinfo::DetectorPropertiesData::GetXTicksCoefficient() const
{
  return fXTicksCoefficient;
}

bool detinfo::DetectorPropertiesData::SimpleBoundary() const
{
  return fProperties.SimpleBoundary();
}
