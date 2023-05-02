#pragma once

#include "Sprite.h"
#include "Vector2D.h"

class SDL2::LoadParams;
class Player:public SDL2::Sprite
{
public:
    Player(const SDL2::LoadParams *pParam);
    ~Player();

    void draw(SDL_Renderer* pRender);
    void update();
    void clean();
private:
    std::string m_textureId;
    int         m_curFrame;
    int         m_curRow;
    int         m_width;
    int         m_height;

    SDL2::Vector2D    m_pos;
    SDL2::Vector2D    m_vel;
};