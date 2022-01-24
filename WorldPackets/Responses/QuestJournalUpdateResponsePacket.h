#ifndef __ROSE_QUEST_JOURNAL_UPDATE_RESPONSE_PACKET__
#define __ROSE_QUEST_JOURNAL_UPDATE_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"

enum class QuestJournalUpdateResult : uint8_t {
	ADD_SUCCESSFUL = 1,
	ADD_FAILED,
	DELETE_SUCCESSFUL,
	DELETE_FAILED,
	TRIGGER_RUN_SUCCESSFUL,
	TRIGGER_RUN_FAILED
};

class QuestJournalUpdateResponsePacket : public ResponsePacket {
private:
	QuestJournalUpdateResult resultType;
	uint8_t journalSlot;
	uint32_t questHash;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x730;
	QuestJournalUpdateResponsePacket(QuestJournalUpdateResult result);
	virtual ~QuestJournalUpdateResponsePacket();

	__inline void setQuestJournalSlot(uint8_t slot) {
		journalSlot = slot;
	}
	__inline void setQuestHash(uint32_t questHash) {
		this->questHash = questHash;
	}
};

#endif //__ROSE_QUEST_JOURNAL_UPDATE_RESPONSE_PACKET__