#include "Game.h"
#include "../common/Commondef.hpp"
#include "TextureManager.h"
#include "Sprite.h"
#include "Player.h"
#include "LoadParams.h"
#include "MenuState.h"
#include "PlayState.h"
#include "GameStateMachine.h"

using namespace SDL2;

Game::Game()
{
    SDL2::LoadParams param1(20,20,120,80,"test1");
    SDL2::LoadParams param2(20,20,120,80,"test2");
    m_spriteVec.push_back(new Player(&param1));
    m_spriteVec.push_back(new Player(&param2));
}

Game::~Game()
{
    TextureManager_Singleton().clean();
}

bool Game::init(std::string title,int x,int y,int w,int h,int flags /*= SDL_WINDOW_SHOWN*/)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        dPrint("sdl init fail\n");
        return false;
    }
    
    m_pWnd = SDL_CreateWindow(title.c_str(),x,y,w,h,flags);
    if(m_pWnd == NULL){
        dPrint("create window fail\n");
        return false;
    }

    m_pRender = SDL_CreateRenderer(m_pWnd,-1,0);
    if(m_pRender == NULL){
        dPrint("create renderer fail\n");
        return false;
    }

    //set default color
    SDL_SetRenderDrawColor(m_pRender,175,175,175,50);

    m_machine = new GameStateMachine;
    m_machine->changeState(new MenuState);

    //fixed me
    if(!TextureManager_Singleton().getInstance()->load("../assets/meinv3.jpeg","beauty",m_pRender)){
        dPrint("texture manager load fail\n");
        return false;
    }

    return true;
}

void Game::render()
{
    SDL_RenderClear(m_pRender);

    //fixed me
    TextureManager_Singleton().getInstance()->draw("beauty",50,40,220,140,m_pRender);

    SDL_RenderPresent(m_pRender);
}

void Game::update()
{
    for(std::vector<Sprite*>::size_type i = 0;i < m_spriteVec.size();++i){
        m_spriteVec[i]->update();
    }
}

void Game::draw()
{
    for(std::vector<Sprite*>::size_type i = 0;i < m_spriteVec.size();++i){
        m_spriteVec[i]->draw(m_pRender);
    }
}

void Game::handleEvents()
{
    loop();
}

void Game::clean()
{
    SDL_DestroyWindow(m_pWnd);
    SDL_DestroyRenderer(m_pRender);
    SDL_Quit();
}

void Game::keyDownEv(const SDL_Event *ev)
{
    if(ev->key.keysym.sym == SDLK_RETURN){
        dPrint("enter is down\n");
        m_machine->changeState(new PlayState);
    }
}

void Game::keyUpEv(const SDL_Event *ev)
{
    if(ev->key.keysym.sym == SDLK_RETURN){
        dPrint("enter is up\n");
    }
}