#ifndef WARRIOR_H
#define WARRIOR_H

#include "Agent.h"
#include <memory>

class Soldier;
class Archer;
class Witch_doctor;

class Warrior : public Agent {
public:
    Warrior(const std::string& name_, Point location_, int strength_, double range_);
    
    // Override is_attacking
    bool is_attacking() const override
    {return attacking;}

    // update warrior status
    void update() override;

    // Make this Warrior start attacking the target Agent.
    // Throws an exception if the target is the same as this Agent,
    // is out of range, or is not alive.
    void start_attacking(std::shared_ptr<Agent> target_ptr) override;
        
    // Overrides Agent's stop to print a message
    void stop() override;

    // output information about the current state
    void describe() const override;
	
protected:
    // getters
	double get_range() const
    {return range;}
	int get_strength() const
	{return strength;}
	std::shared_ptr<Agent> get_target() const
	{return target.lock();}

    // print the attack word and start attacking the specified target
    void initiate_attacking(const std::string& message, std::shared_ptr<Agent> target_ptr);

private:
    int strength;
    double range;
    std::weak_ptr<Agent> target;
    bool attacking;

    // First prints out what the warrior says when attacking.
	// Then let the target take hit. This shall be overriden by derived classes
    // so that the exact attacker type can be used.
    virtual void attack() = 0;
};

#endif
