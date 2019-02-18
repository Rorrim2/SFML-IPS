#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "EventManager.h"
#include "ShipType.h"

/*
enum InputType {
	IP, PORT, PLAYER, SHIP
};*/

class Window
{
public:
   Window();
	Window(const std::string &title, const sf::Vector2u &size);
	~Window();


   void beginDraw();
   void endDraw();

   void update();

   bool isDone();
   bool isFullscreen();


   //Event Manager
   bool getFocus() { return this->isFocused; }
   EventManager* getEventManager(); 
   void toggleFullscreen(EventDetails* details);
   void close(EventDetails* details = nullptr);
   void setup(const std::string title, const sf::Vector2u& size);

   // setdata state
   void SetIsDataStateFlag();
   bool GetIsDataStateFlag();
   std::string getInput();
   std::string getInput(unsigned int i);
   unsigned int getCounter();
   void incrementCounter();
   void deleteChar();
   void SetShip(ShipType shipType);
   ShipType GetShipType();
   void SwapTheSign();

   sf::RenderWindow* getRenderWindow();
   sf::Vector2u getWindowSize();

   void draw(sf::Drawable &drawable);
   void draw(sf::Vertex *verticles, size_t count, sf::PrimitiveType type);
   


private:
   void create();
   void destroy();

   sf::RenderWindow window;
   sf::Vector2u windowSize;
   std::string windowTitle;
   bool done;
   bool fullscreen;

   //EventManager
   EventManager eventManager;
   bool isFocused;

   //state: setdatastate
   std::map <unsigned int, std::string> Inputs;
   unsigned int counter;
   bool isSetDataState;
   std::map <ShipType, std::string> shipNames;
   unsigned int itsTimeToSwap;

   ShipType whichOne;
};

