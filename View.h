#ifndef VIEW_H
#define VIEW_H

#include "Geometry.h"
#include <string>

// Public interface for all views.
class View {
public:
    virtual ~View()
    {}

    // update the location of the specified object that the view has on record
    virtual void update_location(const std::string& name, Point location)
    {}

    // update the health of the specified object that the view has on record
    virtual void update_health(const std::string& name, double health)
    {}

    // update the amount of food that the specified object is carrying
    virtual void update_amount(const std::string& name, double amount)
    {}

    // Remove the name and its information (location, amount, health, etc.);
    // no error if the name is not present.
    virtual void update_remove(const std::string& name) = 0;

    // draw the view
    virtual void draw() = 0;
};

#endif