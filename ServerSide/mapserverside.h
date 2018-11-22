#ifndef MAPSERVERSIDE_H
#define MAPSERVERSIDE_H


#include <SFML-2.5.0/include/SFML/System.hpp>
#include <SFML-2.5.0/include/SFML/Graphics.hpp>
#include <tinyxml.h>
#include <iostream>
#include <vector>

using namespace std;
//#include <SFML/System.hpp>
//#include <SFML/Graphics.hpp>

struct Package {
    vector<vector<vector<int>>> dataVector; //id of tiles
    int tileWidth;
    int tileHeight;
    std::string source;
};

enum OBJECTTYPE { //Is this even necessary
    RECT = 0,
    POLYGON
};

enum BODYTYPE {
    STATIC = 0,
    DYNAMIC,
    SOMETHING_WRONG
};

struct Pointf {
    float x;
    float y;
};

struct Rectangle {
    float x;
    float y;
    float width;
    float height;
    int player_id;
    BODYTYPE type;
};

struct Polygon {
    std::vector <Pointf> points;
    float x;
    float y;
    BODYTYPE type;
};

class MapServerSide
{
    int width, height, tileWidth, tileHeight; //podstawowe informacje o mapie
    Package pack; //paczka do wysłania do clientside
    OBJECTTYPE enum_object; //w sumie nie musi być tutaj, to tylko zmienna tymczasowa
    std::vector<Rectangle> Rectangles; //vector prostokątóW
    std::vector<Polygon> Polygons; //vector polygonów
public:
	MapServerSide() {};
    bool loadFromFile(std::string name); //this func does almost everything
    Package getPackage(void);
    Rectangle getPlayerRect(int id, int* err_code);
};

#endif // MAPSERVERSIDE_H
