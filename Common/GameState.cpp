#include "GameState.h"

void clientHandler(const PacketID &id, sf::Packet &packet, Client *client);

GameState::GameState(StateManager *stateManager)
   :BaseState(stateManager), client(playersManager)
{
}


GameState::~GameState()
{
}

void GameState::onCreate()
{
   sf::IpAddress ip("localhost");
   PortNumber port = 5600;
   std::cout << "Enter Server IP: ";
   std::cin >> ip;
   std::cout << "Enter Server Port: ";
   std::cin >> port;
   setServer(ip, port);
   this->client.setup(clientHandler);
   connect();
}

void GameState::onDestroy()
{
   DELLISNOTNULL(this->player);
   this->client.disconnect();
}

void GameState::draw()
{
   this->playersManager.drawAllPlayers(*this->stateManager->getContext()->window);
}

void GameState::update(const sf::Time & time)
{
   this->playersManager.updateAllPlayers(time);
}

void GameState::activate()
{
}

void GameState::deactivate()
{
   this->client.disconnect();
}

void GameState::setServer(const sf::IpAddress & ip, const PortNumber & portNumber)
{
   this->client.setServer(ip, portNumber);
}

void updateConnection(Client *client)
{
   if (client->connect())
   {
      sf::Clock clock;
      clock.restart();
      while (client->isConnected())
      {
         client->update(clock.restart());
      }
   }
}
bool GameState::connect()
{
   sf::Thread connection(updateConnection, &this->client);
   bool rV = this->client.connect();

   if (rV == true)
   {
      this->client.sendCreatePlayerPacket();
    /*  this->player = new ClientPlayer(100, 40);
      this->playersManager.addPlayer(this->client.getClientID(), this->player);*/
      connection.launch();
   }
   return rV;
}  




void clientHandler(const PacketID &id, sf::Packet &packet, Client *client)
{
   if (static_cast<PacketType>(id) == PacketType::Message)
   {
      std::string message;
      packet >> message;
      std::cout << message << std::endl;
   }
   else if (static_cast<PacketType>(id) == PacketType::Disconnect)
   {
      client->disconnect();
      DEBUG_COUT("Disconnected!");
   }
   else if (static_cast<PacketType>(id) == PacketType::PlayerCreate)
   {
      float x, y;
      ClientID idC;
      packet >> idC >> x >> y;
      client->playersManager.addPlayer(idC, x, y);
   }
   else if (static_cast<PacketType>(id) == PacketType::PlayerUpdate)
   {
      float x, y, angle;
      ClientID idC;
      packet >> idC >> x >> y >> angle;
      client->playersManager.movePlayer(idC, x, y, angle);
   }
}