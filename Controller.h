/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

class Agent;
class View;
class Local_view;
class Map_view;
class Health_view;
class Amounts_view;
class Combat_view;

class Controller {
public:	
	Controller();

	// create View object, run the program by acccepting user commands, 
    // then destroy View object by destroying all View containers and reseting
    // shared pointers to Views
	void run();

private:
    using View_command_func_t = void (Controller::*)() const;
    using Program_command_func_t = void (Controller::*)();
    using Agent_command_func_t = void (Controller::*)(std::shared_ptr<Agent>) const;

    // maps command strings to different types of commands
    std::map<std::string, View_command_func_t> map_view_commands;
    std::map<std::string, Program_command_func_t> program_commands;
    std::map<std::string, Agent_command_func_t> agent_commands;
    
    // All views opened are stored in this container.
    std::vector<std::shared_ptr<View>> view_ptrs;
    // views are also seperately stored based on their types
    std::map<std::string, std::shared_ptr<Local_view>> local_view_ptrs;
    std::map<std::string, std::shared_ptr<Combat_view>> combat_view_ptrs;
    std::shared_ptr<Map_view> map_view_ptr;
    std::shared_ptr<Health_view> health_view_ptr;
    std::shared_ptr<Amounts_view> amounts_view_ptr;

    /* The following functions throw an exception if no map view is open */
    // Restore the default settings of the map view.
    void restore() const;
    // Change the size of the map view as specified.
    void size() const;
    // Change the scale of the map view as specified.
    void zoom() const;
    // Change the origin of the map view as specified.
    void pan() const;

    // Open the specified view. Throw an exception if the view is already open
    // or no object/view is of the specified name.
    void open();
    // Close the specified view. Throw an exception if no view of that name is open.
    void close();
    // Have all objects describe themselves.
    void status();
    // Tell all views to draw themselves.
    void show();
    // Have all objects update themselves.
    void go();
    // Build a new structure as specified and add it to the Model.
    // Throw an exception if any of the specified information is invalid.
    void build();
    // Train a new agent as specified and add s/he to the Model.
    // Throw an exception if any of the specified information is invalid.
    void train();

    // Tell the specified agent to move to the specified location.
    void move(std::shared_ptr<Agent> agent_ptr) const;
    // Tell the specified agent to work.
    void work(std::shared_ptr<Agent> agent_ptr) const;
    // Tell the specified agent to attack another agent.
    void attack(std::shared_ptr<Agent> agent_ptr) const;
    // Tell the specified agent to stop whatever s/he is doing.
    void stop(std::shared_ptr<Agent> agent_ptr) const;

    // Attach the view to Model and add the pointer to the sequential container
    // or associate it with a shared_ptr.
    void add_view_ptr(std::shared_ptr<View> view_ptr);
    // Detach the view from the Model and remove the pointer from the sequential container
    // or resetting the specific shared_ptr.
    void remove_view_ptr(std::shared_ptr<View> view_ptr);
};

#endif
