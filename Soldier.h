#ifndef SOLDIER_H
#define SOLDIER_H

#include "Warrior.h"

class Soldier : public Warrior {
public:
    Soldier(const std::string& name_, Point location_);

    // Overrides Agent's take_hit to counterattack when attacked.
    void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	// Override Soldier under attack from Soldier behavior.
    void take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr) override;
	// Override Soldier under attack from Archer behavior.
    void take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr) override;
	// Override Soldier under attack from Witch_doctor behavior.
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr) override;

    // output information about current state of the soldier
    void describe() const override;

    // override Warrior behavior to print "Clang!"
    void print_attack_word() const override;

	void attack() override;

};

#endif
