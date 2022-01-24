#include "SelectQuestFromJournalCondition.h"
#include "../../../WorldClient.h"

SelectQuestFromJournalCondition::SelectQuestFromJournalCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);
	questId = reader.readUInt();
}

SelectQuestFromJournalCondition::~SelectQuestFromJournalCondition() {

}

bool SelectQuestFromJournalCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	Entity* triggerEntity = context.getTriggerEntity();
	if (!triggerEntity->isPlayer()) {
		logger.logWarn("Select Quest triggered by Entity: ", triggerEntity->getName());
		return false;
	}
	Player* player = dynamic_cast<Player*>(triggerEntity);
	PlayerQuest* quest = player->getQuestJournal()->findQuestById(questId);
	logger.logDebug("Trying to select quest with id: ", questId);
	context.setSelectedQuest(quest);
	return quest != nullptr;
}

std::shared_ptr<char> SelectQuestFromJournalCondition::toPrintable() const {
	char *buffer = new char[0x50];
	sprintf_s(buffer, 0x50, "[SelectQuestFromJournalCondition] Quest to select: %i", questId);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}