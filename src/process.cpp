#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

#include "linux_parser.h"

// DONE: Return this process's ID
int Process::Pid() { 
    return pid_; 
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const { 
     return cpuUtilization_;
}

// DONE: Return the command that generated this process
string Process::Command() { 
    return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { 
    return std::to_string(ram_); 
    }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return CpuUtilization() < a.CpuUtilization();
}


Process::Process(int pid){
    pid_ = pid;
    command_ = LinuxParser::Command(pid);

    std::string ram = LinuxParser::Ram(pid);
    ram_ = std::stol(ram);

    uptime_ = LinuxParser::UpTime(pid);

    user_ = LinuxParser::User(pid);

    long seconds = LinuxParser::UpTime() - uptime_;
    long totaltime = LinuxParser::ActiveJiffies(pid);
    try{
        cpuUtilization_ = float(totaltime) / float(seconds);
    } catch (...) {
        cpuUtilization_ = 0;
    }
}