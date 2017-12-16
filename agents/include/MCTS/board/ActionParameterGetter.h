#pragma once

#include <vector>

#include "state/State.h"
#include "MCTS/Types.h"
#include "MCTS/board/ActionChoices.h"
#include "judge/IActionParameterGetter.h"

namespace mcts
{
	class SOMCTS;

	namespace board
	{
		class ActionParameterGetter : public judge::IActionParameterGetter
		{
		public:
			ActionParameterGetter(SOMCTS & callback) : callback_(callback) {}

			void SetMainOp(FlowControl::MainOpType main_op) { main_op_ = main_op; }
			FlowControl::MainOpType ChooseMainOp() { return main_op_; }

			int GetNumber(ActionType::Types action_type, ActionChoices const& action_choices) final;

		private:
			SOMCTS & callback_;
			FlowControl::MainOpType main_op_;
		};
	}
}