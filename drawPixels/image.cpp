#include "image.h"
#include <cassert>
#include <memory>

using namespace Utils;

constexpr unsigned DDS_MAGIC = 0x20534444;
constexpr int DDS_HEADER_SIZE = 124;

static unsigned getUnsigned(const char*);

Image::Image()
	:mType(Type::Invalid),
	mWidth(0),
	mHeight(0),
	mData(nullptr)
{

}

Image::Image(const Image& img)
{
	if (&img != this) {
		mType = img.mType;
		mWidth = img.mWidth;
		mHeight = img.mHeight;

		mData = new unsigned[mWidth * mHeight];
		memcpy(mData,img.mData,mWidth * mHeight * sizeof(unsigned));
	}
}

Image& Image::operator=(const Image& img)
{
	if (&img != this) {
		mType = img.mType;
		mWidth = img.mWidth;
		mHeight = img.mHeight;

		delete[] mData;
		mData = new unsigned[mWidth * mHeight];
		memcpy(mData, img.mData, mWidth * mHeight * sizeof(unsigned));
	}

	return *this;
}

Image::~Image()
{
	delete[] mData;
}

bool Image::loadFile(File& file)
{
	if (!file.isValid()) {
		assert(0);
		return false;
	}

	auto size = file.size();

	std::unique_ptr<char[]> buffer(std::make_unique<char[]>((size_t)size));

	if (file.read(buffer.get(), size) < 0) {
		assert(0);
		return false;
	}

	if (!loadDDS32(buffer.get(),size)) {
		assert(0);
		return false;
	}

	return true;
}

int Image::width()const
{
	return mWidth;
}

int Image::heigth()const
{
	return mHeight;
}

const unsigned* Image::data()const
{
	return mData;
}

Image Image::part(const Rect& rect)const
{
	Image img;

	switch (mType){
	case Type::DDS32:
		img.mType = Type::DDS32;
		break;
	default:
		assert(0 && "Not suport!");
		return img;
	}

	img.mWidth = rect.mWidth;
	img.mHeight = rect.mHeight;

	if (rect.right() > mWidth) {
		img.mWidth = mWidth - rect.mX;
	}

	if (rect.bottom() > mHeight) {
		img.mHeight = mHeight - rect.mY;
	}

	img.mData = new unsigned[img.mWidth * img.mHeight];

	int nPixel = 0;
	for (int yPixel = rect.mY; yPixel < (rect.mY + img.mHeight); ++yPixel) {
		for (int xPixel = rect.mX; xPixel < (rect.mX + img.mWidth); ++xPixel) {
			img.mData[nPixel++] = mData[xPixel + yPixel * mWidth];
		}
	}

	return img;
}

bool Image::loadDDS32(const char* buffer, int size)
{
	//获取魔数
	auto dwMagic = getUnsigned(&(buffer[0]));
	//获取结构体大小
	auto dwSize = getUnsigned(&(buffer[4]));

	if (dwMagic != DDS_MAGIC) {
		return false;
	}

	if (dwSize != DDS_HEADER_SIZE) {
		return false;
	}

	mHeight = getUnsigned(&(buffer[12]));
	mWidth = getUnsigned(&(buffer[16]));
	mData = new unsigned[mHeight*mWidth];

	for (int i = 0; i < mHeight * mWidth; ++i) {
		mData[i] = getUnsigned(&buffer[128 + i * 4]);//&buffer[128] + i*4
	}

	mType = Type::DDS32;
	return true;
}

uint8 Image::getAlpha(unsigned* pixsel)const
{
	return (*pixsel & 0xff000000) >> 24;
}

unsigned getUnsigned(const char* p)
{
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(p);
	unsigned r = up[0];
	r |= up[1] << 8;
	r |= up[2] << 16;
	r |= up[3] << 24;
	return r;
}