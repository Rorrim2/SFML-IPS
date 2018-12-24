#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "MapParser.h"

using namespace std;

bool MapServerSide::loadFromFile(std::string filename, std::string tilesetImagePath) {
    //ładowanie mapy
    TiXmlDocument MapFile(filename.c_str());
    if (!MapFile.LoadFile())
    {
        cout << "Loading level \"" << filename << "\" failed." << endl;
        return false;
    }
    //główny element mapy
    TiXmlElement *map = MapFile.FirstChildElement("map");

    //setting up map properties
    width = atoi(map->Attribute("width"));
    height = atoi(map->Attribute("height"));
    tileWidth = atoi(map->Attribute("tilewidth"));
    tileHeight = atoi(map->Attribute("tileheight"));

    inCaseItWouldBecomeMoreComplicated.tileHeight = tileHeight;
    inCaseItWouldBecomeMoreComplicated.tileWidth = tileWidth;

    TiXmlElement *tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");

    //ścieżka do tilesetu - to też chyba trzeba wysłać

    pack.name = tilesetImagePath; //wpisanie do paczki

    TiXmlElement *layerElement;
    layerElement = map->FirstChildElement("layer");
    int numberOfLayer = 0; //nie wiem po co mi to było

    //layers
    while(layerElement)
    {
        TiXmlElement *layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data"); //przypisanie danych liczbowych
        if (layerDataElement == nullptr)
        {
            cout << "Bad map. No layer information found." << endl;
            return false;
        }
        //I assume in here that coding is in csv - jeśli byłoby w innym to jest źle

        std::string dataString = layerDataElement->GetText(); //przerobienie danych na string
        std::stringstream datastream(dataString);

        unsigned long int i;
        vector<vector<unsigned long int>> tempVectorRow;
        //przerzucenie danych do tablicy dwuwymiarowej (+ rzutowanie na int)
        for(int y = 0; y < height; ++y)
        {
            vector<unsigned long int> tempVector;
            for(int x = 0; x < width; ++x)
            {
                datastream >> i;
                tempVector.push_back(i);
                //pack.dataVector[numberOfLayer][y].push_back(i);
                if(datastream.peek() == ',')
                    datastream.ignore();
            }
            tempVectorRow.push_back(tempVector);

        }
        inCaseItWouldBecomeMoreComplicated.dataVector.push_back(tempVectorRow); //wrzucenie do vectora wyjściowego
        numberOfLayer++; //nadal nie wiem po co to
        layerElement = layerElement->NextSiblingElement("layer"); //przejście do następnej warstwy
    }

    //objects
    //najpierw mamy objecktgroup - każda ma w sobie przynajmniej jednego objecta i każdy ma w sobie properties które mają przynajmniej jeden property
    //reszta różni się zależnie od kształtu objecta
    int player_number = 1;
    TiXmlElement *objectGroupElement;
    objectGroupElement = map->FirstChildElement("objectgroup");
    while (objectGroupElement)//Check that there is atleast one object layer
    {
        std::string name = objectGroupElement->Attribute("name"); //ships, islands, bounds - na razie nigdzie nie używamy tego
        TiXmlElement *objectElement;
        objectElement = objectGroupElement->FirstChildElement("object"); //pojedyńczy object np. jedna wyspa
        while (objectElement)
        {
            std::string temp_type = objectElement->Attribute("type"); //rect czy polygon
            if(temp_type == "reck")
            {
                Rectangle temp_rect;
                temp_rect.x = atof(objectElement->Attribute("x"));
                temp_rect.y = atof(objectElement->Attribute("y"));
                temp_rect.width = atof(objectElement->Attribute("width"));
                temp_rect.height = atof(objectElement->Attribute("height"));
                TiXmlElement* mapProperties;
                mapProperties = objectElement->FirstChildElement("properties"); //w tym są wszytkie property
                if(mapProperties == nullptr) {
                    temp_rect.type = SOMETHING_WRONG; //wszędzie dodaję informację czy ciało jest statyczne czy dynamiczne, jak tego nie ma to coś jest źle
                    Rectangles.push_back(temp_rect); //vector w którym są wszystkie obiekty prostokątne
                    continue;
                }
                TiXmlElement* objectProperty;
                objectProperty = mapProperties->FirstChildElement("property"); //pojedyńcza własność - na razie mamy tylko body_type
                while(objectProperty)
                {
                    std::string name_property = objectProperty->Attribute("name"); //nazwa własności
                    if(name_property == "body_type")
                    {
                        std::string body_type = objectProperty->Attribute("value");
                        if(body_type == "dynamic")
                        {
                            temp_rect.type = DYNAMIC; //ustawienie enuma
                            temp_rect.player_id = player_number;
                            player_number++;
                        }
                        else if(body_type == "static")
                        {
                            temp_rect.type = STATIC;
                            temp_rect.player_id = 0;
                        }
                    }
                    objectProperty = objectProperty->NextSiblingElement("property"); //przejście do następnej własności (jeśli istnieje)
                }
                Rectangles.push_back(temp_rect); //wrzucenie do vectora
            }
            else if(temp_type == "polygon")
            {
                Polygon temp_poly;
                temp_poly.x = atof(objectElement->Attribute("x"));
                temp_poly.y = atof(objectElement->Attribute("y"));
                //w polygonie najpierw są properties
                //properties
                TiXmlElement* mapProperties;
                mapProperties = objectElement->FirstChildElement("properties");
                if(mapProperties == nullptr) {
                    temp_poly.type = SOMETHING_WRONG; //tu nie możemy wrzucić jeszcze do vectora bo nie mamy punktów
                }
                TiXmlElement* objectProperty;
                objectProperty = mapProperties->FirstChildElement("property");
                while(objectProperty)
                {
                    std::string name_property = objectProperty->Attribute("name");
                    if(name_property == "body_type")
                    {
                        std::string body_type = objectProperty->Attribute("value");
                        if(body_type == "dynamic")
                        {
                            temp_poly.type = DYNAMIC;
                        }
                        else if(body_type == "static")
                        {
                            temp_poly.type = STATIC;
                        }
                    }
                    objectProperty = objectProperty->NextSiblingElement("property");
                }
                //w polygonie jest jeden string z współrzędnymi tworzących go punktów w formacie "x1,x2 y1,y2"
                //points
                TiXmlElement* polygon_object;
                polygon_object = objectElement->FirstChildElement("polygon");
                std::string array_of_points = polygon_object->Attribute("points");
                std::stringstream point_datastream(array_of_points);
                //wrzucenie do bufora, wysiąganie z niego i wrzucanie do vectora
                //najpierw wrzucam wszytskie jak leci do jednego vectora floatów
                //potem wpisuję je do tymczasowej struktury i wrzucam do vectora tych struktur
                //pewnie da się prościej
                //mówi się trudno
                float buffer;
                Pointf temp_pointf;
                std::vector<float> temp_vector_of_points;
                while(point_datastream >> buffer)
                {
                    temp_vector_of_points.push_back(buffer);
                    //pack.dataVector[numberOfLayer][y].push_back(buffer);
                    if(point_datastream.peek() == ',')
                        point_datastream.ignore();
                    else if(point_datastream.peek() == ' ')
                        point_datastream.ignore();

                }
                int bool_variable = 0;
                while(temp_vector_of_points.size() > 0) { //test it later
                    if(bool_variable = !bool_variable) {
                        temp_pointf.x = temp_vector_of_points.back();
                        temp_vector_of_points.pop_back();
                    }
                    else {
                        temp_pointf.y = temp_vector_of_points.back();
                        temp_vector_of_points.pop_back();
						temp_poly.points.push_back(temp_pointf);
                    }
					
                }
                
                Polygons.push_back(temp_poly);
            }
            objectElement = objectElement->NextSiblingElement("object");
        }
        objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
    }
    return true; //udało się wczytać mapę, trochę to kijowe, bo nawet jak jest zwracane true to nie wiadomo czy wszystko się udało
}

Package MapServerSide::getPackage(void) {
    return pack;
}

Rectangle MapServerSide::getPlayerRect(int id, int* err_code) {
    unsigned int i = 0;
    while(i < Rectangles.size()) {
        if(Rectangles[i].player_id == id) {
            *err_code = 1;
            return  Rectangles[i];
        }
        ++i;
    }
    *err_code = 0;
    return Rectangles[0];
}
