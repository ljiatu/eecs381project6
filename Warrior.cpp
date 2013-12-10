#include "Warrior.h"
#include "Model.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::string;

Warrior::Warrior(const string& name_, Point location_, int strength_, double range_) :
    Agent(name_, location_), strength(strength_), range(range_),
    attacking(false)
{}

void Warrior::update()
{
    Agent::update();
    if(!is_alive() || !attacking) {
        return;
    }
    auto target_ptr = target.lock();
    if(!target_ptr || !target_ptr -> is_alive()) {
        // If the target disappears or the target is still there but not alive,
        // stop attacking and forget about the target.
        cout << get_name() << ": Target is dead" << endl;
        attacking = false;
        target.reset();
        return;
    }

    double distance = cartesian_distance(get_location(), target_ptr -> get_location());
    if(distance > range) {
        cout << get_name()  << ": Target is now out of range" << endl;
        attacking = false;
        return;
    }

    // print proper attacking word and impose hit on the target
	dispatch_hit();
    if(!target_ptr -> is_alive()) {
        cout << get_name() << ": I triumph!" << endl;
        attacking = false;
        target.reset();
    }
}

void Warrior::start_attacking(shared_ptr<Agent> target_ptr)
{
    if(target_ptr == shared_from_this()) {
        throw Error(get_name() + ": I cannot attack myself!");
    } else if(!target_ptr -> is_alive()) {
        throw Error(get_name()+ ": Target is not alive!");
    }

    double distance = cartesian_distance(get_location(), target_ptr -> get_location());
    if(distance > range) {
        throw Error(get_name() + ": Target is out of range!");
    }
    initiate_attacking("I'm attacking!", target_ptr);
}

void Warrior::stop()
{
    cout << get_name() << ": Don't bother me" << endl;
}

void Warrior::describe() const
{
    Agent::describe();
    if(!attacking) {
        cout << "   Not attacking" << endl;
        return;
    }
    auto target_ptr = target.lock();
    if(target_ptr) {
        cout << "   Attacking " << target_ptr -> get_name() << endl;
    } else {
        cout << "   Attacking dead target" << endl;
    }
}

void Warrior::initiate_attacking(const string& message, shared_ptr<Agent> target_ptr)
{
    cout << get_name() << ": " << message << endl;
    target = target_ptr;
    attacking = true;
}

bool Warrior::can_counter_attack(shared_ptr<Agent> attacker) const
{
    return !attacking && is_alive() && attacker -> is_alive();
}

bool Warrior::can_run_away(shared_ptr<Agent> attacker) const
{
    return is_alive() && attacker -> is_alive();
}
