#pragma once

#include "InputHandler.h"

namespace SDL2
{
class KeyboardHandler:public InputHandler
{
public:
    KeyboardHandler();
    ~KeyboardHandler();
};
}