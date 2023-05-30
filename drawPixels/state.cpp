#include "state.h"
#include "global.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdio>
#include "GameLib/Framework.h"
#include "image.h"
#include "file.h"
#include "rect.h"

using namespace std;
using namespace Utils;

constexpr int PER_FRAME_MOVE = 1;
constexpr int PER_CELL = 32;
constexpr int LIMIT_FRAMES = PER_CELL / PER_FRAME_MOVE;

State::State(const char* stageData, int size)
	:mWidth(0), mHeight(0)
{
	parseMap(stageData, size);
}

void State::update()
{
	int dx = 0, dy = 0;

	if (canMovePerOn('a')) {
		dx -= 1;
		mDirX = dx;
		mDirY = 0;
		mPerMoveFrames = 0;
		GameLib::cout << "dx: " << -1 << GameLib::endl;
	}
	else if (canMovePerOn('d')) {
		dx += 1;
		mDirX = dx;
		mDirY = 0;
		mPerMoveFrames = 0;
		GameLib::cout << "dx: " << 1 << GameLib::endl;
	}
	else if (canMovePerOn('w')) {
		dy -= 1;
		mDirX = 0;
		mDirY = dy;
		mPerMoveFrames = 0;
		GameLib::cout << "dy: " << -1 << GameLib::endl;
	}
	else if (canMovePerOn('s')) {
		dy += 1;
		mDirX = 0;
		mDirY = dy;
		mPerMoveFrames = 0;
		GameLib::cout << "dx: " << 1 << GameLib::endl;
	}
	
	mPerMoveFrames++;

	if (mPerMoveFrames > LIMIT_FRAMES) {
		return;
	}

	updatePlayer();
	updateMap();
}

void State::draw()
{
	if (!mImage) {
		assert(mImage);
		return;
	}

	drawMap();
	drawPlayer();

	//测试
	//test();
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

	File imgFile("nimotsuKunImage2.dds",IOMode::ReadOnly | IOMode::Binary);
	bool bLoadSuc = false;
	if (imgFile.isValid()) {
		bLoadSuc = mImage->loadFile(imgFile);
	}

	if (!bLoadSuc) {
		GameLib::cout << "State::loadTile,bLoadSuc == false!" << GameLib::endl;
		assert(bLoadSuc);
	}
}

void State::test()const
{
	auto testImg = std::make_shared<Image>();

	File imgFile("forgroundW.dds", IOMode::ReadOnly | IOMode::Binary);
	testImg->loadFile(imgFile);
	//drawCell(Coord(0, 0), Rect(0, 0, 128, 128), *testImg);
	drawCellAlphaBlend(Coord(0, 0), Rect(0, 0, 128, 128), *testImg);
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

	initPlayer();

	return true;
}

void State::drawPixel(const Coord& pos, unsigned color)const
{
	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	vram[pos.mY * windowWidth +pos.mX] = color;
}

void State::drawCell(const Coord& pos, const Vec2& size, unsigned color)const
{
	unsigned* vram = GameLib::Framework::instance().videoMemory();
	int windowWidth = GameLib::Framework::instance().width();

	for (int iY = 0; iY < size.mY; ++iY) {
		for (int iX = 0; iX < size.mX; ++iX) {
			vram[(iY + pos.mY * size.mY) * windowWidth + (iX + pos.mX * size.mX)] = color;
		}
	}
}

void State::drawCell(const Coord& pos, const Utils::Rect& rect, const Image& img)const
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

void State::drawCellAlphaTest(const Coord& pos, const Rect& rect, const Image& img)const
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

void State::drawCellAlphaTest(const Vec2& pos, const Rect& rect, const Image& img)const
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

void State::drawCellAlphaBlend(const Coord& pos, const Rect& rect, const Image& img)const
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

bool State::canMovePerOn(int input)
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

static const State::Coord findPlayer(const Array2D<State::Object>& objs)
{
	int x, y;

	int width = 0,height = 0;
	objs.size(width, height);

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			if (objs(x, y).type == State::ObjectType::OBJ_MAN
				|| objs(x, y).type == State::ObjectType::OBJ_MAN_POINT) {
				break;
			}
		}

		if (x < width) {
			break;
		}
	}

	if (x < width && y < height) {
		return State::Coord(x, y);
	}

	return State::Coord(-1,-1);
}

void State::initPlayer()
{
	const auto& coord = findPlayer(mObjects);
	mPlayerPos = coord.toVec();
}

void State::updateMap()
{
	if (mPerMoveFrames != LIMIT_FRAMES) {
		return;
	}

	int x = mPlayerPos.mX / PER_CELL;
	int y = mPlayerPos.mY / PER_CELL;

	int prevX = x - mDirX;
	int prevY = y - mDirY;

	Object& obj = mObjects(x, y);
	switch (obj.type) {
	case ObjectType::OBJ_SPACE:
		obj.type = ObjectType::OBJ_MAN;
		mObjects(prevX, prevY) = mObjects(prevX, prevY).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		break;
	case ObjectType::OBJ_WALL:
		break;
	case ObjectType::OBJ_POINT:
		obj.type = ObjectType::OBJ_MAN_POINT;
		mObjects(prevX, prevY) = mObjects(prevX, prevY).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		break;
	case ObjectType::OBJ_BLOCK: {
		if (mObjects(x + mDirX, x + mDirY).type == ObjectType::OBJ_SPACE) {
			mObjects(y + mDirX, y + mDirY).type = ObjectType::OBJ_BLOCK;
			obj.type = ObjectType::OBJ_MAN;
			mObjects(prevX, prevY) = mObjects(prevX, prevY).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
		}
		else if (mObjects(x + mDirX, y + mDirY).type == ObjectType::OBJ_POINT) {
			mObjects(x + mDirX, y + mDirY).type = ObjectType::OBJ_BLOCK_POINT;
			obj.type = ObjectType::OBJ_MAN;
			mObjects(prevX, prevY) = mObjects(prevX, prevY).type == ObjectType::OBJ_MAN ? ObjectType::OBJ_SPACE : ObjectType::OBJ_POINT;
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


void State::updatePlayer()
{
	int stepX = mDirX * PER_FRAME_MOVE;
	int stepY = mDirY * PER_FRAME_MOVE;

#if LOG
	GameLib::cout << "per move frames: " << (int)mPerMoveFrames << GameLib::endl;
	GameLib::cout << "player pos," << "x: " << mPlayerPos.mX << ",y: " << mPlayerPos.mY << GameLib::endl;
	GameLib::cout << "stepX: " << stepX << ",stepY: " << stepY << GameLib::endl;
#endif

	//预防穿墙(这里的碰撞检测需要考虑player的上下左右
	//比如与右侧墙壁相撞应该用右侧位置，与左边墙壁相撞应该用左边位置)
	auto nextPos = Vec2(mPlayerPos.mX + stepX, mPlayerPos.mY + stepY);
	int coordX = 0, coordY = 0;
	
	if (mDirX > 0) {
		coordX = (nextPos.mX + PER_CELL) / PER_CELL;
		coordY = nextPos.mY / PER_CELL;
	}

	if (mDirX < 0) {
		coordX = nextPos.mX / PER_CELL;
		coordY = nextPos.mY / PER_CELL;
	}

	if (mDirY > 0) {
		coordX = nextPos.mX / PER_CELL;
		coordY = (nextPos.mY + PER_CELL) / PER_CELL;
	}

	if (mDirY < 0) {
		coordX = nextPos.mX / PER_CELL;
		coordY = nextPos.mY / PER_CELL;
	}

	if (mObjects(coordX, coordY).type == ObjectType::OBJ_WALL) {
		return;
	}

	//预防穿过箱子

	mPlayerPos = Vec2(mPlayerPos.mX + stepX, mPlayerPos.mY + stepY);
}

void State::drawMap()
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			const Object& obj = mObjects(x, y);
#if LOG
			GameLib::cout << "(" << x << "," << y << ") = ";
			GameLib::cout << (int)mObjects(x, y).type << GameLib::endl;
#endif

			//先绘制墙壁和地板
			if (obj.type == ObjectType::OBJ_WALL) {
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_WALL) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
			}
			else {
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_FLOOR) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
			}

			//然后绘制箱子点人物这些表面目标
			switch (obj.type) {
			case ObjectType::OBJ_POINT:
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_POINT) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				break;
			case ObjectType::OBJ_BLOCK:
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_BLOCK) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				break;
			case ObjectType::OBJ_BLOCK_POINT:
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_POINT) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_BLOCK) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				break;
			case ObjectType::OBJ_MAN:
				//drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_PLAYER) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				break;
			case ObjectType::OBJ_MAN_POINT:
				drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_POINT) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				//drawCellAlphaTest(Coord(x, y), Rect(static_cast<int>(TileID::IMG_PLAYER) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
				break;
			default:
				break;
			}
		}
	}
}

void State::drawPlayer()
{
	drawCellAlphaTest(mPlayerPos, Rect(static_cast<int>(TileID::IMG_PLAYER) * PER_CELL, 0, PER_CELL, PER_CELL), *mImage);
}