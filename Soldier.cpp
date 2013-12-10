#include "Soldier.h"
#include "Archer.h"
#include "Witch_doctor.h"
#include "Utility.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using std::cout; using std::endl;
using std::rand;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;

// soldier defend threshold, if rand returns higher than this value,
// then he is able to defend himself.
const int soldier_defend_threshold_c = 1;
// soldier defend random generation range value.
const int soldier_defend_rand_range_c = 2;
// the initial attacking strength of a soldier
const int initial_soldier_strength_c = 2;
// the initial attacking range of a soldier
const double initial_soldier_range_c = 2.0;
// messages printed when attacking
const char* const soldier_message_c = "Clang!";

Soldier::Soldier(const string& name_, Point location_) :
    Warrior(name_, location_, initial_soldier_strength_c, initial_soldier_range_c)
{}

void Soldier::take_hit(int attack_strength, shared_ptr<Soldier> soldier_ptr)
{
	// Solider's defend function, if he "happens to be able to defend himself"
	// Then he will not lose health this time under attack.
	int defend = rand() % soldier_defend_rand_range_c;
	if(defend < soldier_defend_threshold_c) {
		lose_health(attack_strength);
		cout << get_name() << ": I failed to shield myself!" <<endl;
	} else {
		cout << get_name() << ": Tang! I shielded myself!" << endl;
	}
    if(can_counter_attack(soldier_ptr)) {
        initiate_attacking("I'm attacking!", soldier_ptr);
    }
}

void Soldier::take_hit(int attack_strength, shared_ptr<Archer> archer_ptr)
{
    lose_health(attack_strength);
    if(can_counter_attack(archer_ptr)) {
        initiate_attacking("Watch out, Archer!", archer_ptr);
    }
}

void Soldier::take_hit(int attack_strength, shared_ptr<Witch_doctor> doctor_ptr)
{
    lose_health(attack_strength);
    if(can_counter_attack(doctor_ptr)) {
        move_to(doctor_ptr -> get_location());
        initiate_attacking("Watch me coming, Doctor!", doctor_ptr);
    }
}

void Soldier::describe() const
{
    cout << "Soldier ";
    Warrior::describe();
}

void Soldier::dispatch_hit() 
{
    auto target_ptr = get_target();
    assert(target_ptr);
    cout << get_name()  << ": " << soldier_message_c << endl;
	target_ptr -> take_hit(get_strength(), static_pointer_cast<Soldier>(shared_from_this()));
}
