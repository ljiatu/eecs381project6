#include "Controller.h"
#include "Agent.h"
#include "Agent_factory.h"
#include "Model.h"
#include "Structure_factory.h"
#include "Utility.h"
#include "Views.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <new>

using std::any_of;
using std::bad_alloc;
using std::cin; using std::cout; using std::cerr; using std::endl;
using std::find;
using std::for_each;
using std::isalnum;
using std::shared_ptr;
using std::string;

const char* const invalid_name_c = "Invalid name for new object!";
const char* const invalid_double_c = "Expected a double!";
const char* const invalid_int_c = "Expected an integer!";
const char* const view_open_c = "View of that name already open!";
const char* const nonexistent_view_c = "No view of that name is open!";
const char* const map_view_closed_c = "No map view is open!";
const char* const nonexistent_object_c = "No object of that name!";
const char* const invalid_view_type_c = "No view of that type!";
const char* const unknown_command_c = "Unrecognized command!";

static Point read_new_obj_info(string& name, string& type);
static void check_name_validity(const string& name);
static Point read_Point();
static double read_double();
static int read_int();
static void print_error_msg_and_clear_input(const Error& error);

Controller::Controller()
{
    // load command maps
    map_view_commands["default"] = &Controller::restore;
    map_view_commands["size"] = &Controller::size;
    map_view_commands["zoom"] = &Controller::zoom;
    map_view_commands["pan"] = &Controller::pan;

    program_commands["open"] = &Controller::open;
    program_commands["close"] = &Controller::close;
    program_commands["status"] = &Controller::status;
    program_commands["show"] = &Controller::show;
    program_commands["go"] = &Controller::go;
    program_commands["build"] = &Controller::build;
    program_commands["train"] = &Controller::train;

    agent_commands["move"] = &Controller::move;
    agent_commands["work"] = &Controller::work;
    agent_commands["attack"] = &Controller::attack;
    agent_commands["stop"] = &Controller::stop;
}

void Controller::run()
{
    Model& model = Model::get_instance();
    string first_word;

    while(true) {
        cout << "\nTime " << model.get_time() << ": Enter command: ";
        cin >> first_word;

        if(first_word == "quit") {
            for_each(view_ptrs.begin(), view_ptrs.end(), 
                [&model](shared_ptr<View> view_ptr){model.detach(view_ptr);});
            cout << "Done" << endl;
            return;
        }

        try {
            // first test if it is an agent command
            if(model.is_agent_present(first_word)) {
                // In this case, the first word is the name of an agent
                shared_ptr<Agent> agent_ptr = model.get_agent_ptr(first_word);
                assert(agent_ptr -> is_alive());
                string agent_command;
                cin >> agent_command;
                auto it = agent_commands.find(agent_command);
                if(it != agent_commands.end()) {
                    (this ->* (it -> second))(agent_ptr);
                    continue;
                } else {
                    throw Error(unknown_command_c);
                }
            }

            // then test if it is a view command
            auto view_it = map_view_commands.find(first_word);
            if(view_it != map_view_commands.end()) {
                if(map_view_ptr) {
                    (this ->* (view_it -> second))();
                    continue;
                } else {
                    throw Error(map_view_closed_c);
                }
            }

            // finally, test if it is a program command
            auto program_it = program_commands.find(first_word);
            if(program_it != program_commands.end()) {
                (this ->* (program_it -> second))();
            } else {
                throw Error(unknown_command_c);
            }
        } catch(Error& e) {
            print_error_msg_and_clear_input(e);
        } catch(bad_alloc& ba) {
            cerr << ba.what() << endl;
            return;
        } catch(...) {
            cerr << "Unknown Exception caught!" << endl;
            return;
        }
    }
}

void Controller::restore() const
{
    map_view_ptr -> set_defaults();
}

void Controller::size() const
{
    int size = read_int();
    map_view_ptr -> set_size(size);
}

void Controller::zoom() const
{
    double scale = read_double();
    map_view_ptr -> set_scale(scale);
}

void Controller::pan() const
{
    Point origin = read_Point();
    map_view_ptr -> set_origin(origin);
}

void Controller::open()
{
    string type;
    cin >> type;
    if(type == "map") {
        if(map_view_ptr) {
            throw Error(view_open_c);
        } else {
            map_view_ptr = shared_ptr<Map_view>(new Map_view);
            add_view_ptr(map_view_ptr);
        }
    } else if(type == "health") {
        if(health_view_ptr) {
            throw Error(view_open_c);
        } else {
            health_view_ptr = shared_ptr<Health_view>(new Health_view);
            add_view_ptr(health_view_ptr);
        }
    } else if(type == "amounts") {
        if(amounts_view_ptr) {
            throw Error(view_open_c);
        } else {
            amounts_view_ptr = shared_ptr<Amounts_view>(new Amounts_view);
            add_view_ptr(amounts_view_ptr);
        }
    } else if(type == "local") {
        string name;
        cin >> name;
        if(local_view_ptrs.find(name) != local_view_ptrs.end()) {
            throw Error(view_open_c);
        }
        Point point;
        bool exists = Model::get_instance().get_object_location(point, name);
        if(exists) {
            // if the object exists, 
            // add the Local_view pointer to all relevant containers
            shared_ptr<Local_view> local_view_ptr(new Local_view(name, point));
            local_view_ptrs[name] = local_view_ptr;
            add_view_ptr(local_view_ptr);
        } else {
            throw Error(nonexistent_object_c);
        }
    } else if(type == "combat") {
        string name;
        cin >> name;
        if(combat_view_ptrs.find(name) != combat_view_ptrs.end()) {
            throw Error(view_open_c);
        }
        Point point;
        bool exists = Model::get_instance().get_object_location(point, name);
        if(exists) {
            shared_ptr<Combat_view> combat_view_ptr(new Combat_view(name, point));
            combat_view_ptrs[name] = combat_view_ptr;
            add_view_ptr(combat_view_ptr);
        } else {
            throw Error(nonexistent_object_c);
        }
    } else {
        throw Error(invalid_view_type_c);
    }
}

void Controller::close()
{
    string type;
    cin >> type;
    // check each type of the map, and then local map
    // if none of the three specific maps matches
    if(type == "map") {
        if(!map_view_ptr) {
            throw Error(nonexistent_view_c);
        } else {
            remove_view_ptr(map_view_ptr);
            map_view_ptr.reset();
        }
    } else if(type == "health") {
        if(!health_view_ptr) {
            throw Error(nonexistent_view_c);
        } else {
            remove_view_ptr(health_view_ptr);
            health_view_ptr.reset();
        }
    } else if(type == "amounts") {
        if(!amounts_view_ptr) {
            throw Error(nonexistent_view_c);
        } else {
            remove_view_ptr(amounts_view_ptr);
            amounts_view_ptr.reset();
        }
    } else if(type == "local") {
        string name;
        cin >> name;
        auto it = local_view_ptrs.find(name);
        if(it == local_view_ptrs.end()) {
            throw Error(nonexistent_view_c);
        } else {
            remove_view_ptr(it -> second);
            local_view_ptrs.erase(it);
        }
    } else if(type == "combat") {
        string name;
        cin >> name;
        auto it = combat_view_ptrs.find(name);
        if(it == combat_view_ptrs.end()) {
            throw Error(nonexistent_view_c);
        } else {
            remove_view_ptr(it -> second);
            combat_view_ptrs.erase(it);
        }
    } else {
        throw Error(invalid_view_type_c);
    }
}

void Controller::status()
{
    Model::get_instance().describe();
}

void Controller::show()
{
    for(auto view_ptr : view_ptrs) {
        view_ptr -> draw();
    }
}

void Controller::go()
{
    Model::get_instance().update();
}

void Controller::build()
{
    string struct_name, struct_type;
    Point location = read_new_obj_info(struct_name, struct_type);
    shared_ptr<Structure> struct_ptr = create_structure(struct_name, struct_type, location);
    Model::get_instance().add_structure(struct_ptr);
}

void Controller::train()
{
    string agent_name, agent_type;
    Point location = read_new_obj_info(agent_name, agent_type);
    shared_ptr<Agent> agent_ptr = create_agent(agent_name, agent_type, location);
    Model::get_instance().add_agent(agent_ptr);
}

void Controller::move(shared_ptr<Agent> agent_ptr) const
{
    Point destination = read_Point();
    agent_ptr -> move_to(destination);
}

void Controller::work(shared_ptr<Agent> agent_ptr) const
{
    Model& model = Model::get_instance();
    string source_name, destination_name;
    cin >> source_name >> destination_name;
    shared_ptr<Structure> source_ptr = model.get_structure_ptr(source_name);
    shared_ptr<Structure> destination_ptr = model.get_structure_ptr(destination_name);
    agent_ptr -> start_working(source_ptr, destination_ptr);
}

void Controller::attack(shared_ptr<Agent> agent_ptr) const
{
    string target_name;
    cin >> target_name;
    shared_ptr<Agent> target_ptr = Model::get_instance().get_agent_ptr(target_name);
    agent_ptr -> start_attacking(target_ptr);
}

void Controller::stop(shared_ptr<Agent> agent_ptr) const
{
    agent_ptr -> stop();
}

void Controller::add_view_ptr(shared_ptr<View> view_ptr)
{
    view_ptrs.push_back(view_ptr);
    Model::get_instance().attach(view_ptr);
}

void Controller::remove_view_ptr(shared_ptr<View> view_ptr)
{
    // find the view_ptr inside the sequential container and delete it
    auto it = find(view_ptrs.begin(), view_ptrs.end(), view_ptr);
    assert(it != view_ptrs.end());
    view_ptrs.erase(it);
    Model::get_instance().detach(view_ptr);
}

// Read the information of the to-be-created object and return its name, type and location.
static Point read_new_obj_info(string& name, string& type)
{
    cin >> name;
    check_name_validity(name);
    cin >> type;
    return read_Point();
}

// Check if the name is at least num_initial_letters characters long
// if it contains letters that is not alnum, and if it is already in use.
// Throw an exception if the name is invalid.
static void check_name_validity(const string& name)
{
    if(name.length() < num_letters_c) {
        throw Error(invalid_name_c);
    }
    if(any_of(name.begin(), name.end(), [](char c){return !isalnum(c);})) {
        // throw exception if any of the character is not alnum
        throw Error(invalid_name_c);
    }
    if(Model::get_instance().is_name_in_use(name)) {
        throw Error(invalid_name_c);
    }
}

// Read from the input two double values and return a Point
// whose x- and y- coordinates are the two doubles read.
static Point read_Point()
{
    double x_coord = read_double(), y_coord = read_double();
    return Point(x_coord, y_coord);
}

// Read a double from the input stream and return it if read is successful.
// Throw an exception if a double cannot be read.
static double read_double()
{
    double number;
    if(!(cin >> number)) {
        cin.clear();
        throw Error(invalid_double_c);
    }
    return number;
}

// Read an integer from the input stream and return it if read is successful.
// Throw an exception if an integer cannot be read.
static int read_int()
{
    int number;
    if(!(cin >> number)) {
        cin.clear();
        throw Error(invalid_int_c);
    }
    return number;
}

// Print the error message contained in the Error object
// and clean up the wrong line in the input.
static void print_error_msg_and_clear_input(const Error& error)
{
    cout << error.msg << endl;
    while(cin.get() != '\n');
}
