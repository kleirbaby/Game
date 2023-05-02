#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "TextureManager.h"
#include "InputHandler.h"
#include "../common/Singleton.h"

namespace SDL2
{
class Sprite;
class GameStateMachine;
class Game:public InputHandler
{
public:
    Game();
    ~Game();

    bool init(std::string title,int x,int y,int w,int h,int flags = SDL_WINDOW_SHOWN);
    void render();
    void update();
    void draw();
    void handleEvents();
    void clean();

    inline SDL_Renderer* getRenderer()const { return m_pRender;}
private:
    void keyDownEv(const SDL_Event *ev);
    void keyUpEv(const SDL_Event *ev);
private:
    SDL_Window     *m_pWnd       = nullptr;
    SDL_Renderer   *m_pRender    = nullptr;

    std::vector<Sprite*>     m_spriteVec;
    GameStateMachine        *m_machine;
};

typedef Singleton<Game> Game_Singleton;

}