#include "UpdateQuestJournalQuestReward.h"
#include "../../../WorldClient.h"

UpdateQuestJournalQuestReward::UpdateQuestJournalQuestReward(std::shared_ptr<char>& rawData) : QuestReward(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	questId = reader.readUInt();
	operation = static_cast<UpdateQuestJournalOperation>(reader.readUInt() & 0xFF);
}

UpdateQuestJournalQuestReward::~UpdateQuestJournalQuestReward() {

}

void UpdateQuestJournalQuestReward::performAction(QuestTriggerContext& context) {
	if (!context.getTriggerEntity()->isPlayer()) {
		return;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	switch (operation) {
		case UpdateQuestJournalOperation::REMOVE_QUEST:
			if (!player->getQuestJournal()->finishQuestWithQuestId(questId)) {
				logger.logWarn("Quest ", questId, " was not finishable for Player '", player->getName(), "'.");
				return;
			}
		break;
		case UpdateQuestJournalOperation::START_QUEST:
		{
			PlayerQuest* newQuest = new PlayerQuest(questId);
			newQuest->setQuestHash(getQuestHash());
			if (!player->getQuestJournal()->addQuestToJournal(newQuest)) {
				logger.logWarn("Quest ", questId, " was not addable for Player '", player->getName(), "'.");
				return;
			}
		}
		break;
		case UpdateQuestJournalOperation::REPLACE_QUEST_AND_KEEP_ITEMS:
		{
			if (context.getSelectedQuest() == nullptr) {
				logger.logWarn("Quest was not previously selected for Hash 0x", ROSELogger::asHex(getQuestHash()), " for Player ", player->getName());
				return;
			}
			PlayerQuest* selectedQuest = context.getSelectedQuest();
			selectedQuest->setQuestId(questId);
			selectedQuest->setQuestHash(getQuestHash());
			selectedQuest->setTimePassed(0);
		}
		break;
		case UpdateQuestJournalOperation::REPLACE_QUEST_AND_LOSE_ITEMS:
		{
			if (context.getSelectedQuest() == nullptr) {
				logger.logWarn("Quest was not previously selected for Hash 0x", ROSELogger::asHex(getQuestHash()), " for Player ", player->getName());
				return;
			}
			PlayerQuest* selectedQuest = context.getSelectedQuest();
			selectedQuest->setQuestId(questId);
			selectedQuest->setQuestHash(getQuestHash());
			selectedQuest->setTimePassed(0);
			QuestItemInventory* inventory = selectedQuest->getQuestInventory();
			for (uint8_t i = 0; i < inventory->getMaxInventorySlots(); i++) {
				inventory->getItem(i).clear();
			}
		}
		break;
		case UpdateQuestJournalOperation::SELECT_QUEST:
		{
			PlayerQuest* quest = player->getQuestJournal()->findQuestById(questId);
			if (!quest) {
				logger.logWarn("Quest ", questId, " was not selectable for Hash 0x", ROSELogger::asHex(getQuestHash()), " for Player ", player->getName());
				return;
			}
			context.setSelectedQuest(quest);
		}
		break;
	}
}

std::shared_ptr<char> UpdateQuestJournalQuestReward::toPrintable() const {
	char *buffer = new char[0x70];
	sprintf_s(buffer, 0x70, "[UpdateQuestJournalQuestReward] UpdateActionId: %i, QuestId: %i", operation, questId);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}