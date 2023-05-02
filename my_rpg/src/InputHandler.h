#pragma once

#include "Event.h"

namespace SDL2
{
class InputHandler:public Event
{
public:
    InputHandler();
    ~InputHandler();
protected:
    virtual void keyDownEv(const SDL_Event *ev);
    virtual void keyUpEv(const SDL_Event *ev);

    virtual void mouseDownEv(const SDL_Event *ev);
    virtual void mouseUpEv(const SDL_Event *ev);
    virtual void mouseMoveEv(const SDL_Event *ev);

    virtual void quitEv(const SDL_Event *ev);

    virtual void signalEv();
};
}