#include "SelectGlobalNpcQuestCondition.h"
#include "../../../Entities/NPC.h"
#include "../../../WorldServer.h"

SelectGlobalNpcQuestCondition::SelectGlobalNpcQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	npcId = static_cast<uint16_t>(reader.readUInt());
}

SelectGlobalNpcQuestCondition::~SelectGlobalNpcQuestCondition() {

}

bool SelectGlobalNpcQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	NPC* npc = WorldServer::getInstance()->getGlobalNpcById(npcId);
	if (!npc) {
		logger.logWarn("NPC with ID ", npcId, " was not selectable!");
		return false;
	}
	context.setSelectedEntity(npc);
	return true;
}

std::shared_ptr<char> SelectGlobalNpcQuestCondition::toPrintable() const {
	char *buffer = new char[0x60];
	sprintf_s(buffer, 0x60, "[SelectGlobalNpcQuestCondition] NpcId: %i", npcId);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}