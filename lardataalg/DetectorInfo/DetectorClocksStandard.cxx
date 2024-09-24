#include "lardataalg/DetectorInfo/DetectorClocksStandard.h"
#include "fhiclcpp/ParameterSet.h"

#include "range/v3/view/zip.hpp"

#include <iostream>

//-------------------------------------------------------------------------
detinfo::DetectorClocksStandard::DetectorClocksStandard(fhicl::ParameterSet const& pset)
  : fConfigName{"G4RefTime",
                "TriggerOffsetTPC",
                "FramePeriod",
                "ClockSpeedTPC",
                "ClockSpeedOptical",
                "ClockSpeedTrigger",
                "ClockSpeedExternal",
                "DefaultTrigTime",
                "DefaultBeamTime"}
  , fConfigValue{pset.get<double>(fConfigName[kG4RefTime]),
                 pset.get<double>(fConfigName[kTriggerOffsetTPC]),
                 pset.get<double>(fConfigName[kFramePeriod]),
                 pset.get<double>(fConfigName[kClockSpeedTPC]),
                 pset.get<double>(fConfigName[kClockSpeedOptical]),
                 pset.get<double>(fConfigName[kClockSpeedTrigger]),
                 pset.get<double>(fConfigName[kClockSpeedExternal]),
                 pset.get<double>(fConfigName[kDefaultTrigTime]),
                 pset.get<double>(fConfigName[kDefaultBeamTime])}
  , fTrigModuleName{pset.get<std::string>("TrigModuleName")}
  , fG4RefCorrTrigModuleName{pset.get<std::string>("G4RefCorrTrigModuleName", "baddefault")}
  , fTriggerOffsetTPC{fConfigValue[kTriggerOffsetTPC]}
  , fTriggerTime{fConfigValue[detinfo::kDefaultTrigTime]}
  , fBeamGateTime{fConfigValue[detinfo::kDefaultBeamTime]}
  , fFramePeriod{fConfigValue[kFramePeriod]}
  , fTPCClock{fTriggerTime, fFramePeriod, fConfigValue[kClockSpeedTPC]}
{
  SetTriggerTime(fConfigValue[detinfo::kDefaultTrigTime], fConfigValue[detinfo::kDefaultBeamTime]);
}

void detinfo::DetectorClocksStandard::ApplyParams()
{
  fFramePeriod = fConfigValue[kFramePeriod];
  fTriggerOffsetTPC = fConfigValue[kTriggerOffsetTPC];
  SetTriggerTime(fConfigValue[detinfo::kDefaultTrigTime], fConfigValue[detinfo::kDefaultBeamTime]);
}

bool detinfo::DetectorClocksStandard::IsRightConfig(const fhicl::ParameterSet& ps) const
{
  if (ps.has_key("module_label")) { return false; }
  return std::all_of(fConfigName.cbegin(), fConfigName.cend(), [&ps](auto const& config_name) {
    return ps.has_key(config_name);
  });
}

void detinfo::DetectorClocksStandard::debugReport() const
{
  std::cout << "fConfigValues contents: " << std::endl;

  for (auto const& [name, value] : ranges::views::zip(fConfigName, fConfigValue))
    std::cout << "\n    " << name << " ... " << value;
  std::cout << std::endl;

  DataForJob().debugReport(std::cout);
  std::cout.flush();

} // detinfo::DetectorClocksStandard::debugReport()
