#pragma once
#include <unordered_map>
#include "ServerPlayerManager.h"
#include "Server.h"
#include "Window.h"
#include <SFML/Graphics.hpp>


class ServerLogic
{
public:
   ServerLogic(bool windowEnable);
   ~ServerLogic();

   void initPsyhicsWorld();
   void initDebugDraw(Window *window);
   void debugDraw();
   void updatePsyhicsWorld();

   sf::Packet getPlayersSnapshot();
   void addPlayer(ClientID& clientID, const float &x, const float &y);
   void movePlayer(ClientID& clientID, MoveDirection dir);
   void signToRemovePlayer(const ClientID& clientID);
   void clearBodies();
   void clientLeft(const ClientID& clientID);
   
   void commandHandler(Server *server);
   void handler(sf::IpAddress &ip, const PortNumber &port, const PacketID &packetID, sf::Packet &packet, Server *server);

   void runServer();
private: 
   sf::Thread commandThread;

   const bool windowEnable;
   Window *window;
   Server server;
   ServerPlayersManager playersManager;
   World world;
};

 
