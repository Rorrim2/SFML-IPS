#include "StateManager.h"
#include "IntroState.h"
#include "MenuState.h"


StateManager::StateManager(SharedContext *sharedContext)
   : context( sharedContext)
{
   registerState<IntroState>(StateTypeE::INTRO);
   registerState<MenuState>(StateTypeE::MENU);
}


StateManager::~StateManager()
{
   for (auto &iter : this->states)
   {
      iter.second->onDestroy();
      delete iter.second;
   }
}

void StateManager::draw()
{
   if (this->states.empty()) return;

   if (this->states.back().second->getTransparent() == true && this->states.size() > 1)
   {
      auto iter = this->states.end();

      while (iter != this->states.begin())
      {
         if (iter != this->states.end())
         {
            if (iter->second->getTransparent() == false) break;
         }

         --iter;
      }

      for (; iter != this->states.end(); ++iter)
      {
         iter->second->draw();
      }
   }
   else//if we have only one state 
   {
      this->states.back().second->draw();
   }
}

void StateManager::update(const sf::Time &time)
{
   if (this->states.empty()) return;

   if (this->states.back().second->getTransdencent() == true && this->states.size() > 1)
   {
      auto iter = this->states.end();

      while (iter != this->states.begin())
      {
         if (iter != this->states.end())
         {
            if (iter->second->getTransdencent() == false) break;
         }

         --iter;
      }

      for (; iter != this->states.end(); ++iter)
      {
         iter->second->update(time);
      }
   }
   else//if we have only one state 
   {
      this->states.back().second->update(time);
   }
}

void StateManager::processRequest()
{
   while (this->toRemove.begin() != this->toRemove.end())
   {
      removeState(*this->toRemove.begin());
      this->toRemove.erase(this->toRemove.begin());
   }
}

SharedContext * StateManager::getContext()
{
   return this->context;
}

void StateManager::switchTo(const StateTypeE & type)
{
   bool found = false;
   this->context->eventManager->setCurrentState(type);
   for (auto iter = this->states.begin(); iter != this->states.end(); iter++)
   {
      if (iter->first == type)
      {
         this->states.back().second->deactivate();
         StateTypeE tempType = iter->first;
         BaseState* tempState = iter->second;
         this->states.erase(iter);
         this->states.emplace_back(tempType, tempState);
         tempState->activate();
         found = true;
         break;
      }
   }

   //if state is not found
   if (found == false)
   {
      createState(type);
      this->states.back().second->activate();
   }
   //this->context->
}

void StateManager::remove(const StateTypeE & type)
{
   this->toRemove.push_back(type);
}

void StateManager::createState(const StateTypeE & type)
{
   auto newState = this->stateFactory.find(type);
   if (newState == this->stateFactory.end()) return;
   BaseState *state = newState->second();
   this->states.emplace_back(type, state);
   state->onCreate();
}

void StateManager::removeState(const StateTypeE & type)
{
   for (auto iter = this->states.begin(); iter != this->states.end(); iter++)
   {
      if (iter->first == type)
      {
         iter->second->onDestroy();
         delete iter->second;
         this->states.erase(iter);
         break;
      }
   }
}

