#ifndef COMBAT_VIEW_H
#define COMBAT_VIEW_H

#include "List_view.h"
#include "Visual_view.h"

// Combat view shows the health of all agents that are in the local view of the specified object.
class Combat_view : public List_view, private Visual_view {
public:
    Combat_view(const std::string& name_, Point location_);

    void update_health(const std::string& name, double health) override;
    void update_remove(const std::string& name) override;

    // draw the health values of all agents that appear in the local view
    void draw() override;
private:
    std::string name;
    int size;
    double scale;
    Point origin;

    void compute_origin(Point location);
};

#endif
