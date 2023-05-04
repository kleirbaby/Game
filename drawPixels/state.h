#pragma once

#include "array2d.h"
#include <functional>
#include <memory>
#include "vector2.h"
#include "rect.h"

namespace Utils
{
	class Image;
	//״̬��
	class State
	{
	public:
		//��ö��ֵ˳��Ҫ����ȡ��ͼԪ˳��һ�¡�
		enum class TileID { IMG_PLAYER, IMG_WALL, IMG_BLOCK, IMG_BLOCK_POINT, IMG_POINT };
		State(const char* stageData, int size);
		void update(char input);
		void draw() const;
		bool hasCleared() const;
		void loadTile();
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

		int mWidth;
		int mHeight;
		Array2D<Object> mObjects;
		std::shared_ptr<Image> mImage;
	};
}
