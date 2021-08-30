#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hours=0, minutes=0;
  	std::ostringstream result;
    minutes = seconds/60;
    seconds = seconds%60;
    hours = minutes/60;
    minutes = minutes%60;
    result << std::setw(2) << std::setfill('0') << std::to_string(hours)
      << ':' << std::setw(2) << std::setfill('0')<< std::to_string(minutes)
      << ':' << std::setw(2) << std::setfill('0')<< std::to_string(seconds);
  return result.str();
}

