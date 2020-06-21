#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() {return cpu_;} // Accessor function for instance of a processor within instance of a system


// Sorting and storing processes (instances)
vector<Process>& System::Processes() { 
  this->processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  
  for (int pid: pids){
    	Process temp_process(pid);
    	processes_.emplace_back(temp_process);
  	}
   std::sort(processes_.begin(), processes_.end(),[](Process& p1, Process& p2){return (p1>p2);});
  return processes_;
 }

// Pass-through functions to LinuxParser namespace for System's : Kernel, Memory Utilization, Operating System, Uptime, Running and Total processes.

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }