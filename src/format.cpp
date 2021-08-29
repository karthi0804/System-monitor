#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hours=0, minutes=0;
    minutes = seconds/60;
    seconds = seconds%60;
    hours = minutes/60;
    minutes = minutes%60;
    return std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(seconds);
}