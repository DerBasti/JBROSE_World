#include "InventoryResponsePacket.h"
#include "..\..\WorldClient.h"

InventoryResponsePacket::InventoryResponsePacket(Inventory* inventory) : ResponsePacket(ID) {
	moneyAmount = inventory->getItem(0).getAmount();

	for (uint8_t i = 1; i < 140; i++) {
		const Item& item = inventory->getItem(i);
		itemHeader[i] = ItemVisuality::toPacketHeader(item);
		itemData[i] = ItemVisuality::toPacketBody(item);
	}
}

InventoryResponsePacket::~InventoryResponsePacket() {

}

void InventoryResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(moneyAmount);
	packet.addData<uint16_t>(0x00);
	packet.addData<uint32_t>(0x00);
	for (uint8_t i = 1; i < 140; i++) {
		packet.addData(itemHeader[i]);
		packet.addData(itemData[i]);
	}
}