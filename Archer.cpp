#include "Archer.h"
#include "Soldier.h"
#include "Witch_doctor.h"
#include "Model.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::string; using std::static_pointer_cast;

// the initial attacking strength of an archer
const int initial_archer_strength_c = 1;
// the initial attacking range of an archer
const double initial_archer_range_c = 6.0;
// the message printed when attacking
const char* const archer_message_c = "Twang!";
// the attack range within which the archer uses dagger
const int initial_dagger_range_c = 2;
// the attack strength of a dagger
const int initial_dagger_strength_c = 2;

Archer::Archer(const string& name_, Point location_) :
    Warrior(name_, location_, initial_archer_strength_c, initial_archer_range_c),
    dagger_strength(initial_dagger_strength_c), dagger_range(initial_dagger_range_c)
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
        initiate_attacking();
        cout << get_name() << ": I'm attacking!" << endl;
    }
}

void Archer::take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr)
{
	lose_health(attack_strength);
	if(!is_attacking() && is_alive() && attacker_ptr->is_alive()) {
		auto farthest_structure_ptr = Model::get_instance().get_farthest_structure_ptr(shared_from_this());
		cout << get_name() << ": I'm going to run away to " << farthest_structure_ptr->get_name() << endl;
		move_to(farthest_structure_ptr->get_location());
	}
}

void Archer::take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr)
{
	lose_health(attack_strength);
	if (is_alive() && attacker_ptr -> is_alive()) {
        double dist = cartesian_distance(get_location(), attacker_ptr->get_location());
		if (dist <= dagger_range) {
			set_strength(initial_dagger_strength_c);
			cout << get_name() << ": I'm drawing my dagger!" << endl;
		}
	}
}

void Archer::take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr)
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

void Archer::attack()
{
    cout << get_name()  << ": " << archer_message_c << endl;
	get_target() -> take_hit(get_strength(), static_pointer_cast<Archer>(shared_from_this()));
}
