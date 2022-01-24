#include "AttackDesignatedTargetAction.h"
#include "../../../Entities/NPC.h"

void AttackDesignatedTargetAction::performAction(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	if (context.getDesignatedTarget() == nullptr) {
		return;
	}
	sourceNpc->getCombat()->setTarget(context.getDesignatedTarget(), CombatType::BASIC_ATTACK);
}

std::shared_ptr<char> AttackDesignatedTargetAction::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[AttackDesignatedTargetAction] Attack designated target.");
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}