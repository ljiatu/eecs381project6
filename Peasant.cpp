#include "Peasant.h"
#include "Model.h"
#include "Structure.h"
#include "Utility.h"
#include <cassert>
#include <iostream>
#include <string>

using std::cout; using std::endl;
using std::shared_ptr;
using std::string;

// the initial amount of food that a peasant carries
const double initial_amount_c = 0.0;
// the maximum amount of food that a peasant can carry
const double max_amount_c = 35.0;

Peasant::Peasant(const string& in_name, Point in_location) :
    Agent(in_name, in_location), amount(initial_amount_c), work_state(NOT_WORKING)
{}

void Peasant::update()
{
    Model& model = Model::get_instance();
    Agent::update();
    if(!is_alive()) {
        return;
    }
    switch(work_state) {
    case NOT_WORKING:
        return;
    case INBOUND:
        if(!is_moving()) {
            work_state = COLLECTING;
        }
        break;
    case COLLECTING:
        {
            // request food from the source and add to the current amount at hand
            double request_amount = max_amount_c - amount;
            double amount_from_source = source -> withdraw(request_amount);
            amount += amount_from_source;
            assert(amount <= max_amount_c);
            model.notify_amount(get_name(), amount);
            if(amount_from_source > 0) {
                cout << get_name() << ": Collected " << amount_from_source << endl;
                work_state = OUTBOUND;
                Agent::move_to(destination -> get_location());
            } else {
                cout << get_name() << ": Waiting " << endl;
            }
        }
        break;
    case OUTBOUND:
        if(!is_moving()) {
            work_state = DEPOSITING;
        }
        break;
    case DEPOSITING:
        destination -> deposit(amount);
        cout << get_name() << ": Deposited " << amount << endl;
        amount = 0;
        model.notify_amount(get_name(), amount);
        Agent::move_to(source -> get_location());
        work_state = INBOUND;
        break;
    default:
        assert(false);
    }
}

void Peasant::move_to(Point dest)
{
    if(work_state != NOT_WORKING) {
        stop_working();
    }
    Agent::move_to(dest);
}

void Peasant::stop()
{
    Agent::stop();
    if(work_state != NOT_WORKING) {
        stop_working();
    }
}

void Peasant::start_working(shared_ptr<Structure> source_, 
                            shared_ptr<Structure> destination_)
{
    assert(source_);
    assert(destination_);
    Agent::stop();
    work_state = NOT_WORKING;
    source.reset();
    destination.reset();
    if(source_ == destination_) {
        throw Error(get_name() + ": I can't move food to and from the same place!");
    }
    source = source_;
    destination = destination_;
    if(amount == 0.0) {
        // collect some food at the source
        Point source_loc = source -> get_location();
        if(get_location() == source_loc) {
            work_state = COLLECTING;
        } else {
            Agent::move_to(source_loc);
            work_state = INBOUND;
        }
    } else {
        // directly goes to the destination to deposit food at hand
        Point dest_loc = destination -> get_location();
        if(get_location() == dest_loc) {
            work_state = DEPOSITING;
        } else {
            Agent::move_to(dest_loc);
            work_state = OUTBOUND;
        }
    }
}

void Peasant::describe() const
{
    cout << "Peasant ";
    Agent::describe();
    cout << "   Carrying " << amount << endl;
    switch(work_state) {
    case NOT_WORKING:
        break;
    case OUTBOUND:
        cout << "   Outbound to destination " << destination -> get_name() << endl;
        break;
    case INBOUND:
        cout << "   Inbound to source " << source -> get_name() << endl;
        break;
    case COLLECTING:
        cout << "   Collecting at source " << source -> get_name() << endl;
        break;
    case DEPOSITING:
        cout << "   Depositing at destination " << destination -> get_name() << endl;
        break;
    default:
        assert(false);
    }
}

void Peasant::broadcast_current_state() const
{
    Agent::broadcast_current_state();
    Model::get_instance().notify_amount(get_name(), amount);
}

void Peasant::stop_working()
{
    cout << get_name() << ": I'm stopping work" << endl;
    work_state = NOT_WORKING;
    source.reset();
    destination.reset();
}
