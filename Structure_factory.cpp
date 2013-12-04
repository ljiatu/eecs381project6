#include "Structure_factory.h"
#include "Farm.h"
#include "Town_Hall.h"
#include "Utility.h"

using std::shared_ptr;
using std::string;

shared_ptr<Structure> create_structure(const string& name, const string& type, Point location)
{
    if(type == "Farm") {
        return shared_ptr<Structure>(new Farm(name, location));
    } else if(type == "Town_Hall") {
        return shared_ptr<Structure>(new Town_Hall(name, location));
    } else {
        throw Error("Trying to create structure of unknown type!");
    }
}
