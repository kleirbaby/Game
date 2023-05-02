#pragma once

#include <string>

namespace SDL2
{
class LoadParams
{
public:
    LoadParams(int x,int y,int w,int h,std::string textureId)
        :m_x(x),m_y(y),m_width(w),m_height(h),m_textureId(textureId){}
    ~LoadParams(){}

    inline int getX()const { return m_x; }
    inline int getY()const { return m_y; }
    inline int getWidth()const { return m_width; }
    inline int getHeight()const { return m_height; }

    inline std::string getTextureId()const { return m_textureId; }
private:
    std::string m_textureId;
    int         m_x;
    int         m_y;
    int         m_width;
    int         m_height;
};
}