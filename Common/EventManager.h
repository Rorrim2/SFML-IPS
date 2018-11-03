#pragma once
#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <vector>
#include <functional>

//TODO comments
using Bindings = std::unordered_map<std::string, Binding*>; // create unorder map of 
using Events = std::vector<std::pair<EventType, EventInfo>>; // create vector of information about an event type and code for the key 


// types of events that could appear, if necessary -> add some stuff :)
enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

//just code for the key, all about event info
//not necessary at all, some events can work with only eventType 
struct EventInfo {
	EventInfo() { code = 0; }
	EventInfo(int eventNum) { code = eventNum; }
	union {
		int code; // why union? for some expansions in the future 
	};
};


//some details about event type and parameters
struct EventDetails {
	EventDetails(std::string & bindName) : name(bindName) { Clear(); }

	std::string name;
	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode;

	void Clear() {
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		mouse = sf::Vector2i(0, 0);
		mouseWheelDelta = 0;
		keyCode = -1;
	}
};

struct Binding {
	Binding(const std::string& name): nameOfEvent(name), details(name), c(0) {}
	void BindEvent(EventType type, EventInfo info = EventInfo()) {
		events.emplace_back(type, info);
	}

	Events events;
	EventDetails details;
	std::string nameOfEvent;
	int c; //count of events that are active in that moment 
};

template <class T> //used in struct Callback to be universal for any class 
using Function = void (T::*)(); //not sure for what now

struct Callback {
	std::string nameOfCallback;
	T* CallBackInstance; //Pointer to instance of class 
	void(T::*callback)(); //Function pointer 
	void Call() {
		CallBackInstance->*callback(); // universal function which run function pointer from any class
	}
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

