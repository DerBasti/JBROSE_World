#include "PlayerQuestJournal.h"
#include "../QSD.h"

PlayerQuestJournal::PlayerQuestJournal() {
	for (uint32_t i = 0; i < 0x40; i++) {
		if (i < 3) {
			jobVariables[i] = 0;
		}
		if (i < 5) {
			episodeVariables[i] = 0;
		}
		if (i < 7) {
			planetVariables[i] = 0;
		}
		if (i < 10) {
			unionVariables[i] = 0x00;
		}
		questFlags[i] = 0x00;
	}

	for (uint8_t i = 0; i < 10; i++) {
		activeQuests[i] = nullptr;
	}
}

PlayerQuestJournal::~PlayerQuestJournal() {
	finishedQuests.clear();
	for (uint8_t i = 0; i < 10; i++) {
		delete activeQuests[i];
		activeQuests[i] = nullptr;
	}
}

bool PlayerQuestJournal::addQuestToJournal(PlayerQuest* quest) {
	for (uint8_t i = 0; i < 10; i++) {
		if (activeQuests[i] == nullptr) {
			activeQuests[i] = quest;
			quest->setQuestJournalSlot(i);
			return true;
		}
	}
	return false;
}

uint16_t PlayerQuestJournal::getQuestVariableByTypeAndSlot(PlayerQuest* selectedQuest, uint16_t variableType, uint8_t slot) {
	uint16_t result = 0xFFFF;
	switch (variableType) {
		case 0x0000:
			if (selectedQuest != nullptr) {
				result = selectedQuest->getQuestVariable(static_cast<uint8_t>(slot));
			}
		break;
		case 0x0100:
			if (selectedQuest != nullptr) {
				result = static_cast<uint8_t>(selectedQuest->getQuestLever() >> (slot * 8));
			}
		break;
		case 0x0200:
			if (selectedQuest != nullptr) {
				result = static_cast<uint16_t>(selectedQuest->getTimePassed() / 1000);
			}
		break;
		case 0x0300:
			result = getEpisodeVariableBySlot(slot);
		break;
		case 0x0400:
			result = getJobVariableBySlot(slot);
		break;
		case 0x500:
			result = getPlanetVariableBySlot(slot);
		break;
		case 0x600:
			result = getUnionVariableBySlot(slot);
		break;
	}
	return result;
}

void PlayerQuestJournal::addToFinishedQuests(PlayerQuest* quest) {
	finishedQuests.push_back(quest);
}

bool PlayerQuestJournal::finishQuestWithQuestId(uint32_t questId) {
	PlayerQuest* questToDelete = findQuestById(questId);
	if (questToDelete != nullptr) {
		finishedQuests.push_back(questToDelete);
		activeQuests[questToDelete->getQuestJournalSlot()] = nullptr;

		questToDelete->setQuestJournalSlot(-1);
		questToDelete->setFinished(true);
		return true;
	}
	return false;
}

bool PlayerQuestJournal::deleteQuestWithQuestId(uint32_t questId) {
	PlayerQuest* questToDelete = findQuestById(questId);
	bool questEntryFound = questToDelete != nullptr;
	if (questEntryFound) {
		activeQuests[questToDelete->getQuestJournalSlot()] = nullptr;

		delete questToDelete;
		questToDelete = nullptr;
	}
	return questEntryFound;
}

bool PlayerQuestJournal::deleteQuestWithQuestHash(uint32_t questHash) {
	PlayerQuest* questToDelete = findQuestByHash(questHash);
	bool questEntryFound = questToDelete != nullptr;
	if (questEntryFound) {
		activeQuests[questToDelete->getQuestJournalSlot()] = nullptr;

		delete questToDelete;
		questToDelete = nullptr;
	}
	return questEntryFound;
}

PlayerQuest* PlayerQuestJournal::findQuestById(const uint16_t id) {
	for (uint8_t i = 0; i < 10; i++) {
		if (activeQuests[i] != nullptr && activeQuests[i]->getQuestId() == id) {
			return activeQuests[i];
		}
	}
	return nullptr;
}

PlayerQuest* PlayerQuestJournal::findQuestByHash(const uint32_t questHash) {
	for (uint8_t i = 0; i < 10; i++) {
		if (activeQuests[i] != nullptr && activeQuests[i]->getQuestHash() == questHash) {
			return activeQuests[i];
		}
	}
	return nullptr;
}