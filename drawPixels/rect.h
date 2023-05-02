#pragma once

namespace Utils
{
	template<typename T>
	class RectT
	{
	public:
		RectT();
		RectT(T x, T y, T w, T h);
		T left()const;
		T right()const;
		T top()const;
		T bottom()const;

		T mX;
		T mY;
		T mWidth;
		T mHeight;
	};

	template<typename T>
	RectT<T>::RectT()
		:mX(T()),mY(T()),mWidth(T()),mHeight(T())
	{
	}

	template<typename T>
	RectT<T>::RectT(T x, T y, T w, T h)
		:mX(x),mY(y),mWidth(w),mHeight(h)
	{
	}

	template<typename T>
	T RectT<T>::left()const
	{
		return mX;
	}

	template<typename T>
	T RectT<T>::right()const
	{
		return mX + mWidth;
	}

	template<typename T>
	T RectT<T>::top()const
	{
		return mY;
	}

	template<typename T>
	T RectT<T>::bottom()const
	{
		return mY + mHeight;
	}

	typedef RectT<int> Rect;
	typedef RectT<float> RectF;
}

