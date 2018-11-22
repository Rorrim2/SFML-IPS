#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include "Client.h"
#include "ClientPlayersManager.h"


class GameState : public BaseState
{
public:
   GameState(StateManager *stateManager);
   ~GameState();

   void onCreate() override;
   void onDestroy() override;

   void draw() override;
   void update(const sf::Time &time) override;

   void activate() override;
   void deactivate() override;

   void setServer(const sf::IpAddress & ip, const PortNumber & portNumber);
   bool connect();
   

private:
   ClientPlayersManager playersManager;
   ClientPlayer* player;
   Client client;
};

