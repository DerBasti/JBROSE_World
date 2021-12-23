#include "DropItemFromInventoryRequestPacket.h"


DropItemFromInventoryRequestPacket::DropItemFromInventoryRequestPacket(const Packet *packet) : Packet(packet->getCommandId(), packet->getLength()) {
	LoadedDataReader interpreter(packet->getRawData());

	slotId = interpreter.readByte();
	amount = interpreter.readUInt();
}

DropItemFromInventoryRequestPacket::~DropItemFromInventoryRequestPacket() {

}

std::string DropItemFromInventoryRequestPacket::toPrintable() const {
	char buf[0xA0] = { 0x00 };
	sprintf_s(buf, "[DropItemFromInventoryRequestPacket]\n\t* SlotId: %i\n\t* Amount: %i", slotId, amount);
	return std::string(buf);
}