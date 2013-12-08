#ifndef WITCH_DOCTOR_H
#define WITCH_DOCTOR_H

#include "Warrior.h"
#include <memory>

class Witch_doctor : public Warrior {
public:
	Witch_doctor (const std::string& name, Point location_);
	// Implements Doctor's behavior.
	void update() override;
	// Output information about current state.
	void describe() const override;
	// Override Warrior's attack function, Doctor can't voluntarily attack.
    void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	// Start the healing process.
	// Throws exception if the target is out of range, or not alive.
	void start_healing (std::shared_ptr<Agent> target_ptr) override; 
	// Override Agent't take_hit to poison the attacker
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
    void take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr) override
    {}
    void take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr) override
    {}
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr) override
    {}

 	// Override Warrior's behavior to print "Take Poison!"
	void print_attack_word() const override;
        
private:
	int curing_strength;
	std::weak_ptr<Agent> target;
	bool healing;

    // Start healing process, save the target pointer.
    void initiate_healing(std::shared_ptr<Agent> target_ptr);
    // Stop healing process, discarding target and set healing to false.
    void stop_healing();
};

#endif
