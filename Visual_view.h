#ifndef VISUAL_VIEW_H
#define VISUAL_VIEW_H

#include "View.h"
#include <map>
#include <string>
#include <vector>

// Base class for all views that print out a map.
class Visual_view : public View {
public:
    virtual ~Visual_view()
    {}

	// Save the supplied name and location for future use in a draw() call
	// If the name is already present, the new location replaces the previous one.
	void update_location(const std::string& name, Point location) override;

	// Remove the name and its location; no error if the name is not present.
	void update_remove(const std::string& name) override; 

protected:
    // draw the map given the origin
    void plot(int size, double scale, Point origin) const;

    // Query about if the object exists.
    // Return true and thei pass the location of the object back if exists;
    // Return false and does not modify the passed-in Point otherwise.
    bool get_object_location(Point& point, const std::string& name) const;

    // print all objects that are outside of the map
    // given the current size, scale and origin of the map
    void list_outliers(int size, double scale, Point origin) const;

private:
    // locations for all objects that are currently in the view
    std::map<std::string, Point> object_locations;
};

#endif
