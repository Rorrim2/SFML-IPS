#pragma once
#include <unordered_map>
#include <vector>

//TODO comments
using Bindings = std::unordered_map<std::string, Binding*>; // unorder map of 
using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding {
	Binding(const std::string& name):{}

	Events
	EventDetails details;
	std::string nameOfEvent
	int c; //count of events that are active in that moment 
};

class EventManager
{
public:
	EventManager();
	~EventManager();

	bool AddBinding();

private:
	void LoadBindings();
	bool hasFocus;
	Bindings bindings;
	Callbacks callbacks;
};

