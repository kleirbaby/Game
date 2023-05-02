#include "Player.h"
#include "LoadParams.h"
#include "TextureManager.h"

Player::Player(const SDL2::LoadParams *pParam)
    :Sprite(pParam),m_pos(pParam->getX(),pParam->getY()),m_vel(0,0)
{
    m_width     = pParam->getWidth();
    m_height    = pParam->getHeight();
    m_textureId = pParam->getTextureId();

    m_curRow    = 1;
    m_curFrame  = 1;
}

Player::~Player()
{

}

void Player::draw(SDL_Renderer* pRender)
{
    SDL2::TextureManager_Singleton().getInstance()->drawFrame(m_textureId,(int)m_pos.getX(),(int)m_pos.getY(),m_width,m_height,m_curRow,m_curFrame,pRender);
}

void Player::update()
{
    m_vel.setX(1);
    m_pos += m_vel;
    m_curFrame = int((SDL_GetTicks()/100)%6);
}

void Player::clean()
{

}