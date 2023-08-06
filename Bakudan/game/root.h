#pragma once

class State;
namespace Sequence
{
	class Root
	{
	public:
		Root();
		~Root();
	private:
		State* mState{ nullptr };
	};
}