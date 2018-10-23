#include "Window.h"

Window::Window()
{
   Window("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string &title, const sf::Vector2u &size)
   : windowTitle(title), windowSize(size), fullscreen(false), done(false)
{
   create();
}

void Window::create()
{
   auto style = this->isFullscreen() ? sf::Style::Fullscreen : sf::Style::Default;
   this->window.create({ this->windowSize.x, this->windowSize.y, 32 }, this->windowTitle, style);
}

void Window::destroy()
{
   this->window.close();
}


Window::~Window()
{
   destroy();
}

void Window::close()
{
   this->done = true;
}

void Window::beginDraw()
{
   this->window.clear(sf::Color::Black);
}

void Window::endDraw()
{
   this->window.display();
}

void Window::update()
{
   sf::Event event;
   while (this->window.pollEvent(event))
   {
      if (event.type == sf::Event::Closed)
      {
         this->done = true;
      }
      else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
      {
         toggleFullscreen();
      }
   }
}

bool Window::isDone()
{
   return this->done;
}

bool Window::isFullscreen()
{
   return this->fullscreen;
}

void Window::toggleFullscreen()
{
   this->fullscreen != this->fullscreen;
   destroy();
   create();
}

sf::Vector2u Window::getWindowSize()
{
   return sf::Vector2u();
}

void Window::draw(sf::Drawable & drawable)
{
   this->window.draw(drawable);
}

