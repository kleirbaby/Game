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
		void draw(int x, int y, const String& str, unsigned color);
	private:
		bool loadFile(const String& fontFilePath);
		//ע�⣺�����dstX��dstY��srcX��srcY�����������꣬�������������ꡣ
		void draw(int dstX, int dstY, int srcX, int srcY, const Image& img);
		unsigned blendColor(const unsigned* srcPixsel,const unsigned* dstPixsel,unsigned color);
	private:
		Image mFontImg;
		unsigned mFontColor;
	};
}//Utils

