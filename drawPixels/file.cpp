#include "file.h"
#include <cassert>

using namespace std;
using namespace Utils;

File::File(const char* fileName, IOModes mode)
	:mFile(fileName, mode2IOBase(mode))
{
}

File::File(const std::string& fileName, IOModes mode)
	:mFile(fileName, mode2IOBase(mode))
{
}

File::~File()
{
}

bool File::isValid()
{
	return mFile.good();
}

unsigned long long File::size()
{
	if (isValid()) {
		auto pos = mFile.tellg();
		mFile.seekg(0, ifstream::end);
		auto size = static_cast<unsigned long long>(mFile.tellg());
		mFile.seekg(pos);
		return size;
	}
	return 0;
}

int File::read(char* buffer, int bufferSize)
{
	if (!isValid()) {
		assert(0);
		return -1;
	}

	if (!buffer) {
		assert(buffer);
		return -2;
	}

	auto sz = size();
	mFile.read(buffer, bufferSize > sz?sz: bufferSize);

	return 0;
}

int File::write(const char* buffer, int bufferSize)
{
	if (!isValid()) {
		assert(0);
		return -1;
	}

	if (!buffer) {
		assert(buffer);
		return -2;
	}

	mFile.write(buffer, bufferSize);

	return 0;
}

int File::mode2IOBase(IOModes mode)
{
	int base = 0;

	if (mode & IOMode::ReadOnly) {
		base |= ios::in;
	}

	if (mode & IOMode::WriteOnly) {
		base |= ios::out;
	}

	if (mode & IOMode::Binary) {
		base |= ios::binary;
	}

	if (mode & IOMode::ReadWrite) {
		if (base & ios::in) {
			base |= ios::out;
		}
		else if (base & ios::out) {
			base |= ios::in;
		}
		else {
			base |= (ios::in | ios::out);
		}
	}

	return base;
}