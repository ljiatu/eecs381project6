#define NDEBUG

#include "Agent.h"
#include "Model.h"
#include "Utility.h"
#include <cassert>
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr;
using std::string;

// the initial health of an agent
const int initial_health_c = 5;
// the initial speed of an agent
const double initial_speed_c = 5.0;

Agent::Agent(const string& in_name, Point in_location) :
    Sim_object(in_name), Moving_object(in_location, initial_speed_c),
    health(initial_health_c), state(ALIVE)
{}

Agent::~Agent()
{}

Point Agent::get_location() const
{
    return get_current_location();
}

bool Agent::is_moving() const
{
    return is_currently_moving();
}

void Agent::move_to(Point destination_)
{
    start_moving(destination_);
    if(is_currently_moving()) {
        cout << get_name() << ": I'm on the way" << endl;
    } else {
        cout << get_name() << ": I'm already there" << endl;
    }
}

void Agent::stop()
{
    if(is_currently_moving()) {
        stop_moving();
        cout << get_name() << ": I'm stopped" << endl;
    }
}

void Agent::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
    lose_health(attack_strength);
}

void Agent::update()
{
    switch(state) {
    case ALIVE:
        if(is_currently_moving()) {
            cout << get_name()
                 << (update_location() ? ": I'm there!" : ": step...")
                 << endl;
            Model::get_instance().notify_location(get_name(), get_location());
        }
        break;
    case DEAD:
        break;
    default:
        // we should never reach here
        assert(false);
    }
}

void Agent::describe() const
{
    cout << get_name() << " at " << get_location() << endl;
    switch(state) {
    case ALIVE:
        cout << "   Health is " << health  << endl;
        if(is_currently_moving()) {
            cout << "   Moving at speed " << get_current_speed()  
                 << " to " << get_current_destination() << endl;
        } else {
            cout << "   Stopped" << endl;
        }
        break;
    case DEAD:
        // not expected to be visible in this project
        cout << "   Is dead" << endl;
        break;
    default:
        assert(false);
    }
}

void Agent::broadcast_current_state() const
{
    Model& model = Model::get_instance();
    model.notify_location(get_name(), get_location());
    model.notify_health(get_name(), health);
}

void Agent::start_working(shared_ptr<Structure> source_ptr, 
                          shared_ptr<Structure> destination_ptr)
{
    throw Error(get_name() + ": Sorry, I can't work!");
}

void Agent::start_attacking(shared_ptr<Agent> target_ptr)
{
    throw Error(get_name() + ": Sorry, I can't attack!");
}

void Agent::start_healing (std::shared_ptr<Agent> target_ptr) 
{
    throw Error(get_name() + ": Sorry, I can't save!");
}

void Agent::lose_health(int attack_strength)
{
    Model& model = Model::get_instance();
    health -= attack_strength;
    if(health <= 0) {
        state = DEAD;
        stop_moving();
        model.notify_gone(get_name());
        cout << get_name() << ": Arrggh!" << endl;
        model.remove_agent(shared_from_this());
    } else {
        model.notify_health(get_name(), health);
        cout << get_name() << ": Ouch!" << endl;
    }
}

void Agent::gain_health(int curing_strength) {
    Model& model = Model::get_instance();
    if (health == initial_health_c)
        cout << get_name() << ": I'm perfectly fine!" << endl;
    else {
        health += curing_strength;
        if (health > initial_health_c)
            health = initial_health_c;
        cout << get_name() << ": I'm feeling better!" << endl;
    }
    model.notify_health(get_name(), health);
}
