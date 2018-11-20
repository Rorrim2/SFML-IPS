#pragma once
#include <SFML/System/Time.hpp>


class StateManager;

class BaseState
{
   friend class StateManager;

public:
	BaseState(StateManager *stateManager);
	virtual ~BaseState();

	virtual void onCreate() = 0; //state created and pushed on the stack
	virtual void onDestroy() = 0; //state removed from the stack

	virtual void draw() = 0;
	virtual void update(const sf::Time &time) = 0; 

	virtual void activate() = 0; //state move to the top of the stack
   virtual void deactivate() = 0; //state removed
	
	void setTransparent(bool enable); 
	void setTransdencent(bool enable);
	bool getTransparent(); 
	bool getTransdencent();

protected:
	StateManager *stateManager;
	//These flags indicate if this state also needs to render or update a state that came before it.
	bool transparent; 
	bool transdencent; 
};

