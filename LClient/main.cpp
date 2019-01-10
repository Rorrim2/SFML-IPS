#include "Client.h"
#include "Game.h"


int main(int argc, char** argv)
{
   Game game("Game", StateTypeE::GAME);
   game.run();

   //sf::Thread c(&commandProcess, &c
   std::cout << "Quitting..." << std::endl;
   sf::sleep(sf::seconds(1.f));


   return 0;
}