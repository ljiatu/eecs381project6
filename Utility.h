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

#endif
