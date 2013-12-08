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

    // take_hit for each type of warrior
    void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override = 0;
    virtual void take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr) = 0;
    virtual void take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr) = 0;
    virtual void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr) = 0;

protected:
    // getters for attacking state and range value
    double get_range() const
    {return range;}

    // set attacking to true
    void initiate_attacking()
    {attacking = true;}
    // set the target as specified
    void set_target(std::shared_ptr<Agent> target_)
    {target = target_;}

private:
    int strength;
    double range;
    std::weak_ptr<Agent> target;
    bool attacking;

    // Prints out what the warrior says when attacking.
    // This has empty implementation in case some future warriors
    // are silent when attacking.
    virtual void print_attack_word() const
    {}
};

#endif
