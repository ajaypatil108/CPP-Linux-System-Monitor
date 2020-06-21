#ifndef PROCESSOR_H
#define PROCESSOR_H

// Basic class for processor representation

class Processor {
 public:
  float Utilization();  
 private:
  float cpu_utilization_ ; // private variables to store cpu utilization 
};

#endif