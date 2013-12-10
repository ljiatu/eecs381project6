#include "Archer.h"
#include "Model.h"
#include "Soldier.h"
#include "Structure.h"
#include "Utility.h"
#include "Witch_doctor.h"
#include <cassert>
#include <iostream>

using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;
using std::static_pointer_cast;
using std::string;

// the initial attacking strength of an archer
const int initial_archer_strength_c = 1;
// the initial attacking range of an archer
const double initial_archer_range_c = 6.0;
// the message printed when attacking
const char* const archer_message_c = "Twang!";

// the default armor for an archer is a bow
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
        initiate_attacking("I'm attacking!", closest_agent_ptr);
    }
}

void Archer::take_hit(int attack_strength, shared_ptr<Soldier> soldier_ptr)
{
	lose_health(attack_strength);
    if(can_run_away(soldier_ptr)) {
		auto farthest_structure_ptr = Model::get_instance().get_farthest_structure_ptr(shared_from_this());
        cout << get_name() << ": Soldier's attacking me! I'm going to run away to "
             << farthest_structure_ptr -> get_name() << endl;
		move_to(farthest_structure_ptr -> get_location());
	}
}

void Archer::take_hit(int attack_strength, shared_ptr<Archer> archer_ptr)
{
	lose_health(attack_strength);
	if(can_counter_attack(archer_ptr)) {
        initiate_attacking("I'm fighting another Archer!", archer_ptr);
	}
}

void Archer::take_hit(int attack_strength, shared_ptr<Witch_doctor> doctor_ptr)
{
    lose_health(attack_strength);
    if(can_run_away(doctor_ptr)) {
        auto closest_structure_ptr = Model::get_instance().get_closest_structure_ptr(shared_from_this());
        cout << get_name() << ": Doctor's attacking me! I'm going to run away to " 
             << closest_structure_ptr -> get_name() << endl;
        move_to(closest_structure_ptr -> get_location());
    }
}

void Archer::describe() const
{
    cout << "Archer ";
    Warrior::describe();
}

void Archer::dispatch_hit()
{
    auto target_ptr = get_target();
    assert(target_ptr);
    cout << get_name() << ": " << archer_message_c << endl;
    target_ptr -> take_hit(get_strength(), static_pointer_cast<Archer>(shared_from_this()));
}
