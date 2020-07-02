#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// SYSTEM DATA PARSING //

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
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
  stream.close();
  return value;
}


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}


float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  float MemTotal;
  float MemFree;
  float MemUtil;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = value;
        } else if (key == "MemFree:") {
          MemFree = value;
        }
      }
    }
  }
  stream.close();
  MemUtil = ((MemTotal - MemFree) / MemTotal);
  return MemUtil;
}


long LinuxParser::UpTime() {
  long system_uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> system_uptime;
    }
  }
  stream.close();
  return system_uptime;
}


int LinuxParser::TotalProcesses() {
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  stream.close();
  return value;
}


int LinuxParser::RunningProcesses() {
  string key;
  int value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  stream.close();
  return value;
}




// PROCESSES DATA PARSING
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

float LinuxParser::ProcessCpuUtilization(int pid){
string line;
string value;
vector<string> all_values;
long utime, stime, cutime, cstime, starttime;
// CPU time spent in user mode, measured in clock ticks
// CPU time spent in kernel mode, measured in clock ticks
// Waited-for children's CPU time spent in user code (in clock ticks)
// Waited-for children's CPU time spent in kernel code (in clock ticks)
// Time when the process started, measured in clock ticks

std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
if (stream.is_open()){
    while (std::getline(stream, line)){
    std::istringstream linestream(line);
    while (linestream >> value) {
            all_values.push_back(value);
            }
        }
    }
stream.close();
utime = stol(all_values[13]);
stime = stol(all_values[14]);
cutime = stol(all_values[15]);
cstime = stol(all_values[16]);
starttime = stol(all_values[21]);  

long total_time = utime + stime; // Total time spent in the process
total_time = total_time + cutime + cstime;  // Including time from children processes into total time
float Hertz = sysconf(_SC_CLK_TCK);
float seconds = LinuxParser::UpTime() - (starttime/Hertz);
float process_cpu_utilization = (total_time/Hertz)/seconds;
return process_cpu_utilization;
}


string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    return line;
  }
  stream.close();
  return "";
}
  

string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  int value;
  string ram_util;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:"){ 
        	ram_util = to_string(value/1024);
          return ram_util;
        }
      }
    }
  }
  stream.close();
  return string("0");
}


string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  int value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:"){
        	return to_string(value);
        }
      }
    }
  }
  stream.close();
  return string("0");
}
  

string LinuxParser::User(int pid) {
  string line;
  string key1;
  string key2;
  string value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key1 >> key2) {
        if (key1 == "x" && key2 == Uid(pid)){
        	return value;
        }
      }
    }
  }
  stream.close();
  return "0";
}


long LinuxParser::UpTime(int pid){
  string value;
  long int uptime = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
   	for (int i = 0; stream >> value ; i++){
    	if(i == 21){
          uptime = stol(value);
          uptime /= sysconf(_SC_CLK_TCK);   
       }
    }    
  }
  stream.close();
  return uptime;
}