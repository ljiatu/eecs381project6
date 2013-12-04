#include "Warriors.h"
#include "Model.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::string;

// the initial attacking strength of a soldier
const int initial_soldier_strength_c = 2;
// the initial attacking range of a soldier
const double initial_soldier_range_c = 2.0;
// the initial attacking strength of an archer
const int initial_archer_strength_c = 1;
// the initial attacking range of an archer
const double initial_archer_range_c = 6.0;

// messages printed when attacking
const char* const soldier_message_c = "Clang!";
const char* const archer_message_c = "Twang!";

Warrior::Warrior(const string& name_, Point location_, int strength_, double range_) :
    Agent(name_, location_), strength(strength_), range(range_),
    attacking(false)
{}

Warrior::~Warrior()
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
    target_ptr -> take_hit(strength, shared_from_this());
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

Soldier::Soldier(const string& name_, Point location_) :
    Warrior(name_, location_, initial_soldier_strength_c, initial_soldier_range_c)
{}

void Soldier::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
    lose_health(attack_strength);
    if(!is_attacking() && is_alive() && attacker_ptr -> is_alive()) {
        start_attacking();
        set_target(attacker_ptr);
        cout << get_name() << ": I'm attacking!" << endl;
    }
}

void Soldier::describe() const
{
    cout << "Soldier ";
    Warrior::describe();
}

void Soldier::print_attack_word() const
{
    cout << get_name()  << ": " << soldier_message_c << endl;
}

Archer::Archer(const string& name_, Point location_) :
    Warrior(name_, location_, initial_archer_strength_c, initial_archer_range_c)
{}

void Archer::update()
{
    Warrior::update();
    if(!is_alive() || is_attacking()) {
        return;
    }
    // we need to look for a new target if not attacking now
    auto closest_agent_ptr = Model::get_instance().get_closest_agent_ptr(shared_from_this(), get_range());
    if(closest_agent_ptr) {
        // if there is a closest agent that is within range
        set_target(closest_agent_ptr);
        start_attacking();
        cout << get_name() << ": I'm attacking!" << endl;
    }
}

void Archer::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
    lose_health(attack_strength);
    if(is_alive() && attacker_ptr -> is_alive()) {
        auto closest_structure_ptr = Model::get_instance().get_closest_structure_ptr(shared_from_this());
        cout << get_name() << ": I'm going to run away to " << closest_structure_ptr -> get_name() << endl;
        move_to(closest_structure_ptr -> get_location());
    }
}

void Archer::describe() const
{
    cout << "Archer ";
    Warrior::describe();
}

void Archer::print_attack_word() const
{
    cout << get_name()  << ": " << archer_message_c << endl;
}
