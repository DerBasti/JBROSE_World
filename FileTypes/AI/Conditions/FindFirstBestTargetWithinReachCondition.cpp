#include "FindFirstBestTargetWithinReachCondition.h"
#include "../../../Entities/NPC.h"
#include "../../../Map/MapSector.h"

FindFirstBestTargetWithinReachCondition::FindFirstBestTargetWithinReachCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader interpreter(rawData.get());
	interpreter.skipBytes(DEFAULT_HEADER_LENGTH);

	maximumAllowedDistance = static_cast<float>(interpreter.readUInt() * 100.0f);
	allowedMinimumLevelDifference = interpreter.readUShort();
	allowedMaximumLevelDifference = interpreter.readUShort();
	checkForAllyFlag = interpreter.readByte() > 0;
}

FindFirstBestTargetWithinReachCondition::~FindFirstBestTargetWithinReachCondition() {

}

bool FindFirstBestTargetWithinReachCondition::isFulfilled(AIContext& context) {
	auto visibleSectors = context.getSourceEntity()->getVisualityProcessor()->getVisibleSectors();
	NPC* npc = context.getSourceEntity();
	for (auto sectorPair : visibleSectors) {
		MapSector* currentSector = sectorPair.second;
		std::function<bool(Entity*)> searchMethod = [npc, this](Entity* entity) {
			if (!entity || entity->isIngame() || entity->isDrop()) {
				return false;
			}
			int16_t levelDifference = npc->getStats()->getLevel() - entity->getStats()->getLevel();
			float distance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getCurrentPosition(), entity->getLocationData()->getMapPosition()->getCurrentPosition());
			return npc->isAlliedTo(entity) == checkForAllyFlag && allowedMinimumLevelDifference >= levelDifference && allowedMaximumLevelDifference <= levelDifference &&
				distance <= maximumAllowedDistance;
		};
		Entity* entity = currentSector->findFirstEntity(searchMethod);
		if (entity != nullptr) {
			context.setLastFoundTarget(entity);
			context.setNearestTarget(entity);
			return true;
		}
	}
	return false;
}

std::shared_ptr<char> FindFirstBestTargetWithinReachCondition::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[FindFirstBestTargetWithinReachCondition] Allowed maximum distance: %.2f | Level boundry differences: [%i,%i] | Needs ally: %i", maximumAllowedDistance, allowedMinimumLevelDifference, allowedMaximumLevelDifference, checkForAllyFlag);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}