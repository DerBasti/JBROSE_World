#include "RunQuestTriggerQuestReward.h"
#include "../../../WorldServer.h"

RunQuestTriggerQuestReward::RunQuestTriggerQuestReward(std::shared_ptr<char>& rawData) : QuestReward(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	uint16_t triggerNameLength = reader.readUShort();
	triggerName = reader.readStringWrapped(triggerNameLength);
	this->questHash = QSDFile::makeHash(triggerName.get());
}

RunQuestTriggerQuestReward::~RunQuestTriggerQuestReward() {

}

void RunQuestTriggerQuestReward::performAction(QuestTriggerContext& context) {
	auto record = WorldServer::getInstance()->getQuestRecordWithQuestHash(questHash);
	if (!record->conditionsFulfilled(context)) {
		return;
	}
	record->handleRewards(context);
}

std::shared_ptr<char> RunQuestTriggerQuestReward::toPrintable() const {
	char *buffer = new char[0x180];
	sprintf_s(buffer, 0x180, "[RunQuestTriggerQuestReward] TriggerName: %s (Hash: 0x%08x)", triggerName.get(), questHash);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}