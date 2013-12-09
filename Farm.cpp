#include "Farm.h"
#include "Model.h"
#include <cassert>
#include <iostream>

using std::cout; using std::endl;
using std::string;

// the initial amount of food that a farm contains
const double initial_amount_c = 50.0;
// the amount of food that a farm produces on each update
const double production_rate_c = 2.0;

Farm::Farm (const string& in_name, Point in_location) :
    Structure(in_name, in_location), amount(initial_amount_c), rate(production_rate_c)
{}

double Farm::withdraw(double amount_to_get)
{
    assert(amount_to_get >= 0);
    // calculate the amount of food that can be withdrawn,
    // and deduct from the amount of food at hand
    double amount_taken = (amount_to_get < amount)? amount_to_get : amount;
    amount -= amount_taken;
    Model::get_instance().notify_amount(get_name(), amount);
    assert(amount >= 0);
    return amount_taken;
}

void Farm::update()
{
    amount += rate;
    Model::get_instance().notify_amount(get_name(), amount);
    cout << "Farm " << get_name() << " now has " << amount << endl;
}

void Farm::describe() const
{
    cout << "Farm ";
    Structure::describe();
    cout << "   Food available: " << amount << endl;
}

void Farm::broadcast_current_state() const
{
    Structure::broadcast_current_state();
    Model::get_instance().notify_amount(get_name(), amount);
}
