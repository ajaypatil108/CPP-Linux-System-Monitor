#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string formatted_time; 
  long hrs = seconds/3600;
  seconds = seconds%3600;
  long mins = seconds/60;
  seconds = seconds%60;
  long secs = seconds; 
  formatted_time = to_string(hrs) + ":" + to_string(mins) + ":" + to_string(secs); 
  return formatted_time; 
}