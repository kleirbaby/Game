#pragma once
#include "global.h"
#include "image.h"

namespace Utils
{
	class StringRenderer
	{
	public:
		static StringRenderer& getInstance()
		{
			static StringRenderer strRender;
			return strRender;
		}

		static StringRenderer* getInstancePtr()
		{
			return &getInstance();
		}

		StringRenderer();
		~StringRenderer();
		void draw(int x,int y,const char* str);
		void draw(int x, int y, const String& str);
	private:
		bool loadFile(const String& fontFilePath);
		//注意：这里的dstX、dstY、srcX、srcY都是像素坐标，并不是网格坐标。
		void draw(int dstX, int dstY, int srcX, int srcY, const Image& img);
	private:
		Image mFontImg;
	};
}//Utils

