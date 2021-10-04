#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  string key, line;
  float value, memTotal,memFree, memUsage;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "MemTotal:") {
          memTotal = value;
          }else if (key == "MemFree:") {
            memFree = value;
            break;
            }
      }
    }
  }
  memUsage = (memTotal - memFree)/memTotal;
  return memUsage;
  }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 

  string line;
  float systemUptimeSeconds;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> systemUptimeSeconds;
    }
  }
  return systemUptimeSeconds;
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }
  

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0;}


// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
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

  string line, value;
  vector<string> cpuData;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()){
      std::getline(filestream, line);
      
      line.erase(line.begin(), line.begin()+4);

      std::istringstream linestream(line);

      while (linestream >> value) {
        cpuData.push_back(value);
    }
  }
  return cpuData;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int numberProcesses;
  string key, line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> numberProcesses;
        break;
      }
    }
  }
  return numberProcesses;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int numberRunningProcesses;
  string key, line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> numberRunningProcesses;
        break;
      }
    }
  }
  return numberRunningProcesses;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command = " ";

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }

  return command;
}  


// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key;
  string memSring = "0.0";
  long mem;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> mem;
        mem /= 1024;
        memSring = std::to_string(mem);
        break;
      }
    }
  }
  return memSring;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, userId;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> userId;
        break;
      }
    }
  }
  return userId;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {

  string userId = Uid(pid);
  string key, id, x, line;
  string user = "---";

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);

      linestream >> key >> x >> id;
      if (id == userId) {
        user = key;
        break;
      }
    }
  }
  return user;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
 
  float upTimeInSeconds;

  long systemUptime = UpTime();

  vector<string> cpuData = CpuUtilization(pid);

  float startTime = 0.0f;
  if (!cpuData.empty())
    startTime = std::stof(cpuData[21]);


  float hertz = sysconf(_SC_CLK_TCK);

  upTimeInSeconds = (float)systemUptime - (startTime / hertz);

  return upTimeInSeconds;


}

// Overloading this method to be used in the Process class and UpTime(pid)
vector<string> LinuxParser::CpuUtilization(int pid) { 
  
  string line, value;
  vector<string> cpuData;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()){
      std::getline(filestream, line);
      
      std::istringstream linestream(line);

      while (linestream >> value) {
        cpuData.push_back(value);
    }
  }

  return cpuData;
}