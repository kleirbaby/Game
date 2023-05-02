#include "TextureManager.h"
#include "../common/Commondef.hpp"

using namespace SDL2;

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

bool TextureManager::load(std::string fileName,std::string id,SDL_Renderer *pRender)
{
    SDL_Surface *pSurface = IMG_Load(fileName.c_str());
    if(pSurface == NULL){
        dPrint("load img fail\n");
        return false;
    }

    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRender,pSurface);
    if(pTexture == NULL){
        dPrint("create texture from surface fail\n");
        SDL_FreeSurface(pSurface);
        return false;
    }

    SDL_FreeSurface(pSurface);
    m_textureMap[id] = pTexture;

    return true;
}

bool TextureManager::draw(std::string id,int x,int y,int w,int h,SDL_Renderer *render,SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;

    srcRect.x = 0;
    srcRect.y = 0;

    srcRect.w = dstRect.w = w;
    srcRect.h = dstRect.h = h;

    dstRect.x = x;
    dstRect.y = y;

    if(SDL_RenderCopyEx(render,m_textureMap[id],&srcRect,&dstRect,0,0,flip) != 0){
        dPrint("texture render copy fail\n");
        return false;
    }
    return true;
}

bool TextureManager::drawFrame(std::string id,int x,int y,int w,int h,int curRow,int curFrame,SDL_Renderer *render,SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
    SDL_Rect srcRect;
    SDL_Rect dstRect;

    srcRect.x = w * curFrame;
    srcRect.y = h * (curRow - 1);

    srcRect.w = dstRect.w = w;
    srcRect.h = dstRect.h = h;

    dstRect.x = x;
    dstRect.y = y;

    if(SDL_RenderCopyEx(render,m_textureMap[id],&srcRect,&dstRect,0,0,flip) != 0){
        dPrint("texture render copy fail\n");
        return false;
    }
    return true;
}