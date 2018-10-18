#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Window
{
public:
   Window();
	Window(const std::string &title, const sf::Vector2u &size);
	~Window();

   void close();

   void beginDraw();
   void endDraw();

   void update();

   bool isDone();
   bool isFullscreen();
   void toggleFullscreen();

   sf::Vector2u getWindowSize();

   void draw(sf::Drawable &drawable);



private:
   void create();
   void destroy();

   sf::RenderWindow window;
   sf::Vector2u windowSize;
   std::string windowTitle;
   bool done;
   bool fullscreen;

};

