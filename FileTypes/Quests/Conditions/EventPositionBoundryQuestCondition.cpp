#include "EventPositionBoundryQuestCondition.h"
#include "../../../WorldClient.h"

EventPositionBoundryQuestCondition::EventPositionBoundryQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	mapId = reader.readUInt();
	xMapTile = reader.readUInt();
	yMapTile = reader.readUInt();
	eventId = reader.readUInt();
	eventHash = reader.readUInt();
}
EventPositionBoundryQuestCondition::~EventPositionBoundryQuestCondition() {

}
bool EventPositionBoundryQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	return false;
}
std::shared_ptr<char> EventPositionBoundryQuestCondition::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[EventPositionBoundryQuestCondition] Map: %i, Event MapTile [%i, %i], EventId: %i", mapId, xMapTile, yMapTile, eventId);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}