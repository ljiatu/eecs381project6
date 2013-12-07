#include "Visual_view.h"
#include "Geometry.h"
#include "Utility.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <utility>

using std::copy; using std::ostream_iterator;
using std::cout; using std::endl;
using std::ios;
using std::make_pair;
using std::map;
using std::setw;
using std::string;
using std::vector;

// we display the coordinates every three rows and columns
const int show_coord_num_c = 3;
// the width of the axis lable
const int axis_label_width_c = 4;

void Visual_view::update_location(const string& name, Point location)
{
    auto result = object_locations.insert(make_pair(name, location));
    if(!result.second) {
        // If the object with this name is already there, update the location.
        (result.first) -> second = location;
    }
}

void Visual_view::update_remove(const string& name)
{
    object_locations.erase(name);
}

void Visual_view::plot(int size, double scale, Point origin) const
{
    vector<vector<string>> map(size);
    for(vector<string>& vec : map) {
        // resize each vector and fill each cell out with the default display
        vec.assign(size, ". ");
    }

    // collect subscripts for all objects
    for(auto it = object_locations.begin(); it != object_locations.end(); ++it) {
        int ix, iy;
        if(get_subscripts(size, scale, origin, ix, iy, it -> second)) {
            if(map[iy][ix] == ". ") {
                // if there is no other objects occupying the same spot yet
                // go ahead and fill up the spot
                map[iy][ix] = (it -> first).substr(0, num_letters_c);
            } else {
                // otherwise, fill up with an * to indicate
                // there are multiple objects at the current spot
                map[iy][ix] = "* ";
            }
        }
    }

    // A simple class that facilitates saving and restoring output settings
    class Cout_format_saver {
    public:
        Cout_format_saver() :
            old_flags(cout.flags()), old_precision(cout.precision())
        {
            // we don't want any digits to the right of the decimal point
            // when printing axis labels
            cout.precision(0);
        }
        ~Cout_format_saver()
        {
            cout.flags(old_flags);
            cout.precision(old_precision);
        }
    private:
        ios::fmtflags old_flags;
        int old_precision;
    };
    
    // From this point on, until the function ends, the only numbers
    // we're printing are axis labels. So change to new format here.
    Cout_format_saver saver;
    // Print out the map and coordinates.
    // Since large y subscripts appear first, print in reverse order.
    ostream_iterator<string> out_it(cout);
    for(int i = size - 1; i >= 0; --i) {
        if(i % show_coord_num_c) {
            cout << "     ";
        } else {
            // print out y-coordinates every three rows
            cout << setw(axis_label_width_c) << i * scale + origin.y << " ";
        }
        copy(map[i].begin(), map[i].end(), out_it);
        cout << endl;
    }
    // print out x-coordinates every three columns
    for(int i = 0; i < size; i += show_coord_num_c) {
        cout << "  " << setw(axis_label_width_c) << i * scale + origin.x;
    }
    cout << endl;
}

bool Visual_view::get_object_location(Point& point, const string& name) const
{
    auto it = object_locations.find(name);
    if(it != object_locations.end()) {
        point = it -> second;
        return true; 
    } else {
        return false;
    }
}

void Visual_view::list_outliers(int size, double scale, Point origin) const
{
    bool first = true; // is this the first outside object?
    int ix, iy;
    for(auto it = object_locations.begin(); it != object_locations.end(); ++it) {
        if(!get_subscripts(size, scale, origin, ix, iy, it -> second)) {
            if(first) {
                cout << it -> first;
                first = false;
            } else {
                cout << ", " << it -> first;
            }
        }
    }

    if(!first) {
        // If we have printed at least one outside object,
        // print the following statement as well.
        cout << " outside the map" << endl;
    }
}
