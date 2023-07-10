#include "stringrenderer.h"
#include "file.h"
#include "image.h"
#include "GameLib/Framework.h"
#include <cassert>

namespace Utils
{
	class FontPkg
	{
	protected:
		virtual void loadFile(const String& fontFilePath) = 0;
	};

	//字体图片
	class FontImagePkg : public FontPkg
	{
	public:
		FontImagePkg(const String& fontFilePath)
		{
			loadFile(fontFilePath);
		}

		void loadFile(const String& fontFilePath)override
		{
			File fontFile(fontFilePath, IOMode::ReadOnly | IOMode::Binary);
			if (!fontFile.isValid()) {
				assert(0 && "load font file fail!");
				return;
			}

			if (!mFontImg.loadFile(fontFile)) {
				assert(0 && "load image file fail!");
				return;
			}
		}

		Image* fontImage()
		{
			if (mFontImg.isValid()) {
				return &mFontImg;
			}
			return nullptr;
		}

		int fontWith()const
		{
			return 8;
		}

		int fontHeight()const
		{
			return 16;
		}
	private:
		Image mFontImg;
	};

	//todo，其余字体库怎么设计？
	//...

	static const String DEF_FONT_IMAGE = "images/font.dds";

	StringRenderer::StringRenderer()
		:mFontPkg(new FontImagePkg(DEF_FONT_IMAGE))
	{
	}

	StringRenderer::~StringRenderer()
	{
		delete mFontPkg;
	}

	void StringRenderer::draw(int x, int y, const char* str)
	{
		auto img = mFontPkg->fontImage();
		if (!img || !str || ::strlen(str) <= 0) {
			return;
		}

		int dstX = x;
		int dstY = y;

		//字符宽度和高度
		int chWidth = mFontPkg->fontWith();
		int chHeight = mFontPkg->fontHeight();

		//字体图片每行有多少字符
		int numOfRow = img->width() / chWidth;

		for (int i = 0; str[i] != '\0'; ++i) {

		}

		//unsigned* vram = GameLib::Framework::instance().videoMemory();
		//int windowWidth = GameLib::Framework::instance().width();

		//for (int iY = 0; iY < partImg.heigth(); ++iY) {
		//	for (int iX = 0; iX < partImg.width(); ++iX) {
		//		vram[(iY + pos.mY * rect.mHeight) * windowWidth + (iX + pos.mX * rect.mWidth)] = partImg.data()[iY * partImg.width() + iX];
		//	}
		//}
	}

	void StringRenderer::draw(int x, int y, const String& str)
	{

	}
}//Utils