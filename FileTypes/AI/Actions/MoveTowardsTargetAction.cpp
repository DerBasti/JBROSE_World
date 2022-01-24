#include "MoveTowardsTargetAction.h"
#include "../../../Entities/NPC.h"

MoveTowardsTargetAction::MoveTowardsTargetAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);

	distanceToTarget = reader.readUInt() * 100.0f;
	stanceType = reader.readByte();
}

MoveTowardsTargetAction::~MoveTowardsTargetAction() {

}

void MoveTowardsTargetAction::performAction(AIContext& context) {
	NPC *npc = context.getSourceEntity();
	Entity* target = npc->getCombat()->getTarget();
	if (!target || !target->isIngame()) {
		return;
	}

	Position randomPosition = PositionProcessor::generateRandomPointAroundPosition(target->getLocationData()->getMapPosition()->getCurrentPosition(), distanceToTarget);
	npc->getStance()->setStanceById(stanceType);
	npc->getLocationData()->getMapPosition()->setDestinationPositionVisually(std::move(randomPosition));
}

std::shared_ptr<char> MoveTowardsTargetAction::toPrintable() const {
	char *buffer = new char[0x90];
	sprintf_s(buffer, 0x90, "[MoveTowardsTargetAction] Maximum distance to target: %.2f | Stance: %i", distanceToTarget, stanceType);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}