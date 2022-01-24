#include "TeleportPlayerQuestReward.h"
#include "../../../WorldClient.h"
#include "../../../WorldServer.h"

TeleportPlayerQuestReward::TeleportPlayerQuestReward(std::shared_ptr<char>& rawData) : QuestReward(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	mapId = reader.readUInt();
	float x = static_cast<float>(reader.readUInt());
	float y = static_cast<float>(reader.readUInt());
	position = Position(x, y);
	includePartyFlag = reader.readByte() > 0;
}
TeleportPlayerQuestReward::~TeleportPlayerQuestReward() {

}

void TeleportPlayerQuestReward::performAction(QuestTriggerContext& context) {
	if (!context.getTriggerEntity()->isPlayer()) {
		return;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	auto worldServer = WorldServer::getInstance();
	Map* map = worldServer->getMapById(mapId);
	logger.logDebug("Teleporting Player '", player->getName(), "' as reward to ", map->getName(), " @(", position.getX(), ", ", position.getY(), ")");
	worldServer->teleportPlayer(player, map, position);
}

std::shared_ptr<char> TeleportPlayerQuestReward::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[TeleportPlayerQuestReward] MapId: %i, Position: (%.2f, %.2f), Include Party: %i", mapId, position.getX(), position.getY(), includePartyFlag);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}