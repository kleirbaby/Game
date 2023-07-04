#pragma once
#include "state.h"

namespace Sequence
{
	class TitleState : public State
	{
	public:
		void update()override;
	};
}//Sequence