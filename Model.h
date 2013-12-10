/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Structures and Agents there are, but it does not
know about any of their derived classes, nor which Agents are of what kind of Agent. 
It has facilities for looking up objects by name, and removing Agents.  When
created, it creates an initial group of Structures and Agents using the Structure_factory
and Agent_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.

Notice how only the Standard Library headers need to be included - reduced coupling!

*/

#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Agent;
class Model;
class Sim_object;
class Structure;
class View;
struct Point;

class Model {
public:
    // access the singleton model object
    static Model& get_instance();

    // return the current time
    int get_time() {return time;}

    // is name already in use for either agent or structure?
    // either the identical name, or identical in first two characters counts as in-use
    bool is_name_in_use(const std::string& name) const;

    // Return true and the pass location of the object back
    // if the object is present. Return false and does not modify
    // the passed-in point otherwise.
    bool get_object_location(Point& point, const std::string& name) const;

    // is there a structure with this name?
    bool is_structure_present(const std::string& name) const;
    // add a new structure; assumes none with the same name
    void add_structure(std::shared_ptr<Structure>);
    // will throw Error("Structure not found!") if no structure of that name
    std::shared_ptr<Structure> get_structure_ptr(const std::string& name) const;
    // get a pointer to the structure that is the closest to the specified agent
    std::shared_ptr<Structure> get_closest_structure_ptr(std::shared_ptr<Agent> agent_ptr) const;
    // get a pointer to the structure that is the farthest to the specified agent
    std::shared_ptr<Structure> get_farthest_structure_ptr(std::shared_ptr<Agent> agent_ptr) const;
    // is there an agent with this name?
    bool is_agent_present(const std::string& name) const;
    // add a new agent; assumes none with the same name
    void add_agent(std::shared_ptr<Agent> agent_ptr);
    // will throw Error("Agent not found!") if no agent of that name
    std::shared_ptr<Agent> get_agent_ptr(const std::string& name) const;
    // get a pointer to the agent that is the closest to the specified agent
    std::shared_ptr<Agent> get_closest_agent_ptr(std::shared_ptr<Agent> agent_ptr, double range) const;
    // get a pointer to the agent with least health in the specified range
    std::shared_ptr<Agent> get_weakest_agent_in_range(std::shared_ptr<Agent> agent_ptr, double range) const;
    // remove the specified shared_ptr from Model's sim_objects and agents containers
    void remove_agent(std::shared_ptr<Agent> dead_agent);
    
    // tell all objects to describe themselves to the console
    void describe() const;
    // increment the time, and tell all objects to update themselves
    void update();
    
    /* View services */
    // Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information).
    void attach(std::shared_ptr<View>);
    // Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
    void detach(std::shared_ptr<View>);
    // notify the views about an object's location
    void notify_location(const std::string& name, Point location);
    // notify the views about an object's health
    void notify_health(const std::string& name, int health);
    // notify the views about the amount of food that an object is carrying
    void notify_amount(const std::string& name, double amount);
    // notify the views that an object is now gone
    void notify_gone(const std::string& name);
    
private:
    using Structures_t = std::map<std::string, std::shared_ptr<Structure>>;
    using Agents_t = std::map<std::string, std::shared_ptr<Agent>>;
    
    int time;

    // For all maps that follow, the key is the first two letters of the name of the pointed-to object.
    std::map<std::string, std::shared_ptr<Sim_object>> sim_objects;
    Structures_t structures;
    Agents_t agents;
    std::set<std::shared_ptr<View>> views;

    // create the initial objects
    Model();
    // The destructor does nothing. It is here to guarantee that it is private.
    ~Model()
    {}

    // Compare which of the two structures is closer to the agent.
    // Return true if the first structure is further; return false otherwise.
    // This comparator class is private so that the Structures_t type alias can be utilized.
    class Structure_dist_comp {
    public:
        Structure_dist_comp(std::shared_ptr<Agent> target_) : target(target_)
        {}
        bool operator()(Structures_t::value_type structure1, Structures_t::value_type structure2) const;
    private:
        std::shared_ptr<Agent> target;
    };

    // disallow copy/move construction or assignment
    Model(const Model&) = delete;
    Model& operator= (const Model&)  = delete;
    Model(Model&&) = delete;
    Model& operator= (Model&&) = delete;
};

#endif
