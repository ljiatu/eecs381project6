#include "Soldier.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr; using std::string;

// the initial attacking strength of a soldier
const int initial_soldier_strength_c = 2;
// the initial attacking range of a soldier
const double initial_soldier_range_c = 2.0;
// messages printed when attacking
const char* const soldier_message_c = "Clang!";

Soldier::Soldier(const string& name_, Point location_) :
    Warrior(name_, location_, initial_soldier_strength_c, initial_soldier_range_c)
{}

void Soldier::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
    lose_health(attack_strength);
    if(!is_attacking() && is_alive() && attacker_ptr -> is_alive()) {
        initiate_attacking();
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