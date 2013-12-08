#ifndef ARCHER_H
#define ARCHER_H

#include "Warrior.h"

class Archer : public Warrior {
public:
    Archer(const std::string& name_, Point location_);

    // overrides Warrior's update so that the archer 
    // always try to find the closest target to attack at
    void update() override;

    // Overrides Agent's take_hit to run away when attacked.
    void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
    void take_hit(int attack_strength, std::shared_ptr<Soldier> attacker_ptr) override
    {}
    void take_hit(int attack_strength, std::shared_ptr<Archer> attacker_ptr) override
    {}
    void take_hit(int attack_strength, std::shared_ptr<Witch_doctor> attacker_ptr) override
    {}

    // output information about current state of the archer
    void describe() const override;

    // override Warrior behavior to print "Twang!"
    void print_attack_word() const override;
};

#endif
