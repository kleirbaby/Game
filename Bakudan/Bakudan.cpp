// Bakudan.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "GameLib/Framework.h"
#include <string>

static void mainLoop();
static void test();

namespace GameLib
{
	void Framework::update()
	{
		mainLoop();
	}
}

void mainLoop()
{
	test();
}

void test()
{
	//测试新的打印和随机数
	GameLib::Framework f = GameLib::Framework::instance();
	
	int dice = f.getRandom(10);

	std::string logStr = std::string("print: ") + std::to_string(dice);
	f.drawDebugString(10, 1, logStr.c_str(), 0xffff0000);
}