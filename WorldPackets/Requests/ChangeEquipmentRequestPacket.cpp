#include "ChangeEquipmentRequestPacket.h"

ChangeEquipmentRequestPacket::ChangeEquipmentRequestPacket(const Packet *packet) : Packet(packet->getCommandId(), packet->getLength()) {
	LoadedDataReader interpreter(packet->getRawData());

	sourceSlotId = static_cast<uint8_t>(interpreter.readUShort());
	destinationSlotId = static_cast<uint8_t>(interpreter.readUShort());
}

ChangeEquipmentRequestPacket::~ChangeEquipmentRequestPacket() {

}

std::string ChangeEquipmentRequestPacket::toPrintable() const {
	char buf[0x100] = { 0x00 };
	sprintf_s(buf, "[ChangeEquipmentRequestPacket]\n\t* SourceSlot: %i\n\t* DestinationSlot: %i", sourceSlotId, destinationSlotId);
	return std::string(buf);
}