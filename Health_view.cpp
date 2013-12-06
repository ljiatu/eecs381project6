#include "Health_view.h"

using std::string;

void Health_view::update_health(const string& name, double health)
{
    update_info(name, health);
}

void Health_view::draw()
{
    plot("Health");
}
