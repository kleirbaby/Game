#pragma once
#include "noncopyable.h"
#include "global.h"
#include <string>
#include <fstream>

namespace Utils
{
	enum IOMode
	{ 
		NotOpen = 0x0000,
		ReadOnly = 0x0001,
		WriteOnly = 0x0002,
		Append = 0x0004,
		Truncate = 0x0008,
		Binary = 0x0010,
		Text = 0x00020,

		ReadWrite = ReadOnly | WriteOnly,
	};

	typedef int IOModes;

	class File : noncopyable
	{
	public:
		File(const char* fileName, IOModes mode);
		File(const String& fileName, IOModes mode);
		~File();

		bool isValid();
		unsigned long long size();
		int read(char* buffer, int bufferSize);
		int write(const char* buffer, int bufferSize);
	private:
		int mode2IOBase(IOModes mode);
		int mIOBase;
		std::fstream mFile;
	};
}

