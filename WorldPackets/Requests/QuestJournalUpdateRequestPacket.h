#ifndef __ROSE_QUEST_JOURNAL_UPDATE_REQUEST_PACKET__
#define __ROSE_QUEST_JOURNAL_UPDATE_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

enum class QuestJournalUpdateRequestAction : uint8_t {
	ADD_QUEST = 1,
	DELETE_QUEST,
	RUN_TRIGGER
};

class QuestJournalUpdateRequestPacket : public Packet {
private:
	QuestJournalUpdateRequestAction updateAction;
	uint8_t questSlot;
	uint32_t questHash;
public:
	const static uint32_t ID = 0x730;
	QuestJournalUpdateRequestPacket(const Packet* packet);
	virtual ~QuestJournalUpdateRequestPacket();

	__inline QuestJournalUpdateRequestAction getUpdateActionType() const {
		return updateAction;
	}
	__inline uint8_t getQuestJournalSlot() const {
		return questSlot;
	}
	__inline uint32_t getQuestHash() const {
		return questHash;
	}
};

#endif //__ROSE_QUEST_JOURNAL_UPDATE_REQUEST_PACKET__