#include "AttackFoundTargetAction.h"
#include "../../../Entities/NPC.h"

AttackFoundTargetAction::AttackFoundTargetAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {

}

AttackFoundTargetAction::~AttackFoundTargetAction() {

}

void AttackFoundTargetAction::performAction(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	if (!context.getLastFoundTarget()) {
		return;
	}
	npc->getCombat()->setTarget(context.getLastFoundTarget(), CombatType::BASIC_ATTACK);
}

std::shared_ptr<char> AttackFoundTargetAction::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[AttackFoundTargetAction] Attack previously found target.");
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}