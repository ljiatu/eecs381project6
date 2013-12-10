#include "Witch_doctor.h"
#include "Archer.h"
#include "Model.h"
#include "Soldier.h"
#include "Structure.h"
#include "Utility.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::static_pointer_cast;
using std::string;

// Doctor remedy threshold. If rand returns higher than this value, 
// then he finds a remedy
const int doctor_remedy_threshold_c = 1;
// doctor remedy random generation range value
const int doctor_remedy_rand_range_c = 4;
// the initial curing strength of a witch doctor
const int initial_attack_strength_c = 1;
// the intial attackign strength of a witch doctor
const int initial_curing_strength_c = 1;
// the initial attacking range of a witch doctor
const double initial_witch_doctor_range_c = 3.0;

const char * const witch_doctor_attack_message_c = "Take Poison!";

Witch_doctor::Witch_doctor(const string& name_, Point location_) :
    Warrior(name_, location_, initial_attack_strength_c, initial_witch_doctor_range_c),
    curing_strength(initial_curing_strength_c), healing(false)
{}

void Witch_doctor::update() 
{
	// First, Update according the Warrior Behavior for attacking behaviors.
	Warrior::update();
	// Witch_doctor can only in one of healing/attacking state, therefore return if attacking.
	if (is_attacking()) {
		return;
    }
	// If the Witch_doctor is not healing patients, then choose the one with weakest health 
	// within his range to heal; and change state to healing.
	if (!healing) {
		auto patient = Model::get_instance().get_weakest_agent_in_range(shared_from_this(), initial_witch_doctor_range_c);
		if(patient) {
            initiate_healing(patient);
		}
	// If the current state is healing, then check if the target is still alive, and that
	// the target is still in range. If not, change healing to false and output msg accordingly.
	} else {
		// Test if the patient is still alive.
		auto target_ptr = target.lock();
		if(!target_ptr || !target_ptr -> is_alive()) {
			cout << get_name() << ": Patient is dead." << endl;
            stop_healing();
            return;
		}
		// Test if the patient is in range. 
		double distance = cartesian_distance(get_location(), target_ptr -> get_location());
		if(distance > initial_witch_doctor_range_c) {
			cout << get_name()  << ": Patient is now out of range." << endl;
            stop_healing();
            return;
		}
        // Test if the patient is now healed.
		if (target_ptr -> get_health() == initial_health_c) {
			cout << get_name()  << ": Patient is now perfectly healthy!" << endl;
            stop_healing();
            return;
		}	
        // Test if the patient is now attacking other Agents.
        if (target_ptr -> is_attacking()) {
            cout << get_name() << ": Patient is now in fight with other Agents!" <<endl;
            stop_healing();
            return;
        }
		// Continue healing process.
		cout << get_name() << ": I'm healing " << target_ptr -> get_name() << "!"<< endl;
		target_ptr -> gain_health(curing_strength);
	}
}

void Witch_doctor::describe() const 
{
	// First, Describe Witch_doctor's warrior behavior.
	cout << "Witch_doctor ";
	Warrior::describe();
	auto target_ptr = target.lock();
	if (!healing)
		cout << "   Not healing" << endl;
	else {
		if (target_ptr)
			cout << "   Healing " << target_ptr -> get_name() << endl;
		else
			cout << "   Healing dead patient" << endl;
	}
}

void Witch_doctor::start_healing(shared_ptr<Agent> target_ptr) 
{
	// The Witch_doctor can't heal himself when he is counter-attacking
	// his aggressor.
    if(is_attacking()) {
        throw Error(get_name() + ": I'm busy counter-attacking!");
    } 
    // The Witch_doctor cannot heal dead patients.
	if(!target_ptr -> is_alive()) {
        throw Error(get_name()+ ": I can't save dead patients!");
    }
	if (target_ptr -> is_attacking()) {
		throw Error(get_name()+ ": I don't want to catch myself between Warriors!");
	}
    double distance = cartesian_distance(get_location(), target_ptr -> get_location());
    if(distance > initial_witch_doctor_range_c) {
        throw Error(get_name() + ": Patient is out of range!");
    }
    if (target_ptr -> get_health() == initial_health_c) {
    	throw Error(get_name() + ": This patient is perfectly healthy!");
    }
    initiate_healing(target_ptr);
}

void Witch_doctor::take_hit(int attack_strength, shared_ptr<Soldier> soldier_ptr)
{
    under_attack(attack_strength);
    if(can_run_away(soldier_ptr)) {
		auto farthest_structure_ptr = Model::get_instance().get_farthest_structure_ptr(shared_from_this());
		cout << get_name() << ": Soldier's attacking me! I'm going to run away to " 
             << farthest_structure_ptr -> get_name() << endl;
		move_to(farthest_structure_ptr -> get_location());
	}
}

void Witch_doctor::take_hit(int attack_strength, shared_ptr<Archer> archer_ptr)
{
    under_attack(attack_strength);
    if(can_counter_attack(archer_ptr)) {
		move_to(archer_ptr -> get_location());
        initiate_attacking("Archer, I'm coming for you!", archer_ptr);
    }
}

void Witch_doctor::take_hit(int attack_strength, shared_ptr<Witch_doctor> doctor_ptr)
{
    int remedy = rand() % doctor_remedy_rand_range_c;
    if(remedy < doctor_remedy_threshold_c) {
        cout << get_name() << ": I used the wrong medicine!" << endl;
        // lose all health and die immediately
        lose_health(initial_health_c);
    } else {
        cout << get_name() << ": I found the remedy! I healed myself!" << endl;
    }
    if(can_counter_attack(doctor_ptr)) {
        initiate_attacking("Enjoy my poison, Doctor!", doctor_ptr);
    }
}

void Witch_doctor::dispatch_hit()
{
    auto target_ptr = get_target();
    assert(target_ptr);
    cout << get_name()  << ": " << witch_doctor_attack_message_c << endl;
	// Call target's take_hit function with target's exact type.
	// Dispatch to appropriate action.
	get_target() -> take_hit(get_strength(), static_pointer_cast<Witch_doctor>(shared_from_this()));
}

void Witch_doctor::initiate_healing(shared_ptr<Agent> target_ptr)
{
	cout << get_name() << ": Start healing " << target_ptr -> get_name() << "!" << endl;
	target = target_ptr;
	healing = true;
	return;
}

void Witch_doctor::stop_healing()
{
	healing = false;
	target.reset();
	return;
}

void Witch_doctor::under_attack(int attack_strength)
{
    stop_healing();
    lose_health(attack_strength);
}
