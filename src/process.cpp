#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include "process.h"

using std::string;

#include "linux_parser.h"

// DONE: Return this process's ID
int Process::Pid() { 
    return processId_; 
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    /*
    - The calculation method in this function is described in
    https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    - Removed the "100*" in the cpuUsage variable calculation because this multiplication is already done in ncurses_display.cpp
    */

    float totalTime, seconds, cpuUsage;
    float utime, stime, cutime, cstime, starttime;
    float hertz = sysconf(_SC_CLK_TCK);
    
    long systemUptime = LinuxParser::UpTime();

    std::vector<string>cpuData =  LinuxParser::CpuUtilization(processId_);

    if (!cpuData.empty()){
        utime = std::stof(cpuData[13]);
        stime = std::stof(cpuData[14]);
        cutime = std::stof(cpuData[15]);
        cstime = std::stof(cpuData[16]);
        starttime = std::stof(cpuData[21]);
    } else{
        utime = 0.0;
        stime = 0.0;
        cutime = 0.0;
        cstime = 0.0;
        starttime = 0.0;
    }

    totalTime = utime + stime + cutime + cstime;
    seconds = (float)systemUptime - (starttime / hertz);
    cpuUsage = (totalTime / hertz) / seconds;

    return cpuUsage;

}

// DONE: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(processId_);
     
    }

// DONE: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(processId_);
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(processId_); 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return  LinuxParser::UpTime(processId_); 
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& otherProcess) const { 
  return CpuUtilization() < otherProcess.CpuUtilization();
}


Process::Process(int processId){
    processId_ = processId;
}