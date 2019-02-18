#pragma once
#include <unordered_map>
#include "ServerPlayerManager.h"
#include "Server.h"
#include "Window.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include "ServerCannonballManager.h"


class ServerLogic
{
public:
   ServerLogic(bool windowEnable);
   ~ServerLogic();

   void addObjects();

   void initPsyhicsWorld();
   void initDebugDraw(Window *window);
   void debugDraw();
   void updatePsyhicsWorld();

   sf::Packet getPlayersSnapshot();
   sf::Packet getCannonBallsSnapshot();
   void addPlayer(ClientID& clientID, const float &x, const float &y, ShipType shipType);
   void movePlayer(ClientID& clientID, MoveDirection dir, const sf::Int32 & time);
   void signToRemovePlayer(const ClientID& clientID);
   void clearBodies();
   void clientLeft(const ClientID& clientID);
   
   void commandHandler(Server *server);
   void handler(sf::IpAddress &ip, const PortNumber &port, const PacketID &packetID, sf::Packet &packet, Server *server);

   void runServer();
private: 
   sf::Thread commandThread;
   sf::Mutex bodyCreate;
   const bool windowEnable;
   Window *window;
   Server server;
   ServerPlayersManager playersManager;
   ServerCannonballManager cannonballMg;
   World world;
};

 
