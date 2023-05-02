#pragma once

#include "GameState.h"

namespace SDL2
{
class MenuState:public GameState
{
public:
    virtual void update();
    virtual void render();
    virtual bool onEnter();
    virtual bool onExit();
    inline virtual std::string getStateId()const{ return s_menuId; }
private:
    static const std::string s_menuId;
};
}