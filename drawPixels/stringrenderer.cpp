#include "stringrenderer.h"
#include "file.h"
#include "GameLib/Framework.h"
#include <cassert>

namespace Utils
{
	/*
		��ʱ����������ɣ���̫������������Ӧ����������
	*/

	//todo�������������ô��ƣ�
	//...

	static const String DEF_FONT_IMAGE = "images/font.dds";

	StringRenderer::StringRenderer()
		:mFontColor(0)
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
				unsigned p = partImg.data()[iY * partImg.width() + iX];
				if (mFontColor > 0) {
					p = blendColor(&p, &vram[(iY + dstY) * windowWidth + (iX + dstX)], mFontColor);
				}
				vram[(iY + dstY) * windowWidth + (iX + dstX)] = p;
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

		//�ַ���Ⱥ͸߶�
		const int chWidth = 8;
		const int chHeight = 16;

		//����ͼƬÿ���ж����ַ�
		int numOfRow = mFontImg.width() / chWidth;

		for (int i = 0; str[i] != '\0'; ++i) {
			//��Χ���
			int t = str[i];
			if (t < 32 || t > 127) {
				t = 127;
			}

			t -= 32;
			int srcX = (t % numOfRow) * chWidth;
			int srcY = (t / numOfRow) * chHeight;
			
			draw(dstX,dstY,srcX,srcY, mFontImg);
			dstX += chWidth;//�ƶ�����һ���ַ�(����Ӧ�û�������)
		}
	}

	void StringRenderer::draw(int x, int y, const String& str)
	{
		draw(x,y,str.c_str());
	}

	void StringRenderer::draw(int x, int y, const String& str, unsigned color)
	{
		mFontColor = color;
		draw(x, y, str.c_str());
	}

	unsigned StringRenderer::blendColor(const unsigned* srcPixsel, const unsigned* dstPixsel, unsigned color)
	{
		unsigned srcR = color & 0xff0000;
		unsigned srcG = color & 0x00ff00;
		unsigned srcB = color & 0x0000ff;

		unsigned srcA = *srcPixsel & 0xff;//��ȡ��ɫ��Ϣ
		unsigned dstR = *dstPixsel & 0xff0000;
		unsigned dstG = *dstPixsel & 0x00ff00;
		unsigned dstB = *dstPixsel & 0x0000ff; 

		unsigned r = (srcR - dstR) * srcA / 255 + dstR;
		unsigned g = (srcG - dstG) * srcA / 255 + dstG;
		unsigned b = (srcB - dstB) * srcA / 255 + dstB;

		return (r & 0xff0000) | (g & 0x00ff00) | b;
	}

	unsigned StringRenderer::blendColor2(const unsigned* srcPixsel, const unsigned* dstPixsel, unsigned color)
	{
		uint8 alpha = (*srcPixsel & 0xff000000) >> 24;
		double srcA = static_cast<double>(alpha / 255.0);
		
		//͸�����
		double srcR = static_cast<double>((color & 0x00ff0000) >> 16);
		double srcG = static_cast<double>((color & 0x0000ff00) >> 8);
		double srcB = static_cast<double>(color & 0x000000ff);

		double dstR = static_cast<double>((*dstPixsel & 0x00ff0000) >> 16);
		double dstG = static_cast<double>((*dstPixsel & 0x0000ff00) >> 8);
		double dstB = static_cast<double>(*dstPixsel & 0x000000ff);

		double r = (srcR - (1.f - srcA) * dstR)/srcA;
		double g = (srcG - (1.f - srcA) * dstG)/srcA;
		double b = (srcB - (1.f - srcA) * dstB)/srcA;

		unsigned c = static_cast<unsigned>(r) << 16;
		c |= static_cast<unsigned>(g) << 8;
		c |= static_cast<unsigned>(b);

		return c;
	}
}//Utils