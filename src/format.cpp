#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int sec;
  int min;
  int hours;
  string hh,mm,ss;
  hours = seconds/3600;
  min = (seconds % 3600) / 60;
  sec = ((seconds % 3600) % 60);

   hh = hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours);
   mm = min < 10 ? "0" + std::to_string(min) : std::to_string(min);
   ss = sec < 10 ? "0" + std::to_string(sec) : std::to_string(sec);
  
  
  return hh + ":" + mm + ":" + ss; }