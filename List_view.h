#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include "View.h"
#include <map>
#include <vector>

// List view shows information in a list format.
class List_view : public View {
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

#endif
