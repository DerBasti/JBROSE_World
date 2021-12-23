#include "PickupDropResponsePacket.h"

PickupDropResponsePacket::PickupDropResponsePacket(uint16_t localId) : ResponsePacket(ID) {
	this->localId = localId;
}

PickupDropResponsePacket::~PickupDropResponsePacket() {
	
}

void PickupDropResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(static_cast<uint16_t>(messageType));
	if (messageType == PickupDropMessageType::OKAY) {
		packet.addData(inventorySlot);
		packet.addData(ItemVisuality::toPacketHeader(itemToAdd));
		packet.addData(ItemVisuality::toPacketBody(itemToAdd));
	}
}

std::string PickupDropResponsePacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[PickupDropResponsePacket]\n\t* Picking up drop with local id: %i\n\t* MessageType: %i", localId, messageType);
	return std::string(buf);
}