#include "QuestJournalUpdateRequestPacket.h"


QuestJournalUpdateRequestPacket::QuestJournalUpdateRequestPacket(const Packet* packet) : Packet(packet->getCommandId(), packet->getLength()) {
	LoadedDataReader reader(packet->getRawData());
	
	updateAction = static_cast<QuestJournalUpdateRequestAction>(reader.readByte());
	questSlot = reader.readByte();
	questHash = reader.readUInt();
}

QuestJournalUpdateRequestPacket::~QuestJournalUpdateRequestPacket() {

}