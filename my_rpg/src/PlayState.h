#pragma once

#include "GameState.h"
#include "Sprite.h"

namespace SDL2
{
class PlayState:public GameState
{
public:
    virtual void update();
    virtual void render();
    virtual bool onEnter();
    virtual bool onExit();
    inline virtual std::string getStateId()const{ return s_playId; }

    bool checkCollision(Sprite *s1,Sprite *s2);
private:
    static const std::string s_playId;
};
}