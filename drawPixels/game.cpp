#include "game.h"
#include "global.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdio>
#include "GameLib/Framework.h"
#include "image.h"
#include "file.h"
#include "rect.h"
#include "stringrenderer.h"

using namespace std;
using namespace Utils;

constexpr int PER_FRAME_MOVE = 1;
constexpr int PER_CELL = 32;
constexpr int LIMIT_FRAMES = PER_CELL / PER_FRAME_MOVE;

static bool findPlayer(const Array2D<Game::Object>& objs, Game::Coord& coord);

Game::Game(const char* stageData, int size)
	:mWidth(0), mHeight(0)
{
	parseMap(stageData, size); 
}

void Game::update()
{
	if (mPerMoveFrames >= LIMIT_FRAMES) {
		mPerMoveFrames = 0;
		for (int y = 0; y < mHeight; ++y) {
			for (int x = 0; x < mWidth; ++x) {
				mObjects(x, y).mCoord = Coord(0, 0);
			}
		}
		return;
	}

	int dx = 0, dy = 0;
	if (canMovePerOn('a')) {
		dx -= 1;
	}
	else if (canMovePerOn('d')) {
		dx += 1;
	}
	else if (canMovePerOn('w')) {
		dy -= 1;
	}
	else if (canMovePerOn('s')) {
		dy += 1;
	}
	
	if (mPerMoveFrames > 0) {
		mPerMoveFrames++;
	}
	
	Coord coord;
	if (!findPlayer(mObjects, coord)) {
		GameLib::cout << "Game::update(),find player failure! coord: (" << coord.mX << "," << coord.mY << ")." << GameLib::endl;
		return;
	}

	int x = coord.mX + dx;
	int y = coord.mY + dy;

	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
		GameLib::cout << "Game::update(),over mObjects,x: " << x << ",y: " << y << GameLib::endl;
		return;
	}

	Object& obj = mObjects(x, y);
	switch (obj.mType) {
	case ObjectType::OBJ_FLOOR:
	{
		mPerMoveFrames = 1;
		obj.move(ObjectType::OBJ_MAN, dx, dy);
		ObjectType type = mObjects(coord.mX, coord.mY).mType == ObjectType::OBJ_MAN ? ObjectType::OBJ_FLOOR : ObjectType::OBJ_POINT;
		mObjects(coord.mX, coord.mY).move(type, 0, 0);
		break;
	}
	case ObjectType::OBJ_POINT:
	{
		mPerMoveFrames = 1;
		obj.move(ObjectType::OBJ_MAN_POINT, dx, dy);
		ObjectType type = mObjects(coord.mX, coord.mY).mType == ObjectType::OBJ_MAN ? ObjectType::OBJ_FLOOR : ObjectType::OBJ_POINT;
		mObjects(coord.mX, coord.mY).move(type, 0, 0);
		break;
	}
	case ObjectType::OBJ_BLOCK: 
	{
		mPerMoveFrames = 1;
		if (mObjects(x + dx, y + dy).mType == ObjectType::OBJ_FLOOR) {
			mObjects(x + dx, y + dy).move(ObjectType::OBJ_BLOCK,dx,dy);
			obj.move(ObjectType::OBJ_MAN, dx, dy);
			ObjectType type = mObjects(coord.mX, coord.mY).mType == ObjectType::OBJ_MAN ? ObjectType::OBJ_FLOOR : ObjectType::OBJ_POINT;
			mObjects(coord.mX, coord.mY).move(type, 0, 0);
		}
		else if (mObjects(x + dx, y + dy).mType == ObjectType::OBJ_POINT) {
			mObjects(x + dx, y + dy).mType = ObjectType::OBJ_BLOCK_POINT;
			obj.move(ObjectType::OBJ_MAN, dx, dy);
			ObjectType type = mObjects(coord.mX, coord.mY).mType == ObjectType::OBJ_MAN ? ObjectType::OBJ_FLOOR : ObjectType::OBJ_POINT;
			mObjects(coord.mX, coord.mY).move(type, 0, 0);
		}
		break;
	}
	default:
		break;
	}
}

void Game::draw()
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			const Object& obj = mObjects(x, y);
#if LOG
			GameLib::cout << "(" << x << "," << y << ") = ";
			GameLib::cout << (int)mObjects(x, y).mType << GameLib::endl;
#endif
			//绘制背景
			drawBackground(obj, Coord(x, y));

			//绘制前景
			drawForeground(obj, Coord(x, y));
		}
	}

	//测试
	test();
}

bool Game::hasCleared() const
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y).mType == ObjectType::OBJ_BLOCK) {
				return false;
			}
		}
	}
	return true;
}

void Game::loadTile()
{
	if (!mImage) {
		mImage = std::make_shared<Image>();
		assert(mImage);
	}

	File imgFile("nimotsuKunImage2.dds",IOMode::ReadOnly | IOMode::Binary);
	bool bLoadSuc = false;
	if (imgFile.isValid()) {
		bLoadSuc = mImage->loadFile(imgFile);
	}

	if (!bLoadSuc) {
		GameLib::cout << "Game::loadTile,bLoadSuc == false!" << GameLib::endl;
		assert(bLoadSuc);
	}
}

void Game::test()const
{
	//auto testImg = std::make_shared<Image>();

	//File imgFile("forgroundW.dds", IOMode::ReadOnly | IOMode::Binary);
	//testImg->loadFile(imgFile);
	////drawCell(Coord(0, 0), Rect(0, 0, 128, 128), *testImg);
	//drawCellAlphaBlend(Coord(0, 0), Rect(0, 0, 128, 128), *testImg);

	StringRenderer::getInstance().draw(0, 0, "Hello");
}

bool Game::parseMap(const char* stageData, int size)
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
			obj.mType = ObjectType::OBJ_FLOOR;
			break;
		case '.':
			obj.mType = ObjectType::OBJ_POINT;
			break;
		case 'o':
			obj.mType = ObjectType::OBJ_BLOCK;
			break;
		case 'O':
			obj.mType = ObjectType::OBJ_BLOCK_POINT;
			break;
		case 'p':
			obj.mType = ObjectType::OBJ_MAN;
			break;
		case 'P':
			obj.mType = ObjectType::OBJ_MAN_POINT;
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

void Game::drawPixel(const Coord& pos, unsigned color)const
{
	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	vram[pos.mY * windowWidth +pos.mX] = color;
}

void Game::drawCell(const Coord& pos, const Vec2& size, unsigned color)const
{
	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	for (int iY = 0; iY < size.mY; ++iY) {
		for (int iX = 0; iX < size.mX; ++iX) {
			vram[(iY + pos.mY * size.mY) * windowWidth + (iX + pos.mX * size.mX)] = color;
		}
	}
}

void Game::drawCell(const Coord& pos, const Utils::Rect& rect, const Image& img)const
{
	if (!img.isValid()) {
		return;
	}

	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	Image partImg = img.part(rect);

	for (int iY = 0; iY < partImg.heigth(); ++iY) {
		for (int iX = 0; iX < partImg.width(); ++iX) {
			vram[(iY + pos.mY * rect.mHeight) * windowWidth + (iX + pos.mX * rect.mWidth)] = partImg.data()[iY * partImg.width() + iX];
		}
	}
}

void Game::drawCellAlphaTest(const Coord& pos, const Rect& rect, const Image& img)const
{
	if (!img.isValid()) {
		return;
	}

	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	Image partImg = img.part(rect);

	for (int iY = 0; iY < partImg.heigth(); ++iY) {
		for (int iX = 0; iX < partImg.width(); ++iX) {
			unsigned pixsel = partImg.data()[iY * partImg.width() + iX];
			uint8 alpha = img.getAlpha(&pixsel);
			//这里只是简单区分透明和不透明，所以使用大于等于128和小于128来判断
			//这样透明通道就不会限制难么死
			if (alpha >= 128) {
				vram[(iY + pos.mY * rect.mHeight) * windowWidth + (iX + pos.mX * rect.mWidth)] = pixsel;
			}
		}
	}
}

void Game::drawCellAlphaTest(const Vec2& pos, const Rect& rect, const Image& img)const
{
	if (!img.isValid()) {
		return;
	}

	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	Image partImg = img.part(rect);

	for (int iY = 0; iY < partImg.heigth(); ++iY) {
		for (int iX = 0; iX < partImg.width(); ++iX) {
			unsigned pixsel = partImg.data()[iY * partImg.width() + iX];
			uint8 alpha = img.getAlpha(&pixsel);
			//这里只是简单区分透明和不透明，所以使用大于等于128和小于128来判断
			//这样透明通道就不会限制难么死
			if (alpha >= 128) {
				vram[(iY + pos.mY) * windowWidth + (iX + pos.mX)] = pixsel;
			}
		}
	}
}

void Game::drawCellAlphaBlend(const Coord& pos, const Rect& rect, const Image& img)const
{
	if (!img.isValid()) {
		return;
	}

	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	Image partImg = img.part(rect);

	for (int iY = 0; iY < partImg.heigth(); ++iY) {
		for (int iX = 0; iX < partImg.width(); ++iX) {
			unsigned srcPixsel = partImg.data()[iY * partImg.width() + iX];
			uint8 alpha = img.getAlpha(&srcPixsel);
			//透明混合
			double srcA = static_cast<double>(alpha / 255.0);
			double srcR = static_cast<double>((srcPixsel & 0x00ff0000) >> 16);
			double srcG = static_cast<double>((srcPixsel & 0x0000ff00) >> 8);
			double srcB = static_cast<double>(srcPixsel & 0x000000ff);

			unsigned* dstPixsel = vram + (iY * windowWidth + iX);
			double dstR = static_cast<double>((*dstPixsel & 0x00ff0000) >> 16);
			double dstG = static_cast<double>((*dstPixsel & 0x0000ff00) >> 8);
			double dstB = static_cast<double>(*dstPixsel & 0x000000ff);

			double r = srcR * srcA + (1.f - srcA) * dstR;
			double g = srcG * srcA + (1.f - srcA) * dstG;
			double b = srcB * srcA + (1.f - srcA) * dstB;

			*dstPixsel = static_cast<unsigned>(r) << 16;
			*dstPixsel |= static_cast<unsigned>(g) << 8;
			*dstPixsel |= static_cast<unsigned>(b);
		}
	}
}

bool Game::canMovePerOn(int input)
{
	auto f = GameLib::Framework::instance();
	auto s = f.isKeyOn(input);

	if (s && (mKeyStatus.find(input) == mKeyStatus.end() || !mKeyStatus[input])) {
		mKeyStatus[input] = s;
		return true;
	}

	mKeyStatus[input] = s;
	return false;
}

bool findPlayer(const Array2D<Game::Object>& objs, Game::Coord& coord)
{
	int x, y;

	int width = 0,height = 0;
	objs.size(width, height);

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			if (objs(x, y).mType == Game::ObjectType::OBJ_MAN
				|| objs(x, y).mType == Game::ObjectType::OBJ_MAN_POINT) {
				break;
			}
		}

		if (x < width) {
			break;
		}
	}

	if (x < width && y < height) {
		coord = Game::Coord(x, y);
		return true;
	}

	return false;
}

void Game::drawBackground(const Object& obj, const Coord& coord)
{
	if (obj.mType == ObjectType::OBJ_WALL) {
		drawCellAlphaTest(coord, Rect(static_cast<int>(TileID::IMG_WALL) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
	}
	else if (obj.mType == ObjectType::OBJ_POINT 
		|| obj.mType == ObjectType::OBJ_MAN_POINT
		|| obj.mType == ObjectType::OBJ_BLOCK_POINT) {
		drawCellAlphaTest(coord, Rect(static_cast<int>(TileID::IMG_POINT) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
	}
	else {
		drawCellAlphaTest(coord, Rect(static_cast<int>(TileID::IMG_FLOOR) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
	}
}

void Game::drawForeground(const Object& obj, const Coord& coord)
{
#if LOG
	GameLib::cout << "Game::drawForeground,obj(" << (int)obj.mType
		<< ",[" << obj.mCoord.mX << "," << obj.mCoord.mY << "]);coord(" 
		<< coord.mX << "," << coord.mY << ")." << GameLib::endl;

	GameLib::cout << "Game::drawForeground,mPerMoveFrames: " << (int)mPerMoveFrames << GameLib::endl;
#endif

	const int& x = coord.mX;
	const int& y = coord.mY;

	int dxStep = x * PER_CELL - obj.mCoord.mX * PER_CELL + obj.mCoord.mX * mPerMoveFrames;
	int dyStep = y * PER_CELL - obj.mCoord.mY * PER_CELL + obj.mCoord.mY * mPerMoveFrames;

	Vec2 pos(dxStep,dyStep);
	if (obj.mType == ObjectType::OBJ_MAN || obj.mType == ObjectType::OBJ_MAN_POINT) {
		//drawCellAlphaTest(coord, Rect(static_cast<int>(TileID::IMG_PLAYER) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
		drawCellAlphaTest(pos, Rect(static_cast<int>(TileID::IMG_PLAYER) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
	}
	else if (obj.mType == ObjectType::OBJ_BLOCK || obj.mType == ObjectType::OBJ_BLOCK_POINT) {
		//drawCellAlphaTest(coord, Rect(static_cast<int>(TileID::IMG_BLOCK) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
		drawCellAlphaTest(pos, Rect(static_cast<int>(TileID::IMG_BLOCK) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
	}
}