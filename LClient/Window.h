#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "EventManager.h"

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

};

