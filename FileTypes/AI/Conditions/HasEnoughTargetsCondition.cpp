#include "HasEnoughTargetsCondition.h"
#include "../../../Entities/Monster.h"
#include "../../../Map/MapSector.h"


HasEnoughTargetsCondition::HasEnoughTargetsCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader interpreter(datablock.get());
	interpreter.skipBytes(DEFAULT_HEADER_LENGTH);
	this->maxAllowedDistanceFromTarget = interpreter.readUInt() * 100.0f;
	this->alliedFlag = interpreter.readUShort() > 0;
	this->lowerLevelBoundry = static_cast<int16_t>(interpreter.readUShort());
	this->upperLevelBoundry = static_cast<int16_t>(interpreter.readUShort());
	this->maximumOfFoundEntitiesNecessary = interpreter.readUShort();
}

HasEnoughTargetsCondition::HasEnoughTargetsCondition(uint32_t operationCode, uint32_t length, const char* contextData) : AICondition(operationCode, length, contextData) {

}

bool HasEnoughTargetsCondition::isFulfilled(AIContext& context) {
	auto npc = context.getSourceEntity();
	uint32_t targetCount = 0;
	float lastNearestDistance = maxAllowedDistanceFromTarget;
	for (auto sectorPair : npc->getVisualityProcessor()->getVisibleSectors()) {
		MapSector* currentSector = sectorPair.second;
		auto allEntities = currentSector->getAllEntitiesOfSector();
		for (auto entityPair : allEntities) {
			auto currentEntity = entityPair.second;
			if (!currentEntity->isIngame() || currentEntity->isDrop() || currentEntity == npc) {
				continue;
			}
			float distance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getCurrentPosition(), currentEntity->getLocationData()->getMapPosition()->getCurrentPosition());
			int16_t levelDifference = npc->getStats()->getLevel() - currentEntity->getStats()->getLevel();
			if (distance <= maxAllowedDistanceFromTarget && lowerLevelBoundry >= levelDifference && upperLevelBoundry <= levelDifference && npc->isAlliedTo(currentEntity) == alliedFlag) {
				targetCount++;

				if (distance <= lastNearestDistance) {
					lastNearestDistance = distance;
					context.setNearestTarget(currentEntity);
				}
				if (targetCount >= maximumOfFoundEntitiesNecessary) {
					context.setLastFoundTarget(currentEntity);
					return true;
				}
			}
		}
	}
	return false;
}

std::shared_ptr<char> HasEnoughTargetsCondition::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[HasEnoughTargetsCondition] Maximum allowed distance: %.2f | Level difference boundries [%i, %i] | Check for allies: %i | Amount necessary: %i ", maxAllowedDistanceFromTarget, lowerLevelBoundry, upperLevelBoundry, alliedFlag, maximumOfFoundEntitiesNecessary);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}