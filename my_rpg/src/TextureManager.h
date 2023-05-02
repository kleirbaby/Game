#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>
#include "../common/Singleton.h"

namespace SDL2
{
class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    bool load(std::string fileName,std::string id,SDL_Renderer *pRender);
    bool draw(std::string id,int x,int y,int w,int h,SDL_Renderer *render,SDL_RendererFlip flip = SDL_FLIP_NONE);
    bool drawFrame(std::string id,int x,int y,int w,int h,int curRow,int curFrame,SDL_Renderer *render,SDL_RendererFlip flip = SDL_FLIP_NONE);
private:
    std::map<std::string,SDL_Texture*> m_textureMap;
};

typedef Singleton<TextureManager> TextureManager_Singleton;
}