#include "Model.h"
#include "Agent.h"
#include "Agent_factory.h"
#include "Geometry.h"
#include "Structure.h"
#include "Structure_factory.h"
#include "Utility.h"
#include "Views.h"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

using std::copy_if; using std::back_inserter;
using std::make_pair;
using std::map;
using std::min_element;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

const char* const struct_not_found_c = "Structure not found!";
const char* const agent_not_found_c = "Agent not found!";

static string get_initial_letters(const shared_ptr<Sim_object> obj_ptr);

Model& Model::get_instance()
{
    static Model model;
    return model;
}

bool Model::is_name_in_use(const string& name) const
{
    return sim_objects.find(name.substr(0, num_letters_c)) != sim_objects.end();
}

bool Model::get_object_location(Point& point, const string& name) const
{
    auto it = sim_objects.find(name.substr(0, num_letters_c));
    if(it == sim_objects.end() || name != it -> second -> get_name()) {
        // if the first two letters don't match or the whole name doesn't match,
        // we know that the object is not there
        return false;
    } else {
        point = it -> second -> get_location();
        return true;
    }
}

bool Model::is_structure_present(const string& name) const
{
    auto it = structures.find(name.substr(0, num_letters_c));
    if(it == structures.end()) {
        return false;
    }

    // Now that the first two letters match,
    // test if the full name matches the provided name.
    return it -> second -> get_name() == name;
}

void Model::add_structure(shared_ptr<Structure> struct_ptr)
{
    assert(!is_structure_present(struct_ptr -> get_name()));
    string initial_letters = get_initial_letters(struct_ptr);
    sim_objects[initial_letters] = struct_ptr;
    structures[initial_letters] = struct_ptr;
    struct_ptr -> broadcast_current_state();
}

shared_ptr<Structure> Model::get_structure_ptr(const string& name) const
{
    auto it = structures.find(name.substr(0, num_letters_c));
    if(it == structures.end()) {
        throw Error(struct_not_found_c);
    }
    if(it -> second -> get_name() == name) {
        return it -> second;
    } else {
        throw Error(struct_not_found_c);
    }
}

shared_ptr<Structure> Model::get_closest_structure_ptr(shared_ptr<Agent> agent_ptr) const
{
    // Compare which of the two structures is closer to the agent.
    // Return true if the first structure is closer; return false otherwise.
    class Structure_dist_comp {
    public:
        Structure_dist_comp(shared_ptr<Agent> target_) : target(target_)
        {}
        bool operator()(Structures_t::value_type structure1, Structures_t::value_type structure2)
        {
            double dist1 = cartesian_distance((structure1.second) -> get_location(), target -> get_location()),
                   dist2 = cartesian_distance((structure2.second) -> get_location(), target -> get_location());
            return dist1 < dist2;
        }
    private:
        shared_ptr<Agent> target;
    };

    Structure_dist_comp comparator(agent_ptr);
    return min_element(structures.begin(), structures.end(), comparator) -> second;
}

bool Model::is_agent_present(const string& name) const
{
    auto it = agents.find(name.substr(0, num_letters_c));
    if(it == agents.end()) {
        return false;
    }

    return it -> second -> get_name() == name;
}

void Model::add_agent(shared_ptr<Agent> agent_ptr)
{
    assert(!is_agent_present(agent_ptr -> get_name()));
    string initial_letters = get_initial_letters(agent_ptr);
    sim_objects[initial_letters] = agent_ptr;
    agents[initial_letters] = agent_ptr;
    agent_ptr -> broadcast_current_state();
}

shared_ptr<Agent> Model::get_agent_ptr(const string& name) const
{
    auto it = agents.find(name.substr(0, num_letters_c));
    if(it == agents.end()) {
        throw Error(agent_not_found_c);
    }
    if(it -> second -> get_name() == name) {
        return it -> second;
    } else {
        throw Error(agent_not_found_c);
    }
}

shared_ptr<Agent> Model::get_closest_agent_ptr(shared_ptr<Agent> agent_ptr, double range) const
{
    // First find out all agents who are within the attacking range of the specified agent,
    // excluding the specified agent her/himself. All agents will be sorted by name.
    // vector<Agents_t::value_type> reachable_agents;
    // copy_if(agents.begin(), agents.end(), back_inserter(reachable_agents),
    //     [agent_ptr, range](Agents_t::value_type agent)
    //         {return cartesian_distance((agent.second) -> get_location(), agent_ptr -> get_location()) <= range
    //             && agent.second != agent_ptr;});
    // If no body is within range, return a null shared_ptr.

    vector<Agents_t::value_type> reachable_agents = get_agents_in_range(agent_ptr, range);
    if(reachable_agents.empty()) {
        return shared_ptr<Agent>();
    }

    // Compare the distances of two agents with the target agent. 
    // Return true if the first agent is closer; return false otherwise.
    class Agent_dist_comp {
    public:
        Agent_dist_comp(shared_ptr<Agent> target_) : target(target_)
        {}
        bool operator()(Agents_t::value_type agent1, Agents_t::value_type agent2)
        {
            double dist1 = cartesian_distance((agent1.second) -> get_location(), target -> get_location()),
                   dist2 = cartesian_distance((agent2.second) -> get_location(), target -> get_location());
            return dist1 < dist2;
        }
    private:
        shared_ptr<Agent> target;
    };

    // find the closest agent from the set of all reachable agents
    Agent_dist_comp comparator(agent_ptr);
    auto it = min_element(reachable_agents.begin(), reachable_agents.end(), comparator);
    return it -> second;
}

vector<std::map<std::string, std::shared_ptr<Agent>>::value_type> Model::get_agents_in_range (std::shared_ptr<Agent> agent_ptr, double range) const
{
    vector<std::map<std::string, std::shared_ptr<Agent>>::value_type> reachable_agents;
    copy_if(agents.begin(), agents.end(), back_inserter(reachable_agents),
        [agent_ptr, range](Agents_t::value_type agent)
            {return cartesian_distance((agent.second) -> get_location(), agent_ptr -> get_location()) <= range
                && agent.second != agent_ptr;});
    return reachable_agents;
}

void Model::remove_agent(shared_ptr<Agent> dead_agent)
{
    assert(sim_objects.find(get_initial_letters(dead_agent)) != sim_objects.end());
    assert(agents.find(get_initial_letters(dead_agent)) != agents.end());
    sim_objects.erase(get_initial_letters(dead_agent));
    agents.erase(get_initial_letters(dead_agent));
}

void Model::describe() const
{
    for(auto it = sim_objects.begin(); it != sim_objects.end(); ++it) {
        it -> second -> describe();
    }
}

void Model::update()
{
    ++time;
    for(auto it = sim_objects.begin(); it != sim_objects.end(); ++it) {
        it -> second -> update();
    }
}

void Model::attach(shared_ptr<View> view_ptr)
{
    assert(views.find(view_ptr) == views.end());
    views.insert(view_ptr);
    for(auto it = sim_objects.begin(); it != sim_objects.end(); ++it) {
        it -> second -> broadcast_current_state();
    }
}

void Model::detach(shared_ptr<View> view_ptr)
{
    views.erase(view_ptr);
}

void Model::notify_location(const string& name, Point location)
{
    for(shared_ptr<View> ptr : views) {
        ptr -> update_location(name, location);
    }
}

void Model::notify_health(const string& name, int health)
{
    for(shared_ptr<View> ptr : views) {
        ptr -> update_health(name, health);
    }
}

void Model::notify_amount(const string& name, double amount)
{
    for(shared_ptr<View> ptr : views) {
        ptr -> update_amount(name, amount);
    }
}

void Model::notify_gone(const string& name)
{
    for(shared_ptr<View> ptr : views) {
        ptr -> update_remove(name);
    }
}

Model::Model() : time(0)
{
    // create all specified structures and agents
    shared_ptr<Structure> structure_ptr;
    structure_ptr = create_structure("Rivendale", "Farm", Point(10., 10.));
    sim_objects[get_initial_letters(structure_ptr)] = structure_ptr;
    structures[get_initial_letters(structure_ptr)] = structure_ptr;
    structure_ptr = create_structure("Sunnybrook", "Farm", Point(0., 30.));
    sim_objects[get_initial_letters(structure_ptr)] = structure_ptr;
    structures[get_initial_letters(structure_ptr)] = structure_ptr;
    structure_ptr = create_structure("Shire", "Town_Hall", Point(20., 20.));
    sim_objects[get_initial_letters(structure_ptr)] = structure_ptr;
    structures[get_initial_letters(structure_ptr)] = structure_ptr;
    structure_ptr = create_structure("Paduca", "Town_Hall", Point(30., 30.));
    sim_objects[get_initial_letters(structure_ptr)] = structure_ptr;
    structures[get_initial_letters(structure_ptr)] = structure_ptr;
    
    shared_ptr<Agent> agent_ptr;
    agent_ptr = create_agent("Pippin", "Peasant", Point(5., 10.));
    sim_objects[get_initial_letters(agent_ptr)] = agent_ptr;
    agents[get_initial_letters(agent_ptr)] = agent_ptr;
    agent_ptr = create_agent("Merry", "Peasant", Point(0., 25.));
    sim_objects[get_initial_letters(agent_ptr)] = agent_ptr;
    agents[get_initial_letters(agent_ptr)] = agent_ptr;
    agent_ptr = create_agent("Zug", "Soldier", Point(20., 30.));
    sim_objects[get_initial_letters(agent_ptr)] = agent_ptr;
    agents[get_initial_letters(agent_ptr)] = agent_ptr;
    agent_ptr = create_agent("Bug", "Soldier", Point(15., 20.));
    sim_objects[get_initial_letters(agent_ptr)] = agent_ptr;
    agents[get_initial_letters(agent_ptr)] = agent_ptr;
    agent_ptr = create_agent("Iriel", "Archer", Point(20., 38.));
    sim_objects[get_initial_letters(agent_ptr)] = agent_ptr;
    agents[get_initial_letters(agent_ptr)] = agent_ptr;
}

// Return the first num_letters_c letters of the name of the object
// pointer to by obj_ptr.
static string get_initial_letters(const shared_ptr<Sim_object> obj_ptr)
{
    return obj_ptr -> get_name().substr(0, num_letters_c);
}
