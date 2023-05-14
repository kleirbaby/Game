#pragma once
#include "global.h"
#include "file.h"
#include "rect.h"

/*针对像素大小是4个字节的dds图片。即A8R8G8B8*/
namespace Utils
{
	class Image
	{
	public:
		enum class Type { Invalid = 0, DDS32 };
		Image();
		Image(const Image& img);
		Image& operator=(const Image& img);
		~Image();
		bool isValid()const { return mType != Type::Invalid; }
		bool loadFile(File& file);
		int width()const;
		int heigth()const;
		const unsigned* data()const;
		Image part(const Rect& rect)const;

		//获取像素的透明度[0,255]
		uint8 getAlpha(unsigned* pixsel)const;
	private:
		bool loadDDS32(const char* buffer, int size);
	private:
		Type mType;
		int mWidth;
		int mHeight;
		unsigned* mData;
	};
}

