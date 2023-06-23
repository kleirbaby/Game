#include "GameLib/Framework.h"
#include "state.h"
#include <iostream>
#include <fstream>
#include <algorithm>

constexpr int MAX_PREVE_TIMES = 10;

using namespace std;

static void mainLoop();
static bool readFile(const string& file,char*& buffer,int& size);

//����ƽ��֡��
static int averageFps();

//�̶���ָ��֡��
static void fixFps(int fps);

namespace GameLib
{
	void Framework::update()
	{
		mainLoop();
	}
}

void mainLoop()
{
	fixFps(65);

	int fps = averageFps();
	GameLib::cout << "mainLoop,fps: " << fps << GameLib::endl;

	static Utils::State* state = nullptr;
	char* buffer = nullptr;
	int size = 0;

	if (!state) {
		if (!readFile("./stageData.txt", buffer, size)) {
			GameLib::cout << "readFile fail.";
			GameLib::Framework::instance().requestEnd();
			delete[] buffer;
			return;
		}
		
		GameLib::cout << "map data: \n" << buffer << GameLib::endl;
		
		state = new Utils::State(buffer, size);
		state->loadTile();
		delete[] buffer;

		state->draw();
		return;
	}

	state->update();

	state->draw();//������Ʋ��ܷŵ�cinǰ�棬��Ȼ�¼�һֱ�ȴ����룬���²��ܼ�ʱ�ߵ��¸�ѭ����Ҳ����Ⱦ��������

	bool bSuc = false;
	if (state->hasCleared()) {
		bSuc = true;
	}

	if (bSuc) {
		GameLib::Framework::instance().requestEnd();
		GameLib::cout << "You play suc!" << GameLib::endl;
	}

	if (GameLib::Framework::instance().isEndRequested()) {
		delete state;
	}
}

bool readFile(const string& file, char*& buffer, int& size)
{
	ifstream inputFile(file,ifstream::binary);

	if (!inputFile) {
		return false;
	}

	inputFile.seekg(0,ifstream::end);
	size = static_cast<int>(inputFile.tellg());

	inputFile.seekg(0,ifstream::beg);

	buffer = new char[size];
	inputFile.read(buffer, size);

	return true;
}

int averageFps()
{
	static unsigned prevTimes[MAX_PREVE_TIMES] = { 0 };
	unsigned curTime = GameLib::Framework::instance().time();

	unsigned overTimes = curTime - prevTimes[0];
	//�������10֡�����ʱ��
	for (int idx = 0; idx < MAX_PREVE_TIMES - 1; ++idx) {
		prevTimes[idx] = prevTimes[idx + 1];
	}

	prevTimes[MAX_PREVE_TIMES - 1] = curTime;

	//����ƽ��ֵ����
	int fps = (1000 * 10) / overTimes;
	return fps;
}

void fixFps(int fps)
{
	unsigned frameInterval = 1000 / fps; //ms

	static unsigned prevFrameTime = 0;
	unsigned curTime = 0;
	while (((curTime = GameLib::Framework::instance().time()) - prevFrameTime) < frameInterval) {
		GameLib::Framework::instance().sleep(1);
	}
	prevFrameTime = curTime;
}