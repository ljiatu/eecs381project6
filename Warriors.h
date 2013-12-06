/*
A Warrior is an Agent that has attack and defense/escape behaviors. A Soldier can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. An Archer will in addition 
look for a new target if the original target is dead or out of range.

If attacked, a Soldier will counter-attack the attacker
whereas an Archer will run away to the closest structure.
*/

#ifndef WARRIORS_H
#define WARRIORS_H

#include "Agent.h"
#include <memory>

class Warrior : public Agent {
public:
    Warrior(const std::string& name_, Point location_, int strength_, double range_);
    virtual ~Warrior() = 0;
    
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
    // getters for attacking state and range value
    double get_range() const
    {return range;}

    // set attacking to true
    void start_attacking()
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

class Soldier : public Warrior {
public:
    Soldier(const std::string& name_, Point location_);

    // Overrides Agent's take_hit to counterattack when attacked.
    void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;

    // output information about current state of the soldier
    void describe() const override;

    // override Warrior behavior to print "Clang!"
    void print_attack_word() const override;
};

class Archer : public Warrior {
public:
    Archer(const std::string& name_, Point location_);

    // overrides Warrior's update so that the archer 
    // always try to find the closest target to attack at
    void update() override;

    // Overrides Agent's take_hit to run away when attacked.
    void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;

    // output information about current state of the archer
    void describe() const override;

    // override Warrior behavior to print "Twang!"
    void print_attack_word() const override;
};

#endif
