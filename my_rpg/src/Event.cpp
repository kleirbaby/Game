#include <stdio.h>
#include "Event.h"
#include "../common/Commondef.hpp"

using namespace SDL2;

Event::Event(EVENT_WAY way /*= POLL*/)
    :m_evWay(POLL),m_bExit(false),handEvent(way == POLL?(SDL_PollEvent):SDL_WaitEvent)
{  
}

void Event::exit()
{
    m_bExit = true;
}

bool Event::loop()
{
    while(!m_bExit){
        while(handEvent(&m_ev)){
            switch (m_ev.type)
            {
            case SDL_QUIT:
                quitEv(&m_ev);
                break;
            case SDL_KEYDOWN:
                keyDownEv(&m_ev);
                break;
            case SDL_KEYUP:
                keyUpEv(&m_ev);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseDownEv(&m_ev);
                break;
            case SDL_MOUSEBUTTONUP:
                mouseUpEv(&m_ev);
                break;
            case SDL_MOUSEMOTION:
                mouseMoveEv(&m_ev);
                break;
            default:
                dPrint("at the moment,the event cannot be surpport!\n");
                break;
            }
        }
    }

    return true;
}