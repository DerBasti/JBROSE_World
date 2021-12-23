#include "AttackDesignatedTargetAction.h"
#include "../../../Entities/NPC.h"

void AttackDesignatedTargetAction::performAction(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	if (context.getDesignatedTarget() == nullptr) {
		return;
	}
	sourceNpc->getCombat()->setTarget(context.getDesignatedTarget(), CombatType::BASIC_ATTACK);
}