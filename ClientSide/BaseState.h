#pragma once
#include <SFML/System/Time.hpp>


class StateManager;

class BaseState
{
   friend class StateManager;

public:
	BaseState(StateManager *stateManager);
	virtual ~BaseState();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void draw() = 0;
	virtual void update(const sf::Time &time) = 0;

	virtual void activate() = 0;
   virtual void deactivate() = 0;
	
	void setTransparent(bool enable);
	void setTransdencent(bool enable);
	bool getTransparent();
	bool getTransdencent();

protected:
	StateManager *stateManager;
	bool transparent;
	bool transdencent;
};

