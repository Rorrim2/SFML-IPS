#pragma once
#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>
#include <vector>
#include <functional>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include "StateType.h"

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

//code for the key
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
	EventDetails(const std::string & bindName) : name(bindName) { Clear(); }

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

using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>; // callback container
enum StateTypeE;
using Callbacks = std::unordered_map<StateTypeE, CallbackContainer>; //map of states and container
using Events = std::vector<std::pair<EventType, EventInfo>>; // create vector of information about an event type and code for the key 

//it binds events 
//it can operate combination of button (basically: one button for event)
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

using Bindings = std::unordered_map<std::string, Binding*>; // create unorder map of bindings

template <class T> //used in struct Callback to be universal for any class 
struct Callback {
	std::string nameOfCallback;
	T* CallBackInstance; //Pointer to instance of class 
	void(T::*callback)(); //Function pointer 
	void Call() {
		CallBackInstance->*callback(); // universal function which run function pointer from any class
	}
};

//main class
class EventManager
{
public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding *binding);
	bool RemoveBinding(std::string nameOfEvent);

	void SetFocus(const bool &focus) { this->hasFocus = focus; }
	void setCurrentState(const StateTypeE &curState) { this->currentState = curState; }
	StateTypeE getCurrentState() { return this->currentState; }

	template<class T>
	bool AddCallback(StateTypeE state, const std::string &name, void (T::*func)(EventDetails*), T* instance) {
		auto itr = callbacks.emplace(state, CallbackContainer()).first; //make new callbackcontainer
		auto temp = std::bind(func, instance, std::placeholders::_1);	// functor which take function pointer, instance of any 
																		//class and first parameter of function pointer -> EventDetails
		return itr->second.emplace(name, temp).second;	//if there is no element with the key in containter
														//it puts new one to it
	}												
		
	bool RemoveCallback(StateTypeE state, const std::string &name) {
		auto itr = callbacks.find(state);
		if (itr == callbacks.end()) { return false; }
		auto itr2 = itr->second.find(name);
		if (itr2 == itr->second.end()) { return false; }
		itr->second.erase(name);	// throw out an element by the key
		return true;
	}
	
	void HandleEvent(sf::Event &TypeEvent);
	void Update();

	sf::Vector2i GetMousePos(sf::RenderWindow* wind = nullptr) {
		return (wind ? sf::Mouse::getPosition(*wind) : sf::Mouse::getPosition());	//gives position of the mouse 
																					//if its relative to special window or not
	}

private:
	void LoadBindings();

	bool hasFocus;	// contains info about relation between Mouse and Window (focused -> true, not focused -> false)
	Bindings bindings; 
	Callbacks callbacks;
	StateTypeE currentState;
};

