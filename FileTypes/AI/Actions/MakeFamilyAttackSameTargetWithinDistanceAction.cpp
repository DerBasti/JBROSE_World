#include "MakeFamilyAttackSameTargetWithinDistanceAction.h"
#include "../../../Entities/NPC.h"
#include "../../../Map/MapSector.h"


MakeFamilyAttackSameTargetWithinDistanceAction::MakeFamilyAttackSameTargetWithinDistanceAction(std::shared_ptr<char>& rawAction) : AIAction(rawAction) {
	LoadedDataReader interpreter(rawAction.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);

	maxDistanceAllowed = interpreter.readUInt() * 100.0f;
}

MakeFamilyAttackSameTargetWithinDistanceAction::~MakeFamilyAttackSameTargetWithinDistanceAction() {

}

void MakeFamilyAttackSameTargetWithinDistanceAction::performAction(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	auto target = sourceNpc->getCombat()->getTarget();
	if (target == nullptr) {
		return;
	}
	Entity* nearestTarget = context.getNearestTarget();
	for (auto entityPair : sourceNpc->getLocationData()->getCurrentMapSector()->getAllEntitiesOfSector()) {
		Entity* otherEntity = entityPair.second;
		if (!otherEntity->isMonster() || !otherEntity->isNPC()) {
			continue;
		}
		float distance = PositionProcessor::getDistanceBetweenPoints(otherEntity->getLocationData()->getMapPosition()->getCurrentPosition(), sourceNpc->getLocationData()->getMapPosition()->getCurrentPosition());
		NPC* otherNpc = dynamic_cast<NPC*>(otherEntity);
		if (sourceNpc->getDefaultStatValues()->getId() == otherNpc->getDefaultStatValues()->getId() &&
			sourceNpc->isAlliedTo(otherNpc) &&
			distance <= maxDistanceAllowed &&
			otherNpc->getCombat()->getTarget() == nullptr) {
			logger.logDebug(otherNpc->getName(), " (#", otherNpc->getLocationData()->getLocalId(), ") will attack '", target->getName(), "' now as helper.");
			otherNpc->getCombat()->setTarget(target, CombatType::BASIC_ATTACK);
		}
	}
}