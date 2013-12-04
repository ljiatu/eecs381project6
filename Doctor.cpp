#include "Doctor.h"
#include "Model.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::string; using std::vector;
using std::for_each; using std::map;
// the initial healing strength of a doctor
const int initial_healing_strength_c = 5;
// the initial attacking strength of a soldier
const int initial_attack_strength_c = 1;
// the initial attacking range of a soldier
const double initial_doctor_range_c = 3.0;

const char * const doctor_attack_message_c = "Take Poison!";
const char * const doctor_heal_message_c = "I'm Healing!";

Doctor::Doctor(const string& name_, Point location_) :
    Warrior(name_, location_, initial_attack_strength_c, initial_doctor_range_c), 
    curing_strength(initial_healing_strength_c), healing (false)
{}

void Doctor::update() {
	using Agent_t = map<string, shared_ptr<Agent>>;
	vector<Agent_t::value_type> reachable_agents = Model::get_instance().get_agents_in_range(shared_from_this(), get_range());
	if (reachable_agents.size()) {
		healing = true;
		for_each(reachable_agents.begin(), reachable_agents.end(), [&](Agent_t::value_type agent_pair)
			{agent_pair.second->gain_health(curing_strength);});
	}
	Warrior::update();
}
// Stop Healing and Moving, override Warrior's behavior.
void Doctor::stop() {
	Agent::stop();
	if (healing) {
		healing = false;
		cout << "I'm stopping healing!" << endl;
	}
}
// Output information about current state.
void Doctor::describe() const {
    if(!healing)
        cout << "   Not healing" << endl;
    else 
    	cout << "   Healing" << endl;
	Warrior::describe();
}

void Doctor::start_attacking(std::shared_ptr<Agent> target_ptr) {
	cout << get_name() << ": I don't want to attack!" << endl;
}
// Start the healing process.
// Throws exception if the target is out of range, or not alive.
void Doctor::start_healing (std::shared_ptr<Agent> target_ptr) {
	target_ptr->gain_health(curing_strength);
    cout << get_name()  << ": " << doctor_heal_message_c << endl;
}
// Override Agent't take_hit to poison the attacker
void Doctor::take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) {
	Warrior::take_hit(attack_strength, attacker_ptr);
}
	// Override Warrior's behavior to print "Take Poison!"
void Doctor::print_attack_word() const {
    cout << get_name()  << ": " << doctor_attack_message_c << endl;
}

