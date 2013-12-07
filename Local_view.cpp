#include "Local_view.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;
using std::string;

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
