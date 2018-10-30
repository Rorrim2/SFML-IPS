#pragma once
#include <unordered_map>
#include <ServerPlayerManager.h>
#include <SFML/Graphics.hpp>

class ServerLogic
{
public:
   ServerLogic();
   ~ServerLogic();

   void initPsyhicsWorld();
   void initDebugDraw(Window *window);
   void debugDraw();
   void updatePsyhicsWorld();

   std::vector<sf::Packet> getPlayersPos();
   void addPlayer(ClientID& clientID, const float &x, const float &y);
   void movePlayer(ClientID& clientID, MoveDirection dir);
   void signToRemovePlayer(const ClientID& clientID);
   void clearBodies();

   ServerPlayersManager playersManager;
private: 
   World world;
};

