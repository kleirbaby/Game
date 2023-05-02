/*
    FSM:finite state machine.A game has a number of states.
*/

#pragma once

#include <vector>

namespace SDL2
{
class GameState;
class GameStateMachine
{
public:
    ~GameStateMachine();
    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();
private:
    std::vector<GameState*> m_gameStates;
};
}