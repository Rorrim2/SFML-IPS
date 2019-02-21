#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include "Client.h"
#include "ClientPlayersManager.h"
#include "ClientCannonballManager.h"
#include <queue>
#include "MapClientSide.h"
#include "World.h"


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
   
   void moveToMainMenu(EventDetails* details);
   void clientHandler(const PacketID &id, sf::Packet &packet, Client *client);
private:
  
   void shoot(EventDetails *details);
   void movePlayer(EventDetails *details);

   bool physicStarted;
   sf::Time timer;
   float sendEventToServerTimer;
   World world;
   std::queue<std::pair<MoveDirection, sf::Int32>> lastDirections;
   ClientPlayersManager playersManager;
   ClientCannonballManager cannonBallManager;
   ClientPlayer* player;
   Client client;
   TextureManager textureManager;
   sf::Sprite *hpBar;
   short hp = 3;

   bool debug = false;

   MapClientSide map;
};

