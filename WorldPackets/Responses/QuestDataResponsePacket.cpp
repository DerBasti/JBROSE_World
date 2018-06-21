#include "QuestDataResponsePacket.h"

QuestDataResponsePacket::QuestDataResponsePacket() : ResponsePacket(ID, DEFAULT_LENGTH) {
	for (uint8_t i = 0; i < 10; i++) {
		if (i < 3) {
			jobVars[i] = 0;
		}
		if (i < 5) {
			episodeVars[i] = 0;
		}
		if (i < 7) {
			planetVars[i] = 0;
		}
		unionVars[i] = 0;
		globalQuestFlag[i] = 0;
	}
}

QuestDataResponsePacket::~QuestDataResponsePacket() {

}

void QuestDataResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	const std::function<void(const uint16_t*, uint8_t)> arrayIterator = [&](const uint16_t *dataArray, uint8_t amount) {
		for (uint8_t i = 0; i < amount; i++) {
			packet.addData(dataArray[i]);
		}
	};

	arrayIterator(episodeVars, 5);
	arrayIterator(jobVars, 3);
	arrayIterator(planetVars, 7);
	arrayIterator(unionVars, 10);

	for (uint8_t i = 0; i < 10; i++) {
		const QuestJournalEntry& journalEntry = entry[i];
		packet.addData(journalEntry.getId());
		packet.addData(journalEntry.getTimePassed());

		for (uint8_t j = 0; j < QuestJournalEntry::AMOUNT_OF_QUESTVARS; j++) {
			packet.addData(journalEntry.getQuestVar(j));
		}

		packet.addData(journalEntry.getLeverBits());

		for (uint8_t j = 0; j < QuestJournalEntry::AMOUNT_OF_ITEMS; j++) {
			packet.addData(journalEntry.getItemHeader(j));
			packet.addData(journalEntry.getItemData(j));
		}
	}
	for (uint8_t i = 0; i < 10; i++) {
		packet.addData(globalQuestFlag[i]);
	}
}