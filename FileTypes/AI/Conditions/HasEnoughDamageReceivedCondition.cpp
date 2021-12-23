#include "HasEnoughDamageReceivedCondition.h"
#include "../../../Entities/NPC.h"

bool HasEnoughDamageReceivedCondition::isFulfilled(AIContext& context) {
	if (damageDealtOnEnemyFlag) {
		return false;
	}
	NPC* npc = context.getSourceEntity();
	uint32_t damageReceived = npc->getStats()->getMaxHp() - npc->getStats()->getCurrentHp();
	return damageReceived >= damageAmountNecessary;
}