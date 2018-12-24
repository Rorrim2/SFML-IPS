#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

class MapClientSide
{
	std::string mapName;
	sf::Texture map;
	sf::Sprite mapSprite;
public:
	void loadFromFile();
	void setMapName(string name);
	sf::Texture & getMapTexture();
	sf::Sprite & getMapSprite();
};

