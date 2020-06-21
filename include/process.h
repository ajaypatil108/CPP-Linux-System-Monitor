#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid); // Declaring a constructor that takes value of pid as an integer
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();     
  
  // Operator overloading for comparison process based on cpu utilization
  bool operator<(Process & a);  
  bool operator>(Process & a);
  
 private:
  int pid_; // PID of instance of a process class
  float process_cpu_utilization_;	// cpu utilization of instance of a process class
};

#endif