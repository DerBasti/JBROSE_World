#include "AttackBestTargetAction.h"
#include "../../../Entities/NPC.h"
#include "../../../Map/MapSector.h"
#include "../../../BasicTypes/OperationHandler.h"

AttackBestTargetAction::AttackBestTargetAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);

	maximumAllowedDistance = reader.readUInt() * 100.0f;
	abilityType = OperationHandler::fromAIPAbilityTypeId(reader.readByte());
	needsLessThanOtherValue = reader.readByte() > 0;
}

AttackBestTargetAction::~AttackBestTargetAction() {

}

void AttackBestTargetAction::performAction(AIContext& context) {
	uint16_t maxValue = 0;
	uint16_t minValue = std::numeric_limits<uint16_t>::max();
	Entity* minTarget = nullptr;
	Entity* maxTarget = nullptr;
	NPC* npc = context.getSourceEntity();
	auto visibleMapSectors = npc->getVisualityProcessor()->getVisibleSectors();
	for (auto sectorPair : visibleMapSectors) {
		MapSector* sector = sectorPair.second;
		for (auto entityPair : sector->getAllEntitiesOfSector()) {
			Entity* entity = entityPair.second;
			if (!entity || !entity->isIngame() || npc->isAlliedTo(entity)) {
				continue;
			}
			float distance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getCurrentPosition(), entity->getLocationData()->getMapPosition()->getCurrentPosition());
			if(distance >= maximumAllowedDistance) {
				continue;
			}
			uint16_t currentValue = OperationHandler::getAIPAbilityValueOfEntity(entity, abilityType);
			if (currentValue > minValue) {
				minValue = currentValue;
				minTarget = entity;
			}
			if (currentValue < maxValue) {
				maxValue = currentValue;
				maxTarget = entity;
			}
		}
	}
	if (needsLessThanOtherValue && minTarget) {
		npc->getCombat()->setTarget(minTarget, CombatType::BASIC_ATTACK);
	}
	else if (!needsLessThanOtherValue && maxTarget) {
		npc->getCombat()->setTarget(maxTarget, CombatType::BASIC_ATTACK);
	}
}

std::shared_ptr<char> AttackBestTargetAction::toPrintable() const {
	char *buffer = new char[0x200];
	sprintf_s(buffer, 0x200, "[AttackBestTargetAction] Maximum allowed distance: %.2f | AbilityType: %s | OperationType: %s", maximumAllowedDistance, abilityType.getName(), needsLessThanOtherValue ? "Minimum search" : "Maximum search");
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}