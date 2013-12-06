#include "List_view.h"
#include <iostream>

using std::cout; using std::endl;
using std::make_pair;
using std::string;
using std::vector;

void List_view::update_remove(const string& name)
{
    object_info.erase(name);
}

void List_view::update_info(const string& name, double value)
{
    auto result = object_info.insert(make_pair(name, value));
    if(!result.second) {
        // If the name already exists, update the value on record.
        (result.first) -> second = value;
    }
}

void List_view::plot(const string& name)
{
    cout << "Current " << name << ":" << endl;
    cout << "--------------" << endl;
    for(auto it = object_info.begin(); it != object_info.end(); ++it) {
        cout << it -> first << ": " << it -> second << endl;
    }
    cout << "--------------" << endl;
}

void List_view::plot_selected(const string& name, const vector<string>& objects) const
{
    cout << name << endl;
    cout << "--------------" << endl;
    for(const string& name : objects) {
        auto it = object_info.find(name);
        if(it != object_info.end()) {
            cout << it -> first << ": " << it -> second << endl;
        }
    }
    cout << "--------------" << endl;
}
