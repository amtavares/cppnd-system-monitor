#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 

    int hours, minutes;

    //One hour has 3600 seconds
    hours = seconds / 3600;    
    seconds = seconds % 3600;
    
    //One minute has 60 seconds
    minutes = seconds / 60;
    seconds = seconds % 60;

    std::string s;
    s = std::to_string(hours) + ":" + std::to_string(minutes) + ":" +std::to_string(seconds); 

    return s; 
    }