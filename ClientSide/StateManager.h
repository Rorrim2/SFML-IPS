#pragma once
#include "SFML/System/Time.hpp"
#include "Window.h"
#include "BaseState.h"
#include "EventManager.h"
#include "StateType.h"
#include <unordered_map>
#include <functional>
#include <vector>

//container for some classes and devices
struct SharedContext
{
	SharedContext() : window(nullptr), eventManager(nullptr) {}
	Window *window;
	EventManager *eventManager;
};

using StateContainer = std::vector<std::pair<StateTypeE, BaseState*>>;
using StateFactory = std::unordered_map<StateTypeE, std::function<BaseState*(void)>>;
using TypeContainter = std::vector<StateTypeE>;



class StateManager
{
public:
	StateManager(SharedContext *sharedContext);
	~StateManager();

	void draw();
   void update(const sf::Time &time);

   void processRequest();

   SharedContext *getContext();

   void switchTo(const StateTypeE &type);
   void remove(const StateTypeE &type);

private:
   void createState(const StateTypeE &type);
   void removeState(const StateTypeE &type);

   template<class T>
   void registerState(const StateTypeE &type)
   {
      stateFactory[type] = [this]() -> BaseState*
      {
         return new T(this);
      };
   }

   SharedContext *context;
   StateContainer states;
   TypeContainter toRemove;
   StateFactory stateFactory;
};

