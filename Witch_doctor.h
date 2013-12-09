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
	
	// Override Witch_doctor's under attack behavior from Soldier.
    // Running away to the farthest structure, starts healing himself once out of Soldier's
    // attack-range(implemented in update).
    void take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr) override;
	
	// Override Witch_doctor's under attack behavior from Archer.
    // Since by Witch_doctor has no advange by running away from Archer whose range is far,
    // Witch_doctor will run towards his attacker and start attacking.
    void take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr) override;
	
	// Override Witch_doctor's under attack behavior from Witch_doctor.
    // This should never be called since doctor cannot be commanded to attack others.
    // They will only counter-attack if they are attacked by Archer.
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr) override;

    // First print "Take Poison!". Then override Warrior's attack() so that target 
    // can react specifically when attacked by Witch_doctor.
	void attack() override;

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
