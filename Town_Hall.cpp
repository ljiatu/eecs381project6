#include "Town_Hall.h"
#include "Model.h"
#include <cassert>
#include <iostream>

using std::cout; using std::endl;
using std::string;

// the initial amount of food a Toen_Hall has
const double initial_amount_c = 0.0;
// the rate of the tax that Town_Hall collects when food is being withdrawn
const double tax_rate_c = 0.1;

Town_Hall::Town_Hall(const string& name_, Point location_) :
    Structure(name_, location_), amount(initial_amount_c)
{}

void Town_Hall::deposit(double deposit_amount)
{
    assert(deposit_amount >= 0);
    amount += deposit_amount;
    Model::get_instance().notify_amount(get_name(), amount);
}

double Town_Hall::withdraw(double amount_to_obtain)
{
    assert(amount_to_obtain >= 0);
    double amount_available = amount * (1 - tax_rate_c);
    if(amount_available < 1.0) {
        // town hall does not provide tiny amount of food
        amount_available = 0;
        return 0;
    } else {
        // calculate the amount of food to be taken away
        // and notify the model about the change
        double amount_taken = (amount_to_obtain < amount_available)? amount_to_obtain : amount_available;
        amount -= amount_taken;
        Model::get_instance().notify_amount(get_name(), amount);
        assert(amount >= 0);
        return amount_taken;
    }
}

void Town_Hall::describe() const
{
    cout << "Town_Hall ";
    Structure::describe();
    cout << "   Contains " << amount << endl;
}

void Town_Hall::broadcast_current_state() const
{
    Structure::broadcast_current_state();
    Model::get_instance().notify_amount(get_name(), amount);
}
