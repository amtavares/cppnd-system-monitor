#include "processor.h"

#include "linux_parser.h"
#include "string"

using std::string;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  /*
   The calculation method implemented in this function is mainly based on the answers from user Vangelis Tasoulas here:
   https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  
  */

  /*

   Values in cpuData vector:
    0 user
    1 nice
    2 system
    3 idle
    4 iowait
    5 irq
    6 softirq
    7 steal
    8 guest
    9 guest_nice
  */

  float utilization, deltaTotal, deltaIdle;
  float currentIdle, currentIoWait, currentUser, currentNice, currentSystem,
      currentIrq, currentSoftIrq, currentSteal;
  float currentTotal, currentTotalIdle, currentTotalNonIdle;
  float prevTotalNonIdle, prevTotal, prevTotalIdle;

  std::vector<string> cpuData = LinuxParser::CpuUtilization();

  currentUser = std::stof(cpuData[0]);
  currentNice = std::stof(cpuData[1]);
  currentSystem = std::stof(cpuData[2]);
  currentIdle = std::stof(cpuData[3]);
  currentIoWait = std::stof(cpuData[4]);
  currentIrq = std::stof(cpuData[5]);
  currentSoftIrq = std::stof(cpuData[6]);
  currentSteal = std::stof(cpuData[7]);

  prevTotalIdle = prevIdle_ + prevIoWait_;
  currentTotalIdle = currentIdle + currentIoWait;

  prevTotalNonIdle = prevUser_ + prevNice_ + prevSystem_ + prevIrq_ +
                     prevSoftIrq_ + prevSteal_;
  currentTotalNonIdle = currentUser + currentNice + currentSystem + currentIrq +
                        currentSoftIrq + currentSteal;

  prevTotal = prevTotalIdle + prevTotalNonIdle;
  currentTotal = currentTotalIdle + currentTotalNonIdle;

  // diferentiate
  deltaTotal = currentTotal - prevTotal;
  deltaIdle = currentTotalIdle - prevTotalIdle;

  utilization = (deltaTotal - deltaIdle) / deltaTotal;

  // Saving values
  prevIdle_ = currentIdle;
  prevIoWait_ = currentIoWait;
  prevUser_ = currentUser;
  prevNice_ = currentNice;
  prevSystem_ = currentSystem;
  prevIrq_ = currentIrq;
  prevSoftIrq_ = currentSoftIrq;
  prevSteal_ = currentSteal;

  return utilization;
}
