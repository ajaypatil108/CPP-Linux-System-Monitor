#include "processor.h"
#include <string>
#include "linux_parser.h"

using std::string;

// Computation of system's processor utilization. *Not using jiffies & utilization functions as recommended in LinuxParser namespace

float Processor::Utilization() {
  string line{};
  string name{};
  float a[10];
  float idle{0};
  float non_idle{0};
  float total{0};
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
  while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name >>a[0] >>a[1] >>a[2] >>a[3] >>a[4] >>a[5] >>a[6] >>a[7] >>a[8]>>a[9];
      if (name == "cpu"){
        idle = a[LinuxParser::kIdle_] + a[LinuxParser::kIOwait_];
  non_idle = a[LinuxParser::kUser_] + a[LinuxParser::kNice_] + a[LinuxParser::kSystem_] + a[LinuxParser::kIRQ_] + a[LinuxParser::kSoftIRQ_] + a[LinuxParser::kSteal_];
  total = idle + non_idle;
  cpu_utilization_ = (total - idle)/total;    
      }     
        }
  }
  filestream.close();   
  return cpu_utilization_; 
}