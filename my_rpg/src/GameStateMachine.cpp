#include "GameStateMachine.h"
#include "GameState.h"

using namespace SDL2;

GameStateMachine::~GameStateMachine()
{
    for(auto state : m_gameStates){
        if(state != nullptr){
            delete state;
            state = nullptr;
        }
    }
    m_gameStates.clear();
}

void GameStateMachine::pushState(GameState* pState)
{
    m_gameStates.push_back(pState);
    pState->onEnter();
}

void GameStateMachine::changeState(GameState* pState)
{
    if(!m_gameStates.empty()){   
        if(m_gameStates.back()->getStateId() == pState->getStateId()){
            return;
        }

        if(m_gameStates.back()->onExit()){
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    } 

    m_gameStates.push_back(pState);
    m_gameStates.back()->onEnter();
}

void GameStateMachine::popState()
{
    if(!m_gameStates.empty()){
        if(m_gameStates.back()->onExit()){
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    }
}