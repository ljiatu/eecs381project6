#ifndef UTILITY_H
#define UTILITY_H

#include <string>

/* Utility functions and classes used by other modules */

// a simple class for error exceptions - msg points to a std::string error message
struct Error {
    Error(const std::string& in_msg = "") :
        msg(in_msg)
    {}
    const std::string msg;
};

// The number of letters in the object name
// that determines whether a name is in-use or not.
// Any valid name string must be at least this long.
const size_t num_letters_c = 2;
// the size of a local view
const int local_view_size_c = 9;
// the scale of a local view
const double local_view_scale_c = 2.0;

// the initial attacking strength of a soldier
const int initial_soldier_strength_c = 2;
// the initial attacking range of a soldier
const double initial_soldier_range_c = 2.0;
// the initial attacking strength of an archer
const int initial_archer_strength_c = 1;
// the initial attacking range of an archer
const double initial_archer_range_c = 6.0;

// messages printed when attacking
const char* const soldier_message_c = "Clang!";
const char* const archer_message_c = "Twang!";


#endif
