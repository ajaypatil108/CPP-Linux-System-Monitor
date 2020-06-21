#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid){}; //  Initializer list to iniliatlize pid_  variable of a process class

int Process::Pid() { return pid_; } // Accessor function for pid_ of a process

// Calculation of cpu utilization for each process is done in the LinuxParser function mentioned below
float Process::CpuUtilization() { 
  process_cpu_utilization_ = LinuxParser::ProcessCpuUtilization(Pid());
  return process_cpu_utilization_;
}    

// Pass through to parse Command, Ram usage, User and UpTime of a process 
string Process::Command() { return LinuxParser::Command(Pid()); } 

string Process::Ram() { return LinuxParser::Ram(Pid());}

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }


// Defined operator overloading for both "<" and ">"
bool Process::operator<(Process & a) { 
  return CpuUtilization() < a.CpuUtilization(); 
}

bool Process::operator>(Process & a) { 
  return CpuUtilization() > a.CpuUtilization(); 
}
