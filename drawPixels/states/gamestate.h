#pragma once
#include "state.h"

namespace Sequence
{
	class GameState : public State
	{
	public:
		void update()override;
	};
}//Sequence