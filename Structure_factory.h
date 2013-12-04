#ifndef STRUCTURE_FACTORY_H
#define STRUCTURE_FACTORY_H

#include <memory>
#include <string>

class Structure;
struct Point;

// Create and return the pointer to the specified Structure type. If the type
// is unrecognized, throws Error("Trying to create structure of unknown type!")
// The Structure is allocated with new and handled by smart pointers.
// When all shared pointers associated with a Structure go out of scope, the Structure will be automatically deleted.
std::shared_ptr<Structure> create_structure(const std::string& name, const std::string& type, Point location);

#endif
