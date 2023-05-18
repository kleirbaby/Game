#pragma once

#include "array2d.h"
#include <functional>
#include <memory>
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
		State(const char* stageData, int size);
		void update(char input);
		void draw() const;
		bool hasCleared() const;
		void loadTile();
		void test()const;
	private:
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

		struct Object
		{
			Object() {}
			Object(ObjectType t) :type(t) {}

			ObjectType type = ObjectType::OBJ_UNKNOWN;
			bool flag = false;
		};

		bool parseMap(const char* stageData, int size);
		void drawCell(const Vec2& pos, const Vec2& size, unsigned color)const;
		void drawCell(const Vec2& pos, const Rect& rect, const Image& img)const;
		void drawCellAlphaTest(const Vec2& pos, const Rect& rect, const Image& img)const;
		void drawCellAlphaBlend(const Vec2& pos, const Rect& rect, const Image& img)const;

		int mWidth;
		int mHeight;
		Array2D<Object> mObjects;
		std::shared_ptr<Image> mImage;
	};
}
