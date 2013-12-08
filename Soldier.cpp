#include "Soldier.h"
#include "Archer.h"
#include "Utility.h"
#include <iostream>
#include <cmath>

using std::cout; using std::endl;
using std::shared_ptr; using std::string;
using std::rand; using std::static_pointer_cast;

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

// Override Soldier's under attack behavior from Soldier.
// Soldier will draw his shield to defend himself. But he doesn's succeed in doing so
// everytime. If he succeeds, he will not lose_health in this round.
void Soldier::take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr)
{
	// Solider's defend function, if he "happens to be able to defend himself"
	// Then he will not lose health this time under attack.
	cout << "Called\n";
	int defend = rand()%10;
	if (defend<=5) {
		cout << get_name() << ": I failed to shield myself!" <<endl;
		lose_health(attack_strength);
	}
	else {
		cout << get_name() << ": Tang!" <<endl;
		cout << get_name() << ": I shielded myself!" << endl;
	}
    if(!is_attacking() && is_alive() && attacker_ptr -> is_alive()) {
        initiate_attacking();
        set_target(attacker_ptr);
        cout << get_name() << ": I'm attacking!" << endl;
    }
}

// Override Soldier's under attack behavior from Archer.
// Soldier's not smart enough to choose best action, therefore he choose to blindly attack!
void Soldier::take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr)
{
    lose_health(attack_strength);
    if(!is_attacking() && is_alive() && attacker_ptr -> is_alive()) {
        initiate_attacking();
        set_target(attacker_ptr);
        cout << get_name() << ": I'm attacking!" << endl;
    }
}

// Override Soldier's under attack behavior from Witch_doctor.
// Should never be called because Witch_doctor always run towards farthest structure
// while under attack from Soldier.
void Soldier::take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr)
{
	cout << get_name() << ": I'm poisoned!" << endl; 
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

void Soldier::attack() 
{
	get_target()->take_hit(get_strength(), static_pointer_cast<Soldier>(shared_from_this()));
}