#pragma once

namespace Utils
{
	template<typename T>
	class Vec2T
	{
	public:
		Vec2T();
		Vec2T(T x, T y);
		Vec2T(const Vec2T& v);
		Vec2T& operator=(const Vec2T& v);
		Vec2T<T>& operator+=(const Vec2T& v);
		Vec2T<T>& operator-=(const Vec2T& v);

		T mX;
		T mY;
	};

	template<typename T>
	Vec2T<T>::Vec2T()
		:mX(T()),mY(T())
	{
	}

	template<typename T>
	Vec2T<T>::Vec2T(T x, T y)
		:mX(x),mY(y)
	{
	}

	template<typename T>
	Vec2T<T>::Vec2T(const Vec2T<T>& v)
	{
		if (&v != this) {
			mX = v.mX;
			mY = v.mY;
		}
		return *this;
	}

	template<typename T>
	Vec2T<T>& Vec2T<T>::operator=(const Vec2T<T>& v)
	{
		if (&v != this) {
			mX = v.mX;
			mY = v.mY;
		}
		return *this;
	}

	template<typename T>
	Vec2T<T>& Vec2T<T>::operator+=(const Vec2T<T>& v)
	{
		return operator+(v);
	}

	template<typename T>
	Vec2T<T>& Vec2T<T>::operator-=(const Vec2T<T>& v)
	{
		return operator-(v);
	}

	template<typename T>
	inline Vec2T<T> operator+(const Vec2T<T>& v1,const Vec2T<T>& v2)
	{
		return Vec2T<T>(v1.mX + v2.mX,v1.mY + v2.mY);
	}

	template<typename T>
	inline Vec2T<T> operator-(const Vec2T<T>& v1, const Vec2T<T>& v2)
	{
		return Vec2T<T>(v1.mX - v2.mX, v1.mY - v2.mY);
	}

	typedef Vec2T<int> Vec2;
	typedef Vec2T<float> Vec2f;
}