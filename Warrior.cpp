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

    print_attack_word();
	attack();
    //target_ptr -> take_hit(strength, shared_from_this());
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
    cout << get_name() << ": I'm attacking!" << endl;
    target = target_ptr;
    attacking = true;
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
