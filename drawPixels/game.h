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
	class Game
	{
	public:
		//该枚举值顺序要跟读取的图元顺序一致。
		enum class TileID { IMG_PLAYER, IMG_WALL, IMG_BLOCK, IMG_POINT, IMG_FLOOR};

		enum class ObjectType
		{
			OBJ_FLOOR,
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

			int mX{ 0 };
			int mY{ 0 };
		};

		struct Object
		{
			Object() {}
			Object(ObjectType t):mType(t) {}
			Object(ObjectType t,const Coord& c):mType(t), mCoord(c){}

			void move(const Coord& dc)
			{
				mCoord.mX += dc.mX;
				mCoord.mY += dc.mY;
			}

			void move(ObjectType t, const Coord& dc)
			{
				mType = t;
				move(dc);
			}

			void move(int dx,int dy)
			{
				move(Coord(dx,dy));
			}

			void move(ObjectType t, int dx, int dy)
			{
				move(t, Coord(dx, dy));
			}

			ObjectType mType = ObjectType::OBJ_UNKNOWN;
			Coord mCoord;
		};

		Game(const char* stageData, int size);
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
		void drawBackground(const Object& obj,const Coord& coord);
		void drawForeground(const Object& obj, const Coord& coord);

		int mWidth;
		int mHeight;

		Array2D<Object> mObjects;
		std::shared_ptr<Image> mImage;

		std::unordered_map<int, bool> mKeyStatus;
		uint32 mPerMoveFrames = 0;
	};
}
