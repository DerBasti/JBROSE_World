#include "InventoryResponsePacket.h"
#include "..\..\WorldClient.h"

InventoryResponsePacket::InventoryResponsePacket(Inventory* inventory) : ResponsePacket(ID) {
	moneyAmount = 100;

	for (uint8_t i = 0; i < 140; i++) {
		const Item& item = inventory->getItem(i);
		itemHeader[i] = ItemVisuality::toPacketHeader(item);
		itemData[i] = ItemVisuality::toPacketBody(item);
	}
}

InventoryResponsePacket::~InventoryResponsePacket() {

}

void InventoryResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(moneyAmount);

	for (uint8_t i = 0; i < 140; i++) {
		packet.addData(itemHeader[i]);
		packet.addData(itemData[i]);
	}
}