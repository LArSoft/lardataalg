#ifndef DETINFO_DETECTORPROPERTIESDATA_H
#define DETINFO_DETECTORPROPERTIESDATA_H

#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"

#include <vector>

namespace detinfo {
  class DetectorProperties;

  class DetectorPropertiesData {
  public:
    explicit DetectorPropertiesData(DetectorProperties const& properties,
                                    double x_ticks_coefficient,
                                    std::vector<std::vector<std::vector<double>>>&& x_ticks_offsets,
                                    std::vector<std::vector<double>>&& drift_direction);

    double Efield(unsigned int planegap = 0) const; ///< kV/cm
    geo::Vector_t NomEfieldDir(geo::TPCID const& tpcid) const; /// Unit vector 

    double DriftVelocity(double efield = 0.,
                         double temperature = 0.) const; ///< cm/us

    /// dQ/dX in electrons/cm, returns dE/dX in MeV/cm.
    double BirksCorrection(double dQdX) const;
    double BirksCorrection(double dQdX, double EField) const;
    double ModBoxCorrection(double dQdX) const;
    double ModBoxCorrection(double dQdX, double EField) const;

    double ElectronLifetime() const;

    /**
     * @brief Returns argon density at a given temperature
     * @param temperature the temperature in kelvin
     * @return argon density in g/cm^3
     *
     * Density is nearly a linear function of temperature.
     * See the NIST tables for details
     * Slope is between -6.2 and -6.1, intercept is 1928 kg/m^3.
     * This parameterization will be good to better than 0.5%.
     */
    double Density(double temperature = 0.) const; ///< g/cm^3

    /// In kelvin.
    double Temperature() const;

    /**
     * @brief Restricted mean energy loss (dE/dx)
     * @param mom  momentum of incident particle [GeV/c]
     * @param mass mass of incident particle [GeV/c^2]
     * @param tcut maximum kinetic energy of delta rays [MeV]; 0 for unlimited
     * @return the restricted mean energy loss (dE/dx) in units of MeV/cm
     *
     * Returned value is always positive.
     * For unrestricted mean energy loss, set tcut = 0 (special case),
     * or tcut large.
     *
     * Based on Bethe-Bloch formula as contained in particle data book.
     * Material parameters are from the configuration.
     */
    double Eloss(double mom, double mass, double tcut) const;

    /**
     * @brief Energy loss fluctuation (@f$ \sigma_{E}^2 / x @f$)
     * @param mom  momentum of incident particle in [GeV/c]
     * @param mass mass of incident particle [GeV/c^2]
     * @return energy loss fluctuation in MeV^2/cm
     *
     * Based on Bichsel formula referred to but not given in PDG.
     */
    double ElossVar(double mom, double mass) const;

    double ElectronsToADC() const;
    unsigned int NumberTimeSamples() const;
    unsigned int ReadOutWindowSize() const;
    double TimeOffsetU() const;
    double TimeOffsetV() const;
    double TimeOffsetZ() const;
    double TimeOffsetY() const;

    double ConvertXToTicks(double X, int p, int t, int c) const;
    double ConvertXToTicks(double X, geo::PlaneID const& planeid) const;

    double ConvertTicksToX(double ticks, int p, int t, int c) const;
    double ConvertTicksToX(double ticks, geo::PlaneID const& planeid) const;

    double GetXTicksOffset(int p, int t, int c) const;
    double GetXTicksOffset(geo::PlaneID const& planeid) const;

    double GetXTicksCoefficient(int t, int c) const;
    double GetXTicksCoefficient(geo::TPCID const& tpcid) const;
    double GetXTicksCoefficient() const;

    bool SimpleBoundary() const;

  private:
    detinfo::DetectorProperties const& fProperties;
    double const fXTicksCoefficient;
    std::vector<std::vector<std::vector<double>>> const fXTicksOffsets;
    std::vector<std::vector<double>> const fDriftDirection;
  }; // class DetectorPropertiesStandard
} // namespace detinfo

#endif // DETINFO_DETECTOR_PROPERTIES_H
