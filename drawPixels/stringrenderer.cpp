#include "stringrenderer.h"
#include "file.h"
#include "GameLib/Framework.h"
#include <cassert>

namespace Utils
{
	/*
		暂时先这样处理吧，不太清楚其余字体库应该怎样处理
	*/

	//todo，其余字体库怎么设计？
	//...

	static const String DEF_FONT_IMAGE = "images/font.dds";

	StringRenderer::StringRenderer()
	{
		loadFile(DEF_FONT_IMAGE);
	}

	StringRenderer::~StringRenderer()
	{
	}

	bool StringRenderer::loadFile(const String& fontFilePath)
	{
		File fontFile(fontFilePath, IOMode::ReadOnly | IOMode::Binary);
		if (!fontFile.isValid()) {
			assert(0 && "load font file fail!");
			return false;
		}

		if (!mFontImg.loadFile(fontFile)) {
			assert(0 && "load image file fail!");
			return false;
		}

		return true;
	}

	void StringRenderer::draw(int dstX, int dstY, int srcX, int srcY, const Image& img)
	{
		unsigned* vram = GameLib::Framework::instance().videoMemory();
		int windowWidth = GameLib::Framework::instance().width();

		auto partImg = img.part(Rect(srcX,srcY,8,16));

		for (int iY = 0; iY < partImg.heigth(); ++iY) {
			for (int iX = 0; iX < partImg.width(); ++iX) {
				vram[(iY + dstY) * windowWidth + (iX + dstX)] = partImg.data()[iY * partImg.width() + iX];
			}
		}
	}

	void StringRenderer::draw(int x, int y, const char* str)
	{
		if (!str || ::strlen(str) <= 0) {
			return;
		}

		int dstX = x;
		int dstY = y;

		//字符宽度和高度
		const int chWidth = 8;
		const int chHeight = 16;

		//字体图片每行有多少字符
		int numOfRow = mFontImg.width() / chWidth;

		for (int i = 0; str[i] != '\0'; ++i) {
			//范围检测
			int t = str[i];
			if (t < 32 || t > 127) {
				t = 127;
			}

			t -= 32;
			int srcX = (t % numOfRow) * chWidth;
			int srcY = (t / numOfRow) * chHeight;
			
			draw(dstX,dstY,srcX,srcY, mFontImg);
			dstX += chWidth;//移动到下一个字符(换行应该会有问题)
		}
	}

	void StringRenderer::draw(int x, int y, const String& str)
	{
		draw(x,y,str.c_str());
	}
}//Utils