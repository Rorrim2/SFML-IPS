#include "Window.h"

Window::Window()
{
   Window("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string &title, const sf::Vector2u &size)
   : windowTitle(title), windowSize(size), fullscreen(false), done(false)
{
   create();
   setup(title, size);
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

void Window::close(EventDetails* details)
{
   this->done = true;
}


void Window::setup(const std::string title, const sf::Vector2u& size) {
	this->windowTitle = title;
	this->windowSize = size;
	this->fullscreen = false;
	this->done = false;
	this->isFocused = true;
	this->eventManager.AddCallback(eventManager.getCurrentState() ,"Fullscreen_toggle", &Window::toggleFullscreen, this);
	this->eventManager.AddCallback(eventManager.getCurrentState(), "Window_close", &Window::close, this);
	this->create();
}

void Window::beginDraw()
{
   this->window.clear(sf::Color::Black);
}

void Window::endDraw()
{
   this->window.display();
}

//expanded by things from EventManager
void Window::update()
{
   sf::Event event;
   while (this->window.pollEvent(event))
   {
	  if (event.type == sf::Event::LostFocus) {
		  this->isFocused = false;
		  this->eventManager.SetFocus(false);
	  }
	  else if (event.type == sf::Event::GainedFocus) {
		  this->isFocused = true;
		  this->eventManager.SetFocus(true);
	  }
	  if(event.key.code == sf::Keyboard::F5 || event.key.code == sf::Keyboard::Escape) std::cout << event.type << std::endl;
	  this->eventManager.HandleEvent(event);
   }
   this->eventManager.Update();
}

bool Window::isDone()
{
   return this->done;
}

bool Window::isFullscreen()
{
   return this->fullscreen;
}

void Window::toggleFullscreen(EventDetails* details)
{
   this->fullscreen = !this->fullscreen;
   destroy();
   create();
}

sf::Vector2u Window::getWindowSize()
{
   return sf::Vector2u();
}

EventManager* Window::getEventManager() {
	return &this->eventManager;
}

sf::RenderWindow* Window::getRenderWindow() {
	return &this->window;
}

void Window::draw(sf::Drawable & drawable)
{
   this->window.draw(drawable);
}

