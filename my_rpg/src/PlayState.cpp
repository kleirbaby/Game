#include "PlayState.h"

using namespace SDL2;

const std::string PlayState::s_playId = "PLAY";

void PlayState::update()
{

}

void PlayState::render()
{

}

bool PlayState::onEnter()
{
    return true;
}

bool PlayState::onExit()
{
    return true;
}

bool PlayState::checkCollision(Sprite *s1,Sprite *s2)
{
    if(s1->getLeft()  > s2->getRight())
        return false;
    if(s1->getRight() < s2->getLeft())
        return false;
    if(s1->getTop()   > s2->getBottom())
        return false;
    if(s1->getBottom()< s2->getTop())
        return false;
    return true;
}