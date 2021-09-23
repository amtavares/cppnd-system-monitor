#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

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

  string line;
  string key;
  float value;
  float memtotal,memfree, memusage;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "MemTotal:") {
          memtotal = value;
          }else if (key == "MemFree:") {
            memfree = value;
            break;
            }
      }
    }
    memusage = (memtotal - memfree)/memtotal;
  }
  return memusage;
  }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 

  string line;
  float uptime_seconds;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> uptime_seconds;
    }
  }
  return uptime_seconds;
  }

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
  }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  
  long totalTime;
  string line, value;
  vector<string> values;
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if( filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      values.push_back(value);
    }
  }

  long utime = 0, stime = 0, cutime=0, cstime = 0;
  if(std::all_of(values[13].begin(), values[13].end(), isdigit ))
    utime = stol(values[13]);

  if(std::all_of(values[14].begin(), values[13].end(), isdigit ))
    stime = stol(values[14]);

  if(std::all_of(values[15].begin(), values[13].end(), isdigit ))
    cutime = stol(values[15]);

  if(std::all_of(values[16].begin(), values[13].end(), isdigit ))
    cstime = stol(values[16]);

  totalTime = utime + stime + cutime + cstime;
  return totalTime/sysconf(_SC_CLK_TCK);

  }
  


// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 

  auto jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) + 
         stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + 
         stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
  
  }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = CpuUtilization();
  long idlejiffies = stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
  return idlejiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  string line, cpu, value;
  vector<string> jiffies;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
      std::getline(filestream, line);
      std::istringstream linestream(line);

      linestream >> cpu;
      while (linestream >> value) {
        jiffies.push_back(value);
    }
  }
  return jiffies;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int processes;
  string key, line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> processes;
        break;
      }
    }
  }
  return processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int number_of_processes;
  string key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> number_of_processes;
        break;
      }
    }
  }
  return number_of_processes;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}  


// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, mem_string;
  long mem;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> mem;
        mem /= 1000;
        mem_string = std::to_string(mem);
        break;
      }
    }
  }
  return mem_string;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, user_id;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> user_id;
        break;
      }
    }
  }
  return user_id;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string id, x, temp, line;
  string name = "DEFAULT";

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> temp >> x >> id;
      if (id == uid) {
        name = temp;
        break;
      }
    }
  }
  return name;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  vector<string> values;
  long starttime = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  try {
    starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  } catch (...) {
    starttime = 0;
  }
  return starttime;
}
