/* *** View classes ***
The set of view classes encapsulate various data and functions needed to generate the map
display (either in the format of a grid or a list), and control its properties. 
It has a "memory" for the names and various values (location, health, amounts etc.)
of the to-be-plotted objects.
*/

#ifndef VIEWS_H
#define VIEWS_H

#include "Geometry.h"
#include <map>
#include <string>
#include <vector>

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

// Base class for all views that print out a map.
class Visual_view : public virtual View {
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

    // return the name of all objects that will be shown inside the map
    std::vector<std::string> list_insiders(int size, double scale, Point origin) const;

private:
    // locations for all objects that are currently in the view
    std::map<std::string, Point> object_locations;

    // Get the subscripts of an object based on 
    // map size, scale, origin and object location.
    // Return true if the location is within the grid, false if not.
    bool get_subscripts(int size, double scale, Point origin,
                        int &ix, int &iy, Point location) const;
};

// List view shows information in a list format.
class List_view : public virtual View {
public:
    virtual ~List_view()
    {}

    // Remove the name and its value, no error if the name is not present.
    void update_remove(const std::string& name) override;

protected:
    // Save the supplied name and value for future use in a draw() call
	// If the name is already present, the new health replaces the previous one.
    void update_info(const std::string& name, double value);

    // plot the view by first printing the name of the view,
    // then listing values for each object.
    void plot(const std::string& name);

    // plot the view by first printing the name of the view,
    // then listing values for each object that is in both the passed-in
    // container and the object_info container.
    void plot_selected(const std::string& name, const std::vector<std::string>& objects) const;

private:
    // store the information (e.g., health, amounts, etc.)
    // for each object with that attribute
    std::map<std::string, double> object_info;
};

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

class Map_view : public Visual_view {
public:
	// default constructor sets the default size, scale, and origin
	Map_view(); 
	
	// prints out the current map
	void draw() override;
	
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	void set_size(int size_);
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	void set_scale(double scale_);

    // Set the origin as specified by the user.
    void set_origin(Point origin_)
    {origin = origin_;}
	
	// set the parameters to the default values
	void set_defaults();

private:
    int size;
    double scale;
    Point origin;
};

// Health view shows the health value for all objects with a health attribute.
class Health_view : public List_view {
public:
    // Save the supplied name and health value for future use in a draw() call
	// If the name is already present, the new health replaces the previous one.
    void update_health(const std::string& name, double health) override;

    // draw the view by showing the health values for each object
    void draw() override;
};

// Amounts view shows the amount of food that an object currently holds.
class Amounts_view : public List_view {
public:
    // Save the supplied name and amount for future use in a draw() call
	// If the name is already present, the new amount replaces the previous one.
    void update_amount(const std::string& name, double amount) override;

    // draw the view by showing the health values for each object
    void draw() override;
};

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
