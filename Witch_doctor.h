#ifndef WITCH_DOCTOR_H
#define WITCH_DOCTOR_H

#include "Warriors.h"
#include <memory>

class Witch_doctor : public Warrior {
public:
	Witch_doctor (const std::string& name, Point location_);
	// Implements Doctor's behavior.
	void update() override;
	// Overrides Warrior's stop behavior.
	void stop() override;
	// Output information about current state.
	void describe() const override;
	// Override Warrior's attack function, Doctor can't voluntarily attack.
    void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	// Start the healing process.
	// Throws exception if the target is out of range, or not alive.
	void start_healing (std::shared_ptr<Agent> target_ptr) override; 
	// Override Agent't take_hit to poison the attacker
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
 	// Override Warrior's behavior to print "Take Poison!"
	void print_attack_word() const override;
        

private:
	int curing_strength;
	std::weak_ptr<Agent> target;
	enum State_e {ATTACKING, HEALING, NOT_HEALING};
	State_e state;
};

#endif
