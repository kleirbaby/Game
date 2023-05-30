#pragma once

#include <cassert>

template<typename T> class Array2D;

template<typename T>
class Array2DIterator
{
	int pos;
	Array2D<T>* array2D;
public:
	explicit Array2DIterator(Array2D<T>* t,int p):array2D(t),pos(p){}

	bool operator!=(const Array2DIterator& t)const
	{
		assert(array2D == t.array2D);
		return pos != t.pos;
	}

	const T& operator*()const
	{
		return (*array2D)(pos);
	}

	T& operator*()
	{
		return (*array2D)(pos);
	}

	const Array2DIterator& operator++()const
	{
		pos++;
		return (*this);
	}

	Array2DIterator& operator++()
	{
		pos++;
		return (*this);
	}
};

template<typename T> 
class Array2D
{
	friend class Array2DIterator<T>;

	T& operator()(int index)
	{
		return mArray[index];
	}

	const T& operator()(int index)const
	{
		return mArray[index];
	}

	T* mArray;
	bool mAllocFlag;
	int mSize;
	int mSize0;
	int mSize1;
public:
	using Iterator = Array2DIterator<T>;
	Array2D() 
		: mArray(nullptr),
		mAllocFlag(false),
		mSize0(0),
		mSize1(0)
	{
	}

	~Array2D() 
	{
		delete[] mArray;
		mArray = 0;  //为安全起见，把指针值设为0
	}

	void setSize(int size0, int size1)
	{
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[mSize0 * mSize1];
	}

	void size(int& size0,int& size1)const
	{
		size0 = mSize0;
		size1 = mSize1;
	}

	T& operator()(int index0, int index1) 
	{
		return operator()(index1 * mSize0 + index0);
	}

	const T& operator()(int index0, int index1) const
	{
		return operator()(index1 * mSize0 + index0);
	}

	Iterator begin()const
	{
		return Iterator(this,0);
	}

	Iterator begin()
	{
		return Iterator(this, 0);
	}

	Iterator end()const
	{
		return Iterator(this, mSize0*mSize1 - 1);
	}

	Iterator end()
	{
		return Iterator(this, mSize0 * mSize1 - 1);
	}

	void clear()
	{
		memset(mArray,0,sizeof(T) * mSize0 * mSize1);
	}
};