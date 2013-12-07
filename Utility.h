#ifndef UTILITY_H
#define UTILITY_H

#include <string>

struct Point;

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
// the initial (and also maximum) health of an agent
const int initial_health_c = 5;

// Get the subscripts of an object based on 
// map size, scale, origin and object location.
// Return true if the location is within the grid, false if not.
// This function is used by both Visual_view and Combat_view.
bool get_subscripts(int size, double scale, Point origin,
                    int &ix, int &iy, Point location);
#endif
