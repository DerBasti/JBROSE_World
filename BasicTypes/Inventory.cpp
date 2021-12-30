#include "Inventory.h"
#include "../WorldServer.h"

uint8_t Inventory::getSlotForItem(const Item& item) {

	if (!item.isValid()) {
		return Inventory::INVALID_SLOT;
	}
	if (item.getType() == ItemTypeList::MONEY) {
		return 0;
	}

	uint8_t tabId = 0;
	if (item.getType() == ItemTypeList::CONSUMABLE) {
		tabId = 1;
	}
	else if (item.getType() == ItemTypeList::JEWELS || item.getType() == ItemTypeList::OTHER) {
		tabId = 2;
	}
	else if (item.getType() == ItemTypeList::PAT) {
		tabId = 3;
	}

	uint8_t foundSlot = Inventory::INVALID_SLOT;
	if (item.isStackable()) {
		foundSlot = findItemSlotForStacking(item, tabId);
		if (foundSlot == Inventory::INVALID_SLOT) {
			foundSlot = findEmptyItemSlot(tabId);
		}
	}
	else {
		foundSlot = findEmptyItemSlot(tabId);
	}
	return foundSlot;
}

uint8_t Inventory::addItemToInventory(const Item& item) {
	uint8_t slot = getSlotForItem(item);
	if (slot >= 0 && slot <= Inventory::MAX_SLOTS) {
		if (item.isStackable()) {
			if (this->inventorySlots[slot].isValid()) {
				this->inventorySlots[slot].addAmount(item.getAmount());
			}
			else {
				this->inventorySlots[slot] = item;
			}
		}
		else {
			this->inventorySlots[slot] = item;
		}
	}
	return slot;
}

uint16_t Inventory::getTotalWeight() const {
	uint16_t totalWeight = 0;
	for (uint8_t i = 1; i < Inventory::MAX_SLOTS; i++) {
		const Item& item = inventorySlots[i];
		if (item.isValid()) {
			ItemType type = item.getType();
			uint16_t currentItemWeight = 0;
			if (type == ItemTypeList::CONSUMABLE) {
				WorldServer::getInstance()->getConsumeSTB()->getWeightOfEntry(item.getId()) * item.getAmount();
			}
			else {
				WorldServer::getInstance()->getEquipmentSTB(item.getType().getTypeId())->getWeightOfEntry(item.getId()) * item.getAmount();
			}
			totalWeight += currentItemWeight;
		}
	}
	return totalWeight;
}


uint8_t Inventory::findItemSlotForStacking(const Item& item, uint8_t tabId) {
	uint8_t slotStart = Inventory::NON_EQUIPMENT_START_SLOT + (tabId * TAB_SIZE);
	uint8_t endSlot = slotStart + TAB_SIZE;
	for (uint8_t i = slotStart; i < endSlot; i++) {
		Item& inventoryItem = this->inventorySlots[i];
		if (inventoryItem.getId() == item.getId() && inventoryItem.getType() == item.getType() && (inventoryItem.getAmount() + item.getAmount()) <= Inventory::MAXIMUM_AMOUNT_PER_STACK) {
			return i;
		}
	}
	return Inventory::INVALID_SLOT;
}

uint8_t Inventory::findEmptyItemSlot(uint8_t tabId) {
	uint8_t slotStart = Inventory::NON_EQUIPMENT_START_SLOT + (tabId * TAB_SIZE);
	uint8_t endSlot = slotStart + TAB_SIZE;
	for (uint8_t i = slotStart; i < endSlot; i++) {
		Item& inventoryItem = this->inventorySlots[i];
		if (!inventoryItem.isValid()) {
			return i;
		}
	}
	return Inventory::INVALID_SLOT;
}

bool Inventory::reduceAmountOfItemInSlot(uint8_t slotId, uint32_t amount) {
	Item& item = inventorySlots[slotId];
	if (!item.isValid() || amount > item.getAmount()) {
		return false;
	}
	item.setAmount(item.getAmount() - amount);
	if (item.getAmount() <= 0) {
		clearItem(slotId);
	}
	return true;
}