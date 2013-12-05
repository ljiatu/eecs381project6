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
const char * const Witch_doctor_heal_message_c = "I'm Healing!";

Witch_doctor::Witch_doctor(const string& name_, Point location_) :
    Warrior(name_, location_, initial_attack_strength_c, initial_Witch_doctor_range_c)
{state = NOT_HEALING;}

void Witch_doctor::update() 
{
	shared_ptr<Agent> closest_agent_ptr;
	shared_ptr<Agent> target_ptr;
	double distance;

	Warrior::update();
	if (is_attacking())
		return;
	if (healing) {
		closest_agent_ptr = Model::get_instance().get_closest_agent_ptr(shared_from_this(), get_range());
		if (closest_agent_ptr) {
			cout << get_name() << ": Found a Patient!" << endl;
			set_target(closest_agent_ptr);
			healing = true;
			}
	} else {
		target_ptr = target.lock();
		if(!target_ptr || !target_ptr -> is_alive()) {
			// If the target disappears or the target is still there but not alive,
			// stop attacking and forget about the target.
			cout << get_name() << ": Patient is dead" << endl;
			healing = false;
			target.reset();
			return;
		}
		distance = cartesian_distance(get_location(), target_ptr -> get_location());
		if(distance > initial_Witch_doctor_range_c) {
			cout << get_name()  << ": Patient is now out of range" << endl;
			healing = false;
			return;
		}	
		cout << get_name() << ": I'm healing " << target_ptr->get_name() << "!"<< endl;
		target_ptr -> gain_health (curing_strength);
	}
}

void Witch_doctor::describe() const 
{
	Warrior::describe();
	if (is_attacking())
		return;
	auto target_ptr = target.lock();
	if (!healing)
		cout << "	Not healing" << endl;
	else {
		if (target_ptr)
			cout << "	Healing" << target_ptr->get_name() << endl;
		else
			cout << "	Healing dead patient" << endl;
	}
}

void Witch_doctor::start_attacking(std::shared_ptr<Agent> target_ptr)
{
	throw Error ( get_name() + ": I don't want to attack!" + "\n");
}

void Witch_doctor::start_healing (std::shared_ptr<Agent> target_ptr) 
{
    if(target_ptr == shared_from_this() && is_attacking()) {
        throw Error(get_name() + ": I’m Busy counterattacking!");
    } 
	if(!target_ptr -> is_alive()) {
        throw Error(get_name()+ ": “I can’t save dead patients!");
    }
	shared_ptr<Warrior> warrior_ptr = dynamic_pointer_cast<Warrior> (target_ptr);
	if(warrior_ptr && warrior_ptr ->is_attacking()) {
		throw Error(get_name()+ ": “I'm not a Medic!");
	}
    double distance = cartesian_distance(get_location(), target_ptr -> get_location());
    if(distance > initial_Witch_doctor_range_c) {
        throw Error(get_name() + ": Patient is out of range!");
    }
	cout << get_name() << ": I'm healing " << target_ptr->get_name() << "!"<< endl;
	target_ptr->gain_health(curing_strength);
    target = target_ptr;
	state = HEALING;
}

void Witch_doctor::take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) {
    lose_health(attack_strength);
    if(!is_attacking() && is_alive() && attacker_ptr -> is_alive()) {
        Warrior::start_attacking();
        set_target(attacker_ptr);
        cout << get_name() << ": Im counter-attacking!" << endl;
		state = ATTACKING;
    }
}

void Witch_doctor::print_attack_word() const {
    cout << get_name()  << ": " << Witch_doctor_attack_message_c << endl;
}

