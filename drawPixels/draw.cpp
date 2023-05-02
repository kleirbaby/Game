#include "GameLib/Framework.h"
#include "state.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

static void mainLoop();
static bool readFile(const string& file,char*& buffer,int& size);

namespace GameLib
{
	void Framework::update()
	{
		mainLoop();
	}
}

void mainLoop()
{
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

	char input;
	GameLib::cin >> input;

	state->update(input);

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