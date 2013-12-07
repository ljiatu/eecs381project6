#ifndef WARRIOR_H
#define WARRIOR_H
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


#endif
