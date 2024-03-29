#ifndef SOLDIER_H
#define SOLDIER_H

#include "Warrior.h"

class Soldier : public Warrior {
public:
    Soldier(const std::string& name_, Point location_);

	// Override Soldier under attack from Soldier behavior.
    // Soldier will draw his shield to defend himself. But he doesn's succeed in doing so
    // everytime. If he succeeds, he will not lose_health in this round.
    void take_hit(int attack_strength, std::shared_ptr<Soldier> soldier_ptr);
	
	// Override Soldier under attack from Archer behavior.
    // Soldier's not smart enough to choose best action, therefore he choose to blindly attack!
    void take_hit(int attack_strength, std::shared_ptr<Archer> archer_ptr) override;
	
	// Override Soldier under attack from Witch_doctor behavior.
    // Soldier will step towards the doctor and attack it.
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> doctor_ptr) override;

    // output information about current state of the soldier
    void describe() const override;

	// Override Warrior's attack() so that the correct message ("Clang") is printed
    // and target can react specifically when attacked by Soldier.
	void dispatch_hit() override;
};

#endif
