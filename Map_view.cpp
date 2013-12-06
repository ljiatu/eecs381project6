#include "Map_view.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;

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
