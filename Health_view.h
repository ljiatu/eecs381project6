#ifndef HEALTH_VIEW_H
#define HEALTH_VIEW_H

#include "List_view.h"

// Health view shows the health value for all objects with a health attribute.
class Health_view : public List_view {
public:
    // Save the supplied name and health value for future use in a draw() call
	// If the name is already present, the new health replaces the previous one.
    void update_health(const std::string& name, double health) override;

    // draw the view by showing the health values for each object
    void draw() override;
};

#endif