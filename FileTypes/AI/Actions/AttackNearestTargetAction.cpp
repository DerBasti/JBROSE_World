#include "AttackNearestTargetAction.h"
#include "../../../Entities/NPC.h"

AttackNearestTargetAction::AttackNearestTargetAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {

}
AttackNearestTargetAction::~AttackNearestTargetAction() {

}

void AttackNearestTargetAction::performAction(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	if (!context.getNearestTarget()) {
		return;
	}
	npc->getCombat()->setTarget(context.getNearestTarget(), CombatType::BASIC_ATTACK);
}

std::shared_ptr<char> AttackNearestTargetAction::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[AttackNearestTargetAction] Attack nearest found target.");
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}