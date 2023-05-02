#pragma once

#include "InputHandler.h"

namespace SDL2
{
enum Mouse_Buttons
{
    LEFT   = 0,
    MIDDLE = 1,
    RIGHT  = 2
};

class MouseHandler:public InputHandler
{
public:
    MouseHandler();
    ~MouseHandler();
};
}