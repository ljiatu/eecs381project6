#ifndef AMOUNTS_VIEW_H
#define AMOUNTS_VIEW_H

#include "List_view.h"

// Amounts view shows the amount of food that an object currently holds.
class Amounts_view : public List_view {
public:
    // Save the supplied name and amount for future use in a draw() call
	// If the name is already present, the new amount replaces the previous one.
    void update_amount(const std::string& name, double amount) override;

    // draw the view by showing the health values for each object
    void draw() override;
};

#endif
