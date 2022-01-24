#include "Inventory.h"
#include "../WorldServer.h"

uint8_t PlayerInventory::findItemSlotForStacking(const Item& item, uint8_t tabId) {
	uint8_t slotStart = PlayerInventory::NON_EQUIPMENT_START_SLOT + (tabId * TAB_SIZE);
	uint8_t endSlot = slotStart + TAB_SIZE;
	for (uint8_t i = slotStart; i < endSlot; i++) {
		Item& inventoryItem = this->getItem(i);
		if (inventoryItem.getId() == item.getId() && inventoryItem.getType() == item.getType() && (inventoryItem.getAmount() + item.getAmount()) <= Inventory::MAXIMUM_AMOUNT_PER_STACK) {
			return i;
		}
	}
	return Inventory<>::INVALID_SLOT;
}


uint8_t PlayerInventory::findEmptyItemSlot(uint8_t tabId) {
	uint8_t slotStart = PlayerInventory::NON_EQUIPMENT_START_SLOT + (tabId * TAB_SIZE);
	uint8_t endSlot = slotStart + TAB_SIZE;
	for (uint8_t i = slotStart; i < endSlot; i++) {
		Item& inventoryItem = this->getItem(i);
		if (!inventoryItem.isValid()) {
			return i;
		}
	}
	return Inventory::INVALID_SLOT;
}


uint8_t PlayerInventory::getSlotForItem(const Item& item) {
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