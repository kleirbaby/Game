#pragma once

#include "global.h"
#include "array2d.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "vector2.h"
#include "rect.h"

namespace Utils
{
	class Image;
	//状态类
	class State
	{
	public:
		//该枚举值顺序要跟读取的图元顺序一致。
		enum class TileID { IMG_PLAYER, IMG_WALL, IMG_BLOCK, IMG_POINT, IMG_FLOOR};

		enum class ObjectType
		{
			OBJ_SPACE,
			OBJ_WALL,
			OBJ_POINT,
			OBJ_BLOCK,
			OBJ_BLOCK_POINT,
			OBJ_MAN,
			OBJ_MAN_POINT,

			OBJ_UNKNOWN,
		};

		struct Coord
		{
			Coord() {}
			Coord(int x, int y) :mX(x), mY(y) {}
			const Vec2 toVec()const
			{
				return Vec2(mX * 32, mY * 32);
			}

			int mX{ 0 };
			int mY{ 0 };
		};

		struct Object
		{
			Object() {}
			Object(ObjectType t) :type(t) {}

			ObjectType type = ObjectType::OBJ_UNKNOWN;
			bool flag = false;
		};

		State(const char* stageData, int size);
		void update();
		void draw();
		bool hasCleared() const;
		void loadTile();
		void test()const;
	private:
		bool parseMap(const char* stageData, int size);
		void drawPixel(const Coord& pos,unsigned color)const;
		void drawCell(const Coord& pos, const Vec2& size, unsigned color)const;
		void drawCell(const Coord& pos, const Rect& rect, const Image& img)const;
		void drawCellAlphaTest(const Coord& pos, const Rect& rect, const Image& img)const;
		void drawCellAlphaTest(const Vec2& pos, const Rect& rect, const Image& img)const;
		void drawCellAlphaBlend(const Coord& pos, const Rect& rect, const Image& img)const;

		//采用检测方式:只在前一帧没有按下才响应
		bool canMovePerOn(int input);
		//碰撞
		bool isCollisionWithPlayer(const Vec2& playerPos,int dirX,int dirY, ObjectType type);

		void initPlayer();

		void updateMap();
		void updatePlayer();
		void drawMap();
		void drawPlayer();

		int mWidth;
		int mHeight;
		Array2D<Object> mObjects;
		std::shared_ptr<Image> mImage;

		std::unordered_map<int, bool> mKeyStatus;
		uint32 mPerMoveFrames = 0;
		bool mUpdatePlayerCoord{ false };
		Vec2 mPlayerPos{ -1, -1 };
		int mDirX = 0;
		int mDirY = 0;
	};
}
