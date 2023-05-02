#include "Game.h"

int main(int argc,char *argv[])
{  
    SDL2::Game_Singleton game;

    game.getInstance()->init("my-rpg",260,260,640,320);
    game.getInstance()->render();
    game.getInstance()->handleEvents();
    game.getInstance()->clean();

    game.clean();
    return 0;
}