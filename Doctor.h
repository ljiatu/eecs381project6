#ifndef DOCTOR_H
#define DOCTOR_H

#include "Warriors.h"
#include <memory>

class Doctor : public Warrior {
public:
	Doctor (const std::string& name, Point location_);
	// Implements Doctor's behavior.
	void update() override;
	// Stop Healing and Moving, override Warrior's behavior.
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
	bool healing;
};

#endif