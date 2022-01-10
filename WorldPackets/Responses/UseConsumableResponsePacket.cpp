#include "UseConsumableResponsePacket.h"

UseConsumableResponsePacket::UseConsumableResponsePacket(uint16_t itemUserLocalId, uint16_t consumableItemId) : ResponsePacket(ID) {
	this->itemUserLocalId = itemUserLocalId;
	this->consumableItemId = consumableItemId;
	this->inventorySlotId = 0;
}

UseConsumableResponsePacket::~UseConsumableResponsePacket() {

}

void UseConsumableResponsePacket::appendContentToSendable(SendablePacket& sendable) const {
	sendable.addData(itemUserLocalId);
	sendable.addData(consumableItemId);
	if (inventorySlotId != 0) {
		sendable.addData(inventorySlotId);
	}
}