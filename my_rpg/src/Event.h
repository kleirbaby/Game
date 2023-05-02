/*
    该类用于处理事件
*/
#pragma once

#include <SDL2/SDL.h>
#include <functional>

namespace SDL2
{
enum EVENT_WAY
{
    POLL = 0,
    WAIT = 1
};

class Event
{
public:
    virtual bool loop();
    void exit();
protected:
    Event(EVENT_WAY way = POLL);
    virtual ~Event(){}

    virtual void keyDownEv(const SDL_Event *ev)   = 0;
    virtual void keyUpEv(const SDL_Event *ev)     = 0;

    virtual void mouseDownEv(const SDL_Event *ev) = 0;
    virtual void mouseUpEv(const SDL_Event *ev)   = 0;
    virtual void mouseMoveEv(const SDL_Event *ev) = 0;

    virtual void quitEv(const SDL_Event *ev)      = 0;

    virtual void signalEv()                       = 0;
private:
    bool      m_bExit;
    SDL_Event m_ev;
    EVENT_WAY m_evWay;

    std::function<int(SDL_Event*)> handEvent;
};
}
