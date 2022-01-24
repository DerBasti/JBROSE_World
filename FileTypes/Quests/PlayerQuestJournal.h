#ifndef __ROSE_PLAYER_QUEST_JOURNAL__
#define __ROSE_PLAYER_QUEST_JOURNAL__

#include <cinttypes>
#include <list>
#include "../../BasicTypes/Inventory.h"
#include "../../../JBROSE_Common/Timer.h"

class PlayerQuest {
private:
	const static uint8_t QUEST_VARIABLE_AMOUNT = 10;
	QuestItemInventory* questItemInventory;
	bool finishedFlag;
	uint16_t questId;
	uint32_t questHash;
	Timer timer;
	uint32_t leverVariable;
	uint16_t questVariables[QUEST_VARIABLE_AMOUNT];
	uint8_t questJournalSlot;
public:
	PlayerQuest() : PlayerQuest(0) {

	}
	PlayerQuest(uint16_t questId) {
		this->questId = questId;
		this->questHash = 0x00;
		finishedFlag = false;
		questJournalSlot = 0x00;
		memset(questVariables, 0x00, sizeof(uint16_t) * QUEST_VARIABLE_AMOUNT);
		leverVariable = 0x00;

		questItemInventory = new QuestItemInventory();
		timer.updateTimestamp();
	}
	virtual ~PlayerQuest() {
		delete questItemInventory;
	}
	__inline uint8_t getQuestJournalSlot() const {
		return questJournalSlot;
	}
	__inline void setQuestJournalSlot(const uint8_t slotId) {
		this->questJournalSlot = slotId;
	}
	__inline uint16_t getQuestId() const {
		return questId;
	}
	__inline void setQuestId(const uint16_t questId) {
		this->questId = questId;
	}
	__inline uint32_t getTimePassed() {
		return static_cast<uint32_t>(timer.getPassedTimeInMillis());
	}
	__inline void setTimePassed(uint32_t timePassedInMilliseconds) {
		timer.setTimestamp(Timer::getCurrentTimepointInMillis() - timePassedInMilliseconds);
	}
	__inline uint32_t getQuestHash() const {
		return questHash;
	}
	__inline void setQuestHash(uint32_t questHash) {
		this->questHash = questHash;
	}
	__inline uint32_t getQuestLever() {
		return leverVariable;
	}
	__inline void setQuestLever(uint32_t newLeverValue) {
		leverVariable = newLeverValue;
	}
	__inline uint16_t getQuestVariable(uint8_t slot) {
		return slot >= QUEST_VARIABLE_AMOUNT ? 0 : questVariables[slot];
	}
	__inline void setQuestVariable(uint8_t slot, uint16_t newValue) {
		if (slot >= QUEST_VARIABLE_AMOUNT) {
			return;
		}
		questVariables[slot] = newValue;
	}
	__inline QuestItemInventory* getQuestInventory() const {
		return questItemInventory;
	}
	__inline bool isFinished() const {
		return finishedFlag;
	}
	__inline void setFinished(bool flag) {
		finishedFlag = flag;
	}
};

class PlayerQuestJournal {
private:
	uint16_t episodeVariables[5];
	uint16_t jobVariables[3];
	uint16_t planetVariables[7];
	uint16_t unionVariables[10];
	uint8_t questFlags[0x40];

	std::list<PlayerQuest*> finishedQuests;
	PlayerQuest* activeQuests[10];
public:
	PlayerQuestJournal();
	virtual ~PlayerQuestJournal();

	bool addQuestToJournal(PlayerQuest* quest);
	void addToFinishedQuests(PlayerQuest* quest);
	
	bool finishQuestWithQuestId(uint32_t questId);

	bool deleteQuestWithQuestId(uint32_t questId);
	bool deleteQuestWithQuestHash(uint32_t questHash);
	PlayerQuest* findQuestById(const uint16_t id);
	PlayerQuest* findQuestByHash(const uint32_t questHash);

	__inline PlayerQuest* getQuestByJournalSlot(uint8_t slot) {
		return slot < 10 ? activeQuests[slot] : nullptr;
	}

	uint16_t getQuestVariableByTypeAndSlot(PlayerQuest* selectedQuestIfExistent, uint16_t variableType, uint8_t slot);

	__inline uint16_t getEpisodeVariableBySlot(uint8_t slot) const {
		return episodeVariables[slot];
	}
	__inline void setEpisodeVariableBySlot(uint16_t newValue, uint8_t slot) {
		episodeVariables[slot] = newValue;
	}
	__inline uint16_t getJobVariableBySlot(uint8_t slot) const {
		return jobVariables[slot];
	}
	__inline uint16_t getPlanetVariableBySlot(uint8_t slot) const {
		return planetVariables[slot];
	}
	__inline uint16_t getUnionVariableBySlot(uint8_t slot) const {
		return unionVariables[slot];
	}
	__inline uint8_t getGlobalFlagAsByte(uint8_t slot) const {
		return questFlags[slot];
	}
	__inline bool isGlobalFlagSet(uint32_t flagPosition) const {
		return (questFlags[(flagPosition / 8)] >> (flagPosition % 8)) & 1;
	}
	__inline void setGlobalFlag(uint32_t flagPosition, bool value) {
		questFlags[(flagPosition / 8)] = (questFlags[(flagPosition / 8)]) | (value << (flagPosition % 8));
	}
};

#endif //__ROSE_PLAYER_QUEST_JOURNAL__