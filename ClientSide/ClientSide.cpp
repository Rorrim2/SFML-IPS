#include "Client.h"
#include "Game.h"
#include <iostream>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics.hpp>
#include "MapClientSide.h"

using namespace std;


void commandProcess(Client* client)
{
	while (client->isConnected())
	{
		std::string str;
		std::getline(std::cin, str);
		if (str != "")
		{
			if (str == "!quit")
			{
				client->disconnect();
				break;
			}
			sf::Packet p;
			StampPacket(PacketType::Message, p);
			p << str;
			client->sendPacket(p);
		}
	}
}

int main(int argc, char** argv)
{
 //  Game game("Game", StateTypeE::GAME);
 //  game.run();
 //  
 //  //sf::Thread c(&commandProcess, &c
	//std::cout << "Quitting..." << std::endl;
	//sf::sleep(sf::seconds(1.f));
	sf::RenderWindow window(sf::VideoMode(840, 840), "Just testing");
	string imagepath("C:\\Users\\aliss\\Documents\\SFML-IPS\\testing.png");
	MapClientSide map;
	map.setMapName(imagepath);
	map.loadFromFile();
	sf::Sprite tym = map.getMapSprite();
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// Close window button clicked.
				window.close();
			}
		}
		window.clear(sf::Color::Black);
		window.draw(tym);
		window.display();
	}
	return 0;
}