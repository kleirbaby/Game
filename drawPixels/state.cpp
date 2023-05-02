#include "state.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include "GameLib/Framework.h"
#include "image.h"
#include "file.h"
#include "rect.h"

using namespace std;
using namespace Utils;

State::State(const char* stageData, int size)
	:mWidth(0), mHeight(0)
{
	parseMap(stageData, size);
}

void State::update(char input)
{
	int dx = 0, dy = 0;
	switch (input) {
	case 'w':
		dy--;
		break;
	case 's':
		dy++;
		break;
	case 'a':
		dx--;
		break;
	case 'd':
		dx++;
		break;
	}

	int x, y;
	for (y = 0; y < mHeight; ++y) {
		for (x = 0; x < mWidth; ++x) {
			if (mObjects(x, y).type == ObjectType::OBJ_MAN
				|| mObjects(x, y).type == ObjectType::OBJ_MAN_POINT) {
				goto FIND_MAN;
				break;
			}
		}
	}

FIND_MAN:
	if (x > mWidth || y > mHeight) {
		assert(0);
		return;
	}

	Object& obj = mObjects(x + dx, y + dy);
	switch (obj.type) {
	case ObjectType::OBJ_SPACE:
		obj.type = ObjectType::OBJ_MAN;
		mObjects(x, y) = mObjects(x, y).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		break;
	case ObjectType::OBJ_WALL:
		break;
	case ObjectType::OBJ_POINT:
		obj.type = ObjectType::OBJ_MAN_POINT;
		mObjects(x, y) = mObjects(x, y).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		break;
	case ObjectType::OBJ_BLOCK: {
		int blockX = x + dx;
		int blockY = y + dy;
		if (mObjects(blockX + dx, blockY + dy).type == ObjectType::OBJ_SPACE) {
			mObjects(blockX + dx, blockY + dy).type = ObjectType::OBJ_BLOCK;
			obj.type = ObjectType::OBJ_MAN;
			mObjects(x, y) = mObjects(x, y).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		}
		else if (mObjects(blockX + dx, blockY + dy).type == ObjectType::OBJ_POINT) {
			mObjects(blockX + dx, blockY + dy).type = ObjectType::OBJ_BLOCK_POINT;
			obj.type = ObjectType::OBJ_MAN;
			mObjects(x, y) = mObjects(x, y).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		}
	}
		break;
	case ObjectType::OBJ_BLOCK_POINT:
		//todo
		break;
	default:
		break;
	}
}

void State::draw() const
{
	if (!mImage) {
		assert(mImage);
		return;
	}

	//drawCell(Vec2(0, 0), Rect(static_cast<int>(TileID::IMG_WALL) * 32, 0, 32, 32), *mImage);
	//drawCell(Vec2(0, 0), Rect(0, 0, 32, 32), *mImage);
	//drawCell(Vec2(0, 0), Rect(0, 0, mImage->width(), mImage->heigth()), *mImage);
#if 0
	auto getUnsigned = [](const char* p) {
		const unsigned char* up;
		up = reinterpret_cast<const unsigned char*>(p);
		unsigned r = up[0];
		r |= up[1] << 8;
		r |= up[2] << 16;
		r |= up[3] << 24;
		return r;
	};

	char* buffer = nullptr;
	int size = 0;
	ifstream in("bar.dds", ifstream::binary);

	in.seekg(0, ifstream::end);
	size = static_cast<int>(in.tellg());
	in.seekg(0, ifstream::beg);
	buffer = new char[size];
	in.read(buffer, size);

	int h = getUnsigned(&(buffer[12]));
	int w = getUnsigned(&(buffer[16]));
	unsigned* data = new unsigned[w * h];
	for (int i = 0; i < w * h; ++i) {
		data[i] = getUnsigned(&(buffer[128 + i * 4]));
	}
#else
	//Image img;
	//File file("nimotsuKunImage.dds",IOMode::ReadOnly | IOMode::Binary);
	//if (file.isValid()) {
	//	img.loadFile(file);
	//}
	//int h = img.heigth();
	//int w = img.width();
	//const unsigned* data = img.data();
#endif
	//unsigned* vram = GameLib::Framework::instance().videoMemory();
	//int canvasWidth = GameLib::Framework::instance().width();

	//for (int iY = 0; iY < h; ++iY) {
	//	for (int iX = 0; iX < w; ++iX) {
	//		vram[iY * canvasWidth + iX] = data[iY * w + iX];
	//	}
	//}
	//return;

	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			const Object& obj = mObjects(x, y);
			GameLib::cout << "(" << x << "," << y << ") = ";
			GameLib::cout << (int)mObjects(x, y).type << GameLib::endl;
			switch (obj.type) {
			case ObjectType::OBJ_SPACE:
				//drawCell16(x, y, 0x000000);
				//drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_PLAYER) * 32,0,32,32), *mImage);
				break;
			case ObjectType::OBJ_WALL:
				//drawCell16(x, y, 0xffffff);
				drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_WALL) * 32, 0, 32, 32), *mImage);
				break;
			case ObjectType::OBJ_POINT:
				//drawCell16(x, y, 0xd9d919);
				drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_POINT) * 32, 0, 32, 32), *mImage);
				break;
			case ObjectType::OBJ_BLOCK:
				drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_BLOCK) * 32, 0, 32, 32), *mImage);
				//drawCell16(x, y, 0x0000ff);
				break;
			case ObjectType::OBJ_BLOCK_POINT:
				//drawCell16(x, y, 0xd9da18);
				drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_BLOCK_POINT) * 32, 0, 32, 32), *mImage);
				break;
			case ObjectType::OBJ_MAN:
				//drawCell16(x, y, 0x556677);
				drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_PLAYER) * 32, 0, 32, 32), *mImage);
				break;
			case ObjectType::OBJ_MAN_POINT:
				//drawCell16(x, y, 0xe2579e);
				drawCell(Vec2(x, y), Rect(static_cast<int>(TileID::IMG_PLAYER) * 32, 0, 32, 32), *mImage);
				break;
			default:
				break;
			}
		}
	}
}

bool State::hasCleared() const
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y).type == ObjectType::OBJ_BLOCK) {
				return false;
			}
		}
	}
	return true;
}

void State::loadTile()
{
	if (!mImage) {
		mImage = std::make_shared<Image>();
	}

	File imgFile("nimotsuKunImage.dds",IOMode::ReadOnly | IOMode::Binary);
	bool bLoadSuc = false;
	if (imgFile.isValid()) {
		bLoadSuc = mImage->loadFile(imgFile);
	}

	if (!bLoadSuc) {
		GameLib::cout << "State::loadTile,bLoadSuc == false!" << GameLib::endl;
		assert(bLoadSuc);
	}
}

bool State::parseMap(const char* stageData, int size)
{
	char szWidth[250] = {};
	char szHeight[250] = {};
	const char* ptr = ::strchr(stageData, '[');

	bool bParseSuc = false;
	if (ptr) {
		const char* dst = ptr + 1;
		ptr = ::strchr(dst, ',');
		if (ptr) {
			::strncpy_s(szWidth, 250, dst, ptr - dst);
			dst = ptr + 1;
			ptr = ::strchr(dst, ']');
			if (ptr) {
				::strncpy_s(szHeight, 250, dst, ptr - dst);
				bParseSuc = true;
			}
		}
	}

	if (!bParseSuc) {
		return false;
	}

	ptr = ::strchr(ptr, '\n');
	if (ptr) {
		ptr += strlen("\n");
	}

	if (::strlen(szWidth) > 0) {
		mWidth = ::atoi(szWidth);
	}

	if (::strlen(szHeight) > 0) {
		mHeight = ::atoi(szHeight);
	}

	/////////////////////
	const char* mapData = ptr;
	GameLib::cout << "/////////////////////" << GameLib::endl;
	for (int pos = 0; pos < size - (ptr - stageData); ++pos) {
		GameLib::cout << mapData[pos];
	}
	GameLib::cout << GameLib::endl;
	GameLib::cout << "/////////////////////" << GameLib::endl;

	mObjects.setSize(mWidth, mHeight);

	int p = 0;
	for (int pos = 0; pos < size - (ptr - stageData); ++pos) {
		Object obj;

		bool invalid = false;
		switch (mapData[pos]) {
		case '#':
			obj = Object(ObjectType::OBJ_WALL);
			break;
		case ' ':
			obj.type = ObjectType::OBJ_SPACE;
			break;
		case '.':
			obj.type = ObjectType::OBJ_POINT;
			break;
		case 'o':
			obj.type = ObjectType::OBJ_BLOCK;
			break;
		case 'O':
			obj.type = ObjectType::OBJ_BLOCK_POINT;
			break;
		case 'p':
			obj.type = ObjectType::OBJ_MAN;
			break;
		case 'P':
			obj.type = ObjectType::OBJ_MAN_POINT;
			break;
		default:
			invalid = true;
			break;
		}

		if (invalid) {
			invalid = false;
			continue;
		}

		int x = p % mWidth;
		int y = p / mWidth;
		p++;

		if (x > mWidth || y > mHeight) {
			assert(0);
			return false;
		}

		mObjects(x, y) = std::move(obj);
	}

	return true;
}

void State::drawCell16(int x, int y, unsigned color)const
{
	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int canvasWidth = GameLib::Framework::instance().width();

	for (int iY = 0; iY < 16; ++iY) {
		for (int iX = 0; iX < 16; ++iX) {
			vram[(iY + y * 16) * canvasWidth + (iX + x * 16)] = color;
		}
	}
}

void State::drawCell(const Utils::Vec2& pos, const Utils::Rect& rect, const Image& img)const
{
	if (!img.isValid()) {
		return;
	}

	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int canvasWidth = GameLib::Framework::instance().width();

	Image partImg = img.part(rect);

	for (int iY = 0; iY < partImg.heigth(); ++iY) {
		for (int iX = 0; iX < partImg.width(); ++iX) {
			vram[(iY + pos.mY) * canvasWidth + (iX + pos.mX)] = partImg.data()[iY * partImg.width() + iX];
		}
	}
}