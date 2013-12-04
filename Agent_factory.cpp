#include "Agent_factory.h"
#include "Peasant.h"
#include "Warriors.h"
#include "Doctor.h"
#include "Utility.h"

using std::shared_ptr;
using std::string;

shared_ptr<Agent> create_agent(const string& name, const string& type, Point location)
{
    if(type == "Peasant") {
        return shared_ptr<Agent>(new Peasant(name, location));
    } else if(type == "Soldier") {
        return shared_ptr<Agent>(new Soldier(name, location));
    } else if(type == "Archer") {
        return shared_ptr<Agent>(new Archer(name, location));
    } else if(type == "Doctor") {
    	return shared_ptr<Agent>(new Doctor(name, location));
    } else {
        throw Error("Trying to create agent of unknown type!");
    }
}
