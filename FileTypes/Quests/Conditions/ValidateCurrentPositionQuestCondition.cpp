#include "ValidateCurrentPositionQuestCondition.h"
#include "../../../WorldClient.h"
#include "../../../Map/Map.h"

ValidateCurrentPositionQuestCondition::ValidateCurrentPositionQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	mapId = static_cast<uint8_t>(reader.readUInt());
	float x = static_cast<float>(reader.readUInt());
	float y = static_cast<float>(reader.readUInt());
	uint32_t z = reader.readUInt();

	centerPosition = Position(x, y);

	maximumAllowedDistanceFromCenter = static_cast<float>(reader.readUInt());
}

ValidateCurrentPositionQuestCondition::~ValidateCurrentPositionQuestCondition() {

}

bool ValidateCurrentPositionQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	Entity* entity = context.getTriggerEntity();
	if (entity->getLocationData()->getMap()->getId() != mapId) {
		return false;
	}
	Position currentEntityPosition = context.getTriggerEntity()->getLocationData()->getMapPosition()->getCurrentPosition();
	return PositionProcessor::getDistanceBetweenPoints(currentEntityPosition, centerPosition) <= maximumAllowedDistanceFromCenter;
}

std::shared_ptr<char> ValidateCurrentPositionQuestCondition::toPrintable() const {
	char *buffer = new char[0xC0];
	sprintf_s(buffer, 0xC0, "[ValidateCurrentPositionQuestCondition] Needs to be on Map: %i | Position (%.2f, %.2f) | Maximum allowed distance: %.2f", mapId, centerPosition.getX(), centerPosition.getY(), maximumAllowedDistanceFromCenter);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}