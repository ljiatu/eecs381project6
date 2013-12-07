#include "Combat_view.h"
#include "Utility.h"

using std::string;
using std::vector;

Combat_view::Combat_view(const string& name_, Point location_)
    : name(name_), size(local_view_size_c), scale(local_view_scale_c)
{
    compute_origin(location_);
}

void Combat_view::update_health(const string& name, double health)
{
    update_info(name, health);
}

void Combat_view::update_location(const string& name, Point location)
{
    auto result = object_locations.insert(make_pair(name, location));
    if(!result.second) {
        // If the object with this name is already there, update the location.
        (result.first) -> second = location;
    }
}

void Combat_view::draw()
{
    // If the object is not removed, 
    // re-compute the origin in case that the object moved.
    auto it = object_locations.find(name);
    if(it != object_locations.end()) {
        compute_origin(it -> second);
    }
    vector<string> insiders = list_insiders();
    plot_selected("Combat view for: " + name, insiders);
}

void Combat_view::compute_origin(Point location)
{
    double x = location.x - (size / 2.0) * scale,
           y = location.y - (size / 2.0) * scale;
    origin = Point(x, y);
}

vector<string> Combat_view::list_insiders() const
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
