#include "QuestDataResponsePacket.h"
#include "../../FileTypes/Quests/PlayerQuestJournal.h"

/*
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
*/

QuestDataResponsePacket::QuestDataResponsePacket(PlayerQuestJournal* journal) : ResponsePacket(ID, DEFAULT_LENGTH) {
	this->journal = journal;
}

QuestDataResponsePacket::~QuestDataResponsePacket() {

}

void QuestDataResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	for (uint8_t i = 0; i < 5; i++) {
		packet.addData(journal->getEpisodeVariableBySlot(i));
	}
	for (uint8_t i = 0; i < 3; i++) {
		packet.addData(journal->getJobVariableBySlot(i));
	}
	for (uint8_t i = 0; i < 7; i++) {
		packet.addData(journal->getPlanetVariableBySlot(i));
	}
	for (uint8_t i = 0; i < 10; i++) {
		packet.addData(journal->getUnionVariableBySlot(i));
	}

	for (uint8_t i = 0; i < 10; i++) {
		PlayerQuest* journalEntry = journal->getQuestByJournalSlot(i);
		if (journalEntry != nullptr) {
			packet.addData(journalEntry->getQuestId());
			packet.addData(journalEntry->getTimePassed());

			for (uint8_t j = 0; j < 10; j++) {
				packet.addData(journalEntry->getQuestVariable(j));
			}

			packet.addData(journalEntry->getQuestLever());

			for (uint8_t j = 0; j < journalEntry->getQuestInventory()->getMaxInventorySlots(); j++) {
				packet.addData(ItemVisuality::toPacketHeader(journalEntry->getQuestInventory()->getItem(j)));
				packet.addData(ItemVisuality::toPacketBody(journalEntry->getQuestInventory()->getItem(j)));
			}
		}
		else {
			packet.addData<uint16_t>(0x00);
			packet.addData<uint32_t>(0x00);

			for (uint8_t j = 0; j < 10; j++) {
				packet.addData<uint16_t>(0x00);
			}
			packet.addData<uint32_t>(0x00);

			for (uint8_t j = 0; j < 5; j++) {
				packet.addData<uint16_t>(0x00);
				packet.addData<uint32_t>(0x00);
			}
		}
	}
	for (uint8_t i = 0; i < 0x40; i++) {
		packet.addData(journal->getGlobalFlagAsByte(i));
	}
}