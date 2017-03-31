#pragma once

#include "state/targetor/Targets.h"
#include "state/State.h"
#include "FlowControl/FlowContext.h"

namespace state {
	namespace targetor {
		inline void Targets::Fill(FlowControl::Manipulate & manipulate, std::vector<CardRef>& targets) const {
			Process(manipulate, [&](CardRef ref) {
				targets.push_back(ref);
			});
		}
		inline void Targets::Fill(FlowControl::Manipulate & manipulate, std::unordered_set<CardRef>& targets) const {
			Process(manipulate, [&](CardRef ref) {
				targets.insert(ref);
			});
		}

		template <typename Functor>
		inline void Targets::ForEach(FlowControl::Manipulate & manipulate, Functor&& func) const {
			Process(manipulate, [&](CardRef ref) {
				func(manipulate, ref);
			});
		}

		inline void Targets::Count(FlowControl::Manipulate & manipulate, int * count) const {
			Process(manipulate, [count](CardRef) {
				++(*count);
			});
		}

		template <typename Functor>
		inline void Targets::Process(FlowControl::Manipulate & manipulate, Functor&& functor) const {
			if (include_first) ProcessPlayerTargets(manipulate, manipulate.Board().FirstPlayer(), std::forward<Functor>(functor));
			if (include_second) ProcessPlayerTargets(manipulate, manipulate.Board().SecondPlayer(), std::forward<Functor>(functor));
		}

		template <typename Functor>
		inline void Targets::ProcessPlayerTargets(FlowControl::Manipulate & manipulate, board::Player const& player, Functor&& functor) const {
			if (include_hero) {
				if (player.GetHeroRef() != exclude) {
					functor(player.GetHeroRef());
				}
			}
			if (include_minion) {
				for (CardRef minion : player.minions_.Get()) {
					ProcessMinionTargets(manipulate, minion, std::forward<Functor>(functor));
				}
			}
		}

		template <typename Functor>
		inline void Targets::ProcessMinionTargets(FlowControl::Manipulate & manipulate, CardRef minion, Functor&& functor) const {
			if (minion == exclude) return;

			auto const& card = manipulate.Board().GetCard(minion);

			switch (minion_filter) {
			case kMinionFilterAll:
				break;
			case kMinionFilterNotMorallyWounded:
				if (card.GetHP() <= 0) return;
				break;
			case kMinionFilterTargetable:
				// TODO: check stealth
				// check owning player; stealth cannot be targeted by enemy
				break;
			case kMinionFilterTargetableBySpell:
				// TODO: check stealh
				// TODO: check immune spell
				break;
			case kMinionFilterMurloc:
				if (card.GetRace() == kCardRaceMurloc) break;
				return;
			}

			functor(minion);
		}
	}
}