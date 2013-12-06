#include "Witch_doctor.h"
#include "Model.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>
#include <cassert>
using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::string; using std::dynamic_pointer_cast;


// the initial attacking strength of a soldier
const int initial_attack_strength_c = 1;
// the initial attacking range of a soldier
const double initial_Witch_doctor_range_c = 3.0;

const char * const Witch_doctor_attack_message_c = "Take Poison!";

Witch_doctor::Witch_doctor(const string& name_, Point location_) :
    Warrior(name_, location_, initial_attack_strength_c, initial_Witch_doctor_range_c),
    curing_strength(1), healing(false)
{}

void Witch_doctor::update() 
{
	double distance;
	// First, Update according the Warrior Behavior for attacking behaviors.
	Warrior::update();
	// Witch_doctor can only in one of healing/attacking state, therefore return if attacking.
	if (is_attacking())
		return;
	// If the Witch_doctor is not healing patients, then choose the one with weakest health 
	// within his range to heal; and change state to healing.
	if (!healing) {
		auto patient = Model::get_instance().get_weakest_agent_in_range(shared_from_this(), initial_Witch_doctor_range_c);
		if (patient) {
			target = patient;
			healing = true;
			cout << get_name() << ": Start healing " << patient->get_name() << "!" << endl;
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
		distance = cartesian_distance(get_location(), target_ptr -> get_location());
		if(distance > initial_Witch_doctor_range_c) {
			cout << get_name()  << ": Patient is now out of range." << endl;
            stop_healing();
            return;
		}
        // Test if the patient is now healed.
		if (target_ptr->get_health() == 5) {
			cout << get_name()  << ": Patient is now perfectly healthy!" << endl;
            stop_healing();
            return;
		}	
        // Test if the patient is now attacking other Agents.
        if (target_ptr->is_attacking()) {
            cout << get_name() << ": Patient is now in fight with other Agents!" <<endl;
            stop_healing();
            return;
        }
		// Start healing process.
		target = target_ptr;
		cout << get_name() << ": I'm healing " << target_ptr->get_name() << "!"<< endl;
		target_ptr->gain_health(curing_strength);
		healing = true;		
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
			cout << "   Healing " << target_ptr->get_name() << endl;
		else
			cout << "   Healing dead patient" << endl;
	}
}

void Witch_doctor::start_attacking(std::shared_ptr<Agent> target_ptr)
{
	// Witch_doctor cannot voluntarily attack other Agents.
	throw Error ( get_name() + ": I don't want to attack!");
}

void Witch_doctor::start_healing (std::shared_ptr<Agent> target_ptr) 
{
	// The Witch_doctor can heal himself when he is counter-attacking
	// his aggressor.
    if(is_attacking()) {
        throw Error(get_name() + ": I'm busy counter-attacking!");
    } 
    // The Witch_doctor cannot heal dead patients.
	if(!target_ptr -> is_alive()) {
        throw Error(get_name()+ ": I can't save dead patients!");
    }
	if (target_ptr->is_attacking()) {
		throw Error(get_name()+ ": I don't want to caught myself between Warriors!");
	}
    double distance = cartesian_distance(get_location(), target_ptr -> get_location());
    if(distance > initial_Witch_doctor_range_c) {
        throw Error(get_name() + ": Patient is out of range!");
    }
    if (target_ptr->get_health() == 5) {
    	throw Error(get_name() + ": This patient is perfectly healthy!");
    }
	cout << get_name() << ": I'm healing " << target_ptr->get_name() << "!"<< endl;
	target_ptr->gain_health(curing_strength);
	target = target_ptr;
	healing = true;
}

void Witch_doctor::take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) {
	healing = false;
    lose_health(attack_strength);
    if(!is_attacking() && is_alive() && attacker_ptr -> is_alive()) {
        cout << get_name() << ": I'm counter-attacking!" << endl;
        Warrior::start_attacking();
        set_target(attacker_ptr);
    }
}

void Witch_doctor::print_attack_word() const {
    cout << get_name()  << ": " << Witch_doctor_attack_message_c << endl;
}

void Witch_doctor::stop_healing() {
	healing = false;
	target.reset();
	return;
}