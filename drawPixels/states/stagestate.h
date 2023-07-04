#pragma once
#include "state.h"

namespace Sequence
{
	class StageState : public State
	{
	public:
		void update()override;
	};
}//Sequence