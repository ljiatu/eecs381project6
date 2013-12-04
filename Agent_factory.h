#ifndef AGENT_FACTORY_H
#define AGENT_FACTORY_H

#include <memory>
#include <string>

class Agent;
struct Point;

// Create and return the shared pointer to the specified Agent type. If the type
// is unrecognized, throws Error("Trying to create agent of unknown type!")
// The Agent is allocated with new and handled by smart pointers.
// When all shared pointers associated with an Agent go out of scope, the Agent will be automatically deleted.
std::shared_ptr<Agent> create_agent(const std::string& name, const std::string& type, Point location);

#endif
