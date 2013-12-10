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

	// Start the healing process.
	// Throws exception if the target is out of range, or not alive.
	void start_healing(std::shared_ptr<Agent> target_ptr) override; 
	
	// Override Witch_doctor's under attack behavior from Soldier.
    // Running away to the farthest structure, starts healing himself once out of Soldier's
    // attack-range(implemented in update).
    void take_hit(int attack_strength, std::shared_ptr<Soldier> soldier_ptr) override;
	
	// Override Witch_doctor's under attack behavior from Archer.
    // Since by Witch_doctor has no advange by running away from Archer whose range is far,
    // Witch_doctor will run towards his attacker and start attacking.
    void take_hit(int attack_strength, std::shared_ptr<Archer> archer_ptr) override;
	
	// Override Witch_doctor's under attack behavior from Witch_doctor.
    // Doctor will try to find a remedy, but he may intead find a lethal poison since he's in a hurry.
    // If a remedy is found, he will not lose health this time. But if a poison is found, he will die immediately.
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> doctor_ptr) override;

    // First print "Take Poison!". Then override Warrior's attack() so that target 
    // can react specifically when attacked by Witch_doctor.
	void dispatch_hit() override;

private:
	int curing_strength;
    // the agent that the doctor is healing
	std::weak_ptr<Agent> heal_target;
	bool healing;

    // Start healing process, save the target pointer.
    void initiate_healing(std::shared_ptr<Agent> target_ptr);
    // Stop healing process, discarding target and set healing to false.
    void stop_healing();
    // When attacked, stop healing process and lose health
    void under_attack(int attack_strength);
};

#endif
