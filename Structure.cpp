#include "Structure.h"
#include "Model.h"
#include <iostream>

using std::cout; using std::endl;
using std::string;

Structure::Structure(const string& name_, Point location_) :
    Sim_object(name_), location(location_)
{}

Structure::~Structure()
{}

void Structure::describe() const
{
    cout << get_name() << " at " << location << endl;
}

void Structure::broadcast_current_state() const
{
    Model::get_instance().notify_location(get_name(), location);
}

double Structure::withdraw(double amount_to_get)
{
    return 0.0;
}

