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

std::shared_ptr<char> HasEnoughDamageReceivedCondition::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[HasEnoughDamageReceivedCondition] Damage amount required till trigger: %i", damageAmountNecessary);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}