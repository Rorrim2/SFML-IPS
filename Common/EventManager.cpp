#include "EventManager.h"

EventManager::EventManager() : hasFocus(true) { LoadBindings(); } // sets hasFocus on true and loads bindings from file

EventManager::~EventManager() {
	for (auto &itr : bindings) {
		delete itr.second;
		itr.second = nullptr;
	}
}

//checks if there is any binding of the event with the same name in "Binding map", if there isn't, it adds one
bool EventManager::AddBinding(Binding *binding) {
	if (bindings.find(binding->nameOfEvent) != bindings.end()) return false;
	return bindings.emplace(binding->nameOfEvent, binding).second;
}

//if there is binding with the same name in "Binding map", free memory by deleting the second element
// and then throw out from "Binding map"
bool EventManager::RemoveBinding(std::string nameOfEvent) {
	auto itr = bindings.find(nameOfEvent);
	if (itr == bindings.end()) return false;
	
	delete itr->second;
	bindings.erase(itr);
	return true;
}

//checks if SFML event (TypeEvent) matches the type of binding event that's currently being processed
//if it does, we check if it's mouse or keyboard event
//last part is checking key code/mouse button code
void EventManager::HandleEvent(sf::Event &TypeEvent) {
	for (auto &bindItr : bindings) {
		Binding* bind = bindItr.second;
		for (auto &eventItr : bind->events)
		{
			EventType sfmlEvent = (EventType)TypeEvent.type; //casting event type from sfml to var "EventType"

			if (eventItr.first != sfmlEvent) { continue; } //if Events.EventType != sfmlEvent (type) -> continue
 			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp)
			{
				if (eventItr.second.code == TypeEvent.key.code)  //if eventInfo.code == typeevent.key.code (keyboard) ->
				{
					if (bind->details.keyCode != -1)  // -> if eventdetails.keycode != -1
					{
						bind->details.keyCode = eventItr.second.code; // binding->eventdetailt->keycode = Events->EventInfo->code
					}
					++(bind->c); // increase number of active events
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp)
			{
				if (eventItr.second.code == TypeEvent.mouseButton.button)  //if eventInfo.code == typeevent.mouseButton.code (mouse)
				{
					bind->details.mouse.x = TypeEvent.mouseButton.x;
					bind->details.mouse.y = TypeEvent.mouseButton.y;
					if (bind->details.keyCode != -1)
					{
						bind->details.keyCode = eventItr.second.code;
					}
					++(bind->c);
					break;
				}
			}
			else
			{
				if (sfmlEvent == EventType::MouseWheel)
				{
					bind->details.mouseWheelDelta = TypeEvent.mouseWheel.delta;
				}
				else if (sfmlEvent == EventType::WindowResized)
				{
					bind->details.size.x = TypeEvent.size.width;
					bind->details.size.y = TypeEvent.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered)
				{
					bind->details.textEntered = TypeEvent.text.unicode;
				}
				++(bind->c);
			}
		}
	}
}

//checks input in real-time, validates and resets states of the bindings
void EventManager::Update() {
	if (!hasFocus) return;
	for (auto &bindItr : bindings) {
		Binding* bind = bindItr.second;
		for (auto &eventItr : bind->events) {
			switch (eventItr.first) { //eventType
				case (EventType::Keyboard):
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(eventItr.second.code))) { // checks code of the pressed key
						if (bind->details.keyCode != -1) { 
							bind->details.keyCode = eventItr.second.code; //eventdetails. keycode = eventinfo.code
						}
						++(bind->c);
					}
					break;
				case (EventType::Mouse):
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button(eventItr.second.code))) {
						if (bind->details.keyCode != -1) {
							bind->details.keyCode = eventItr.second.code;
						}
						++(bind->c);
					}
					break;
				case (EventType::Joystick):
					//Some expansion in the future
					break;

			}
		}
		if (bind->events.size() == bind->c) {	//	checks if the number of events in the event
												//	container matches the number of events that are "on"
			auto callItr = callbacks.find(bind->nameOfEvent);
			if (callItr != callbacks.end()) {
				callItr->second(&bind->details); // locate callback(EventDetails*) in callbacks container 
												// its an std::function methods
			}
		}
		// clear all stuff
		bind->c = 0;
		bind->details.Clear();
	}
}


// loads events and their parameters from a file
// Window_close 0:0
// nameOfEvent key:value
void EventManager::LoadBindings() {
	std::string delimeter = ":";

	std::ifstream fileBindings;
	fileBindings.open("keys.txt"); // path of file, where it should be?
	if (!fileBindings.is_open()) {
		std::cout << "Failed to load file keys.txt" << std::endl;
		return;
	}

	std::string line;
	while (std::getline(fileBindings, line)) {
		std::stringstream keystream(line); // constructor of stringstream
		std::string callbackName;
		keystream >> callbackName; // takes info to the first space
		Binding* bind = new Binding(callbackName);
		while (!keystream.eof()) {
			std::string keyval;
			keystream >> keyval; // key:value
			int start = 0;
			int end = keyval.find(delimeter);
			if (end == std::string::npos) { // if "end" is not in string 
				delete bind;
				bind = nullptr;
				break;
			}
			EventType type = EventType(stoi(keyval.substr(start, end - start))); // type == key
			int code = stoi(keyval.substr(end + delimeter.length(), keyval.find(delimeter, end + delimeter.length()))); // code == value
			EventInfo eventInfo;
			eventInfo.code = code;

			bind->BindEvent(type, eventInfo); //bind event info
		}
		if (!AddBinding(bind)) { delete bind; }
		bind = nullptr;
	}
	fileBindings.close();
}