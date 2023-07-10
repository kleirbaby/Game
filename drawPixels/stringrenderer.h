#pragma once
#include "global.h"

namespace Utils
{
	class FontImagePkg;
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
		FontImagePkg* mFontPkg;
	};
}//Utils

