#include "ChangeEquipmentResponsePacket.h"
#include "../../WorldClient.h"

ChangeEquipmentResponsePacket::ChangeEquipmentResponsePacket(Player* player, uint8_t inventorySlot) : ResponsePacket(ID) {
	localId = player->getLocationData()->getLocalId();
	inventorySlotId = inventorySlot;
	itemVisualityBytes = ItemVisuality::toVisualityBytes(player->getInventory()->getItem(inventorySlot));
	movementSpeed = player->getStats()->getMovementSpeed();
}

ChangeEquipmentResponsePacket::~ChangeEquipmentResponsePacket() {

}


void ChangeEquipmentResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(inventorySlotId);
	packet.addData(itemVisualityBytes);
	packet.addData(movementSpeed);
}

std::string ChangeEquipmentResponsePacket::toPrintable() const {
	char buf[0x150] = { 0x00 };
	sprintf_s(buf, "[ChangeEquipmentResponsePacket]\n\t* LocalId: %i\n\t* InventorySlot: %i\n\t* Visuality: 0x%08x", localId, inventorySlotId, itemVisualityBytes);
	return std::string(buf);
}