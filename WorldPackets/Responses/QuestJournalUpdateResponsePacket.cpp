#include "QuestJournalUpdateResponsePacket.h"
#include "../../FileTypes/Quests/PlayerQuestJournal.h"

QuestJournalUpdateResponsePacket::QuestJournalUpdateResponsePacket(QuestJournalUpdateResult result) : ResponsePacket(ID) {
	resultType = result;
	journalSlot = -1;
	questHash = -1;
}

QuestJournalUpdateResponsePacket::~QuestJournalUpdateResponsePacket() {

}

void QuestJournalUpdateResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(static_cast<uint8_t>(resultType));
	packet.addData<uint8_t>(0x00);
	packet.addData(questHash);
}