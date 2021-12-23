#include "UpdateInventorySlotResponsePacket.h"
#include "../../BasicTypes/Item.h"

UpdateInventorySlotsResponsePacket::UpdateInventorySlotsResponsePacket() : ResponsePacket(ID) {

}
UpdateInventorySlotsResponsePacket::~UpdateInventorySlotsResponsePacket() {

}

void UpdateInventorySlotsResponsePacket::addItemToUpdate(uint8_t slotId, const Item& item) {
	slotIdUpdates.push_back(slotId);
	itemUpdates.push_back(item);
}

void UpdateInventorySlotsResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(static_cast<uint8_t>(itemUpdates.size()));
	for (size_t i = 0; i < itemUpdates.size(); i++) {
		const Item& item = itemUpdates.at(i);
		packet.addData(slotIdUpdates.at(i));
		packet.addData(ItemVisuality::toPacketHeader(item));
		packet.addData(ItemVisuality::toPacketBody(item));
	}
}

std::string UpdateInventorySlotsResponsePacket::toPrintable() const {
	char buf[0x250] = { 0x00 };
	sprintf_s(buf, "[UpdateInventorySlotsResponsePacket]\n\t* Amount of items to update: %i", itemUpdates.size());
	for (size_t i = 0; i < itemUpdates.size(); i++) {
		const Item& item = itemUpdates.at(i);
		sprintf_s(&buf[strlen(buf)], 0x250, "\n\t* Slot: %i | Item: %i, %i", slotIdUpdates.at(i), item.getType().getTypeId(), item.getId());
	}
	return std::string(buf);
}