#include "Views.h"
#include "Utility.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

using std::copy; using std::ostream_iterator;
using std::cout; using std::endl; using std::ios;
using std::floor;
using std::make_pair;
using std::map;
using std::setw;
using std::string;
using std::vector;

// the size of a local view
const int local_view_size_c = 9;
// the scale of a local view
const double local_view_scale_c = 2.0;
// the maximum size allowed for a map view
const int max_map_view_size_c = 30;
// the minimum size allowed for a map view
const int min_map_view_size_c = 6;
// the initial size of the map view
const int initial_map_view_size_c = 25;
// the initial scale of the map view
const double initial_map_view_scale_c = 2.0;
// the initial coordinates of the origin of the map view
const double initial_map_view_x_coord_c = -10.0;
const double initial_map_view_y_coord_c = -10.0;
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

vector<string> Visual_view::list_insiders(int size, double scale, Point origin) const
{
    vector<string> insiders;
    int ix, iy;
    for(auto it = object_locations.begin(); it != object_locations.end(); ++it) {
        if(get_subscripts(size, scale, origin, ix, iy, it -> second)) {
            insiders.push_back(it -> first);
        }
    }
    return insiders;
}

bool Visual_view::get_subscripts(int size, double scale, Point origin,
                                 int &ix, int &iy, Point location) const
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will return the largest integer smaller than the supplied value
	// even for negative values, so -0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
	} else {
		return true;
    }
}

void List_view::update_remove(const string& name)
{
    object_info.erase(name);
}

void List_view::update_info(const string& name, double value)
{
    auto result = object_info.insert(make_pair(name, value));
    if(!result.second) {
        // If the name already exists, update the value on record.
        (result.first) -> second = value;
    }
}

void List_view::plot(const string& name)
{
    cout << "Current " << name << ":" << endl;
    cout << "--------------" << endl;
    for(auto it = object_info.begin(); it != object_info.end(); ++it) {
        cout << it -> first << ": " << it -> second << endl;
    }
    cout << "--------------" << endl;
}

void List_view::plot_selected(const string& name, const vector<string>& objects) const
{
    cout << name << endl;
    cout << "--------------" << endl;
    for(const string& name : objects) {
        auto it = object_info.find(name);
        if(it != object_info.end()) {
            cout << it -> first << ": " << it -> second << endl;
        }
    }
    cout << "--------------" << endl;
}

Local_view::Local_view(const string& name_, Point location_) : 
    name(name_), size(local_view_size_c), scale(local_view_scale_c)
{
    compute_origin(location_);
}

void Local_view::draw()
{
    cout << "Local view for: " << name << endl;
    Point location;
    if(get_object_location(location, name)) {
        // re-compute the origin if the object is still there
        compute_origin(location);
    }
    plot(size, scale, origin);
}

void Local_view::compute_origin(Point location)
{
    double x = location.x - (size / 2.0) * scale,
           y = location.y - (size / 2.0) * scale;
    origin = Point(x, y);
}

Map_view::Map_view() : 
    size(initial_map_view_size_c), scale(initial_map_view_scale_c),
    origin(Point(initial_map_view_x_coord_c, initial_map_view_y_coord_c))
{}

void Map_view::draw()
{
    cout << "Display size: " << size << ", scale: " << scale
         << ", origin: " << origin << endl;
    list_outliers(size, scale, origin);
    plot(size, scale, origin);
}

void Map_view::set_size(int size_)
{
    if(size_ > max_map_view_size_c) {
        throw Error("New map size is too big!");
    } else if(size_ <= min_map_view_size_c) {
        throw Error("New map size is too small!");
    } else {
        size = size_;
    }
}

void Map_view::set_scale(double scale_)
{
    if(scale_ <= 0) {
        throw Error("New map scale must be positive!");
    } else {
        scale = scale_;
    }
}

void Map_view::set_defaults()
{
    size = initial_map_view_size_c;
    scale = initial_map_view_scale_c;
    origin = Point(initial_map_view_x_coord_c, initial_map_view_y_coord_c);
}

void Health_view::update_health(const string& name, double health)
{
    update_info(name, health);
}

void Health_view::draw()
{
    plot("Health");
}

void Amounts_view::update_amount(const string& name, double amount)
{
    update_info(name, amount);
}

void Amounts_view::draw()
{
    plot("Amounts");
}

Combat_view::Combat_view(const string& name_, Point location_)
    : name(name_), size(local_view_size_c), scale(local_view_scale_c)
{
    compute_origin(location_);
}

void Combat_view::update_health(const string& name, double health)
{
    update_info(name, health);
}

void Combat_view::update_remove(const string& name)
{
    List_view::update_remove(name);
}

void Combat_view::draw()
{
    Point location;
    // If the object exists, re-compute the origin; otherwise, use the origin on file.
    if(get_object_location(location, name)) {
        compute_origin(location);
    }
    vector<string> insiders = list_insiders(size, scale, origin);
    plot_selected("Combat view for: " + name, insiders);
}

void Combat_view::compute_origin(Point location)
{
    double x = location.x - (size / 2.0) * scale,
           y = location.y - (size / 2.0) * scale;
    origin = Point(x, y);
}
