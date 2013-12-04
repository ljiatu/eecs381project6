/* 
A Peasant is an Agent that can move food between Structures. It can be commanded to
start_working, whereupon it moves to the source, picks up food, returns to destination,
deposits the food, returns to source. If picks up zero food at the source, it waits there
and tries again on the next update. 
If commanded to move_to somewhere, it stops working, and goes there.
*/

#ifndef PEASANT_H
#define PEASANT_H

#include "Agent.h"
#include <memory>

class Peasant : public Agent {
public:
    Peasant(const std::string& in_name, Point in_location);

    // implement Peasant behavior
    void update() override;
    
    // overridden to suspend working behavior
    void move_to(Point dest) override;
    
    // stop moving and working
    void stop() override;

    // starts the working process
    // Throws an exception if the source is the same as the destination.
    void start_working(std::shared_ptr<Structure> source_,
                       std::shared_ptr<Structure> destination_) override;

    // output information about the current state
    void describe() const override;

    // ask Model to broadcast our current state to all Views
    void broadcast_current_state() const override;

private:
    enum Working_state_e {NOT_WORKING, INBOUND, COLLECTING, OUTBOUND, DEPOSITING};
    double amount;
    std::shared_ptr<Structure> source;
    std::shared_ptr<Structure> destination;
    Working_state_e work_state;

    // Tell the peasant to stop working and forget about
    // any working information the peasant currently has
    void stop_working();
};

#endif
