#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "MapClientSide.h"

using namespace std;

void MapClientSide::loadFromFile()
{
	if (!this->map.loadFromFile(this->mapName))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
	}
	this->mapSprite.setTexture(this->map);
}

void MapClientSide::setMapName(string name)
{
	this->mapName = name;
}

sf::Texture & MapClientSide::getMapTexture()
{
	return this->map;
}

sf::Sprite & MapClientSide::getMapSprite()
{
	return this->mapSprite;
}
