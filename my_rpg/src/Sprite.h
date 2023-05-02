#pragma once

#include <string>
#include <SDL2/SDL.h>
#include "Vector2D.h"

namespace SDL2
{
class LoadParams;
class Sprite
{
public:
    virtual void draw(SDL_Renderer* pRender)  = 0;
    virtual void update()= 0;
    virtual void clean() = 0;
    inline Vector2D getLeftTopPos()const
    {
        return m_leftTopPos;
    }
    inline Vector2D getRightBottomPos()const
    {
        return m_rightBottomPos;
    }
    inline int getWidth()const
    {
        return m_rightBottomPos.getX() - m_leftTopPos.getX();
    }
    inline int getHeight()const
    {
        return m_rightBottomPos.getY() - m_leftTopPos.getY();
    }
    inline int getLeft()
    {
        return m_leftTopPos.getX();
    }
    inline int getRight()
    {
        return m_rightBottomPos.getX();
    }
    inline int getTop()
    {
        return m_leftTopPos.getY();
    }
    inline int getBottom()
    {
        return m_rightBottomPos.getY();
    }
protected:
    Sprite(const LoadParams *pParams){}
    virtual ~Sprite(){}
private:
    Vector2D m_leftTopPos;
    Vector2D m_rightBottomPos;
};
}