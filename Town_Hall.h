/* 
A Town_Hall is a structure that provides for depositing and withdrawing food,
but does no updating.
*/

#ifndef TOWN_HALL_H
#define TOWN_HALL_H

#include "Structure.h"

class Town_Hall : public Structure {
public:
    Town_Hall(const std::string& in_name, Point in_location);
    
    // deposit adds in the supplied amount
    void deposit(double deposit_amount) override;

    // Return whichever is less, the request or (the amount on hand - 10%) (a "tax"),
    // but amounts less than 1.0 are not supplied - the amount returned is zero.
    // update the amount on hand by subtracting the amount returned.
    double withdraw(double amount_to_obtain) override;

    // output information about the current state
    void describe() const override;

    // in addition to broadcast the states included in Structure,
    // broadcast the current amount of food in the Town Hall.
    void broadcast_current_state() const override;

private:
    double amount;
};

#endif
