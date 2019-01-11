#include "Window.h"

Window::Window()
{
   Window("Window", sf::Vector2u(640, 480));
   this->isSetDataState = false;
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
   sf::ContextSettings settings;
   settings.antialiasingLevel = 2;
   this->window.create({ this->windowSize.x, this->windowSize.y, 32 }, this->windowTitle, style, settings);
   this->window.setVerticalSyncEnabled(true);
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
	this->eventManager.AddCallback(StateTypeE(0),"Fullscreen_toggle", &Window::toggleFullscreen, this);
	this->eventManager.AddCallback(StateTypeE(0), "Window_close", &Window::close, this);
	this->create();
	this->counter = 0;
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
	  else if (event.type == sf::Event::TextEntered) {
		  if (this->isSetDataState) {
				if (event.text.unicode < 127 && event.text.unicode >= 32)
					{
						this->Inputs[this->counter].push_back((char)event.text.unicode);
					}
			}
			  
	  }
	 // if(event.key.code == sf::Keyboard::F5 || event.key.code == sf::Keyboard::Escape) std::cout << event.type << std::endl;
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
//setdatastate
void Window::SetIsDataStateFlag()
{
	this->isSetDataState = !this->isSetDataState;
}

bool Window::GetIsDataStateFlag() 
{
	return this->isSetDataState;
}

unsigned int Window::getCounter()
{
	return this->counter;
}

std::string Window::getInput()
{
	return this->Inputs[this->counter];
}

void Window::incrementCounter()
{
	this->counter++;
}

void Window::deleteChar()
{
	this->Inputs[this->counter].pop_back();
}
//end things with setdatastate


sf::Vector2u Window::getWindowSize()
{
   return sf::Vector2u(this->windowSize);
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

void Window::draw(sf::Vertex *verticles, size_t count, sf::PrimitiveType type)
{
   this->window.draw(verticles, count, type);
}

