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

void Combat_view::update_remove(const string& name)
{
    List_view::update_remove(name);
}

void Combat_view::draw()
{
    vector<string> insiders = list_insiders(size, scale, origin);
    plot_selected("Combat view for: " + name, insiders);
}

void Combat_view::compute_origin(Point location)
{
    double x = location.x - (size / 2.0) * scale,
           y = location.y - (size / 2.0) * scale;
    origin = Point(x, y);
}
