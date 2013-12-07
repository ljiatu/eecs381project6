#ifndef COMBAT_VIEW_H
#define COMBAT_VIEW_H

#include "List_view.h"

// Combat view shows the health of all agents that are in the local view of the specified object.
class Combat_view : public List_view {
public:
    Combat_view(const std::string& name_, Point location_);

    void update_health(const std::string& name, double health) override;
    void update_location(const std::string& name, Point location) override;

    // draw the health values of all agents that appear in the local view
    void draw() override;
private:
    std::string name;
    int size;
    double scale;
    Point origin;
    // this stores the locations for all objects
    std::map<std::string, Point> object_locations;

    // compute the origin based on the agent 
    void compute_origin(Point location);
    
    // return the name of all objects that will be shown inside the map
    std::vector<std::string> list_insiders() const;
};

#endif
