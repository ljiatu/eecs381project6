#ifndef ARCHER_H
#define ARCHER_H

#include "Warrior.h"

class Archer : public Warrior {
public:
    Archer(const std::string& name_, Point location_);

    // overrides Warrior's update so that the archer 
    // always try to find the closest target to attack at
    void update() override;

	// Override Archer's under attack behavior from Soldier.
    // Archer will run to the farthest structure, then he can use his superior range and
    // attack Soldier on his way.
    void take_hit(int attack_strength, std::shared_ptr<Soldier> soldier_ptr) override;
	
	// Override Archer's under attack behavior from Archer.
    // Archer will draw his dagger to defend himself if the other Archer is within a close range.
    // Else, they will continue shooting each other without running away.
    void take_hit(int attack_strength, std::shared_ptr<Archer> archer_ptr) override;
	
	// Override Archer's under attack behavior from Witch_doctor.
    // The archer will run away to the closest structure and attack the doctor
    // as long as possible on his way.
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> doctor_ptr) override;

    // output information about current state of the archer
    void describe() const override;

    // Override Warrior behavior to print "Twang!". Then override Warrior's attack() 
    // can react specifically when attacked by Archer.
	void attack() override;
};

#endif
