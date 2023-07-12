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
		//ע�⣺�����dstX��dstY��srcX��srcY�����������꣬�������������ꡣ
		void draw(int dstX, int dstY, int srcX, int srcY, const Image& img);
	private:
		Image mFontImg;
	};
}//Utils

