#ifndef LOCAL_VIEW_H
#define LOCAL_VIEW_H

#include "Visual_view.h"

// Local view tracks the location of a specified object. 
// It's always centered at the location of the object.
// If the object disappears, it is centered at the last location of the object.
class Local_view : public Visual_view {
public:
    // construct the local view given the name and location of the object
    Local_view(const std::string& name_, Point location_);

    // Draw the map. The map is centered at the location of the object.
    void draw() override;

private:
    // the name of the object that the view is tracking
    std::string name;
    int size;
    double scale;
    Point origin;

    // compute the origin of the local view based on the location of the object
    void compute_origin(Point location);
};

#endif
