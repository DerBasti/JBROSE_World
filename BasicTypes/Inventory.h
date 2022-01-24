#ifndef __ROSE_INVENTORY__
#define __ROSE_INVENTORY__

#include "Item.h"
#include <map>

template<uint8_t INVENTORY_AMOUNT = 140>
class Inventory {
public:
	const static uint16_t MAXIMUM_AMOUNT_PER_STACK = 999;
	const static uint8_t INVALID_SLOT = -1;
private:
	Item inventorySlots[INVENTORY_AMOUNT];
	uint8_t weightPercentage;

protected:
	virtual uint8_t findItemSlotForStacking(const Item& itemToStack, uint8_t tabId) {
		for (uint8_t i = 0; i < getMaxInventorySlots(); i++) {
			Item& inventoryItem = this->getItem(i);
			if (inventoryItem.getId() == itemToStack.getId() && inventoryItem.getType() == itemToStack.getType() && (inventoryItem.getAmount() + itemToStack.getAmount()) <= Inventory::MAXIMUM_AMOUNT_PER_STACK) {
				return i;
			}
		}
		return Inventory<>::INVALID_SLOT;
	}
	virtual uint8_t findEmptyItemSlot(uint8_t tabId) {
		for (uint8_t i = 0; i < getMaxInventorySlots(); i++) {
			Item& inventoryItem = this->getItem(i);
			if (!inventoryItem.isValid()) {
				return i;
			}
		}
		return Inventory<>::INVALID_SLOT;
	}
public:
	Inventory() {
	}
	virtual ~Inventory() {}

	const Item& operator[](uint8_t slotId) const {
		return inventorySlots[slotId];
	}
	virtual std::map<uint8_t, Item> getSameItemsInInventory(const Item& item) {
		if (!item.isValid()) {
			return std::map<uint8_t, Item>();
		}
		std::map<uint8_t, Item> resultMap;
		for (uint8_t i = 0; i < getMaxInventorySlots(); i++) {
			const Item& inventoryItem = getItem(i);
			if (inventoryItem.getType() == item.getType() && inventoryItem.getId() == item.getId()) {
				resultMap.insert(std::make_pair(i, inventoryItem));
			}
		}
		return resultMap;
	}
	virtual uint8_t getSlotForItem(const Item& item) {
		if (!item.isValid()) {
			return Inventory::INVALID_SLOT;
		}
		uint8_t tabId = 0;
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

	uint8_t addItemToInventory(const Item& item) {
		uint8_t slot = getSlotForItem(item);
		if (slot >= 0 && slot <= getMaxInventorySlots()) {
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

	bool reduceAmountOfItemInSlot(uint8_t slotId, uint32_t amount) {
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
	uint16_t getTotalWeight() const {
		uint16_t totalWeight = 0;
		for (uint8_t i = 1; i < getMaxInventorySlots(); i++) {
			const Item& item = inventorySlots[i];
			if (item.isValid()) {
				ItemType type = item.getType();
				uint16_t currentItemWeight = 0;
				if (type == ItemTypeList::CONSUMABLE) {
					currentItemWeight = WorldServer::getInstance()->getConsumeSTB()->getWeightOfEntry(item.getId()) * item.getAmount();
				}
				else {
					currentItemWeight = WorldServer::getInstance()->getEquipmentSTB(item.getType().getTypeId())->getWeightOfEntry(item.getId()) * item.getAmount();
				}
				totalWeight += currentItemWeight;
			}
		}
		return totalWeight;
	}

	__inline uint8_t getMaxInventorySlots() const {
		return INVENTORY_AMOUNT;
	}

	__inline Item& getItem(uint8_t slot) {
		return inventorySlots[slot];
	}

	__inline const Item& getItem(uint8_t slot) const {
		return inventorySlots[slot];
	}

	__inline void setItem(const uint8_t slot, const Item& item) {
		inventorySlots[slot] = item;
	}

	__inline bool isSlotNotInUse(const uint8_t slotId) {
		return isValidSlot(slotId) && !getItem(slotId).isValid();
	}

	__inline void clearItem(const uint8_t slot) {
		inventorySlots[slot] = Item();
	}

	__inline bool isValidSlot(const uint8_t slot) {
		return slot >= 0 && slot < INVENTORY_AMOUNT;
	}
};

typedef Inventory<5> QuestItemInventory;

class PlayerInventory : public Inventory<> {
protected:
	virtual uint8_t findItemSlotForStacking(const Item& itemToStack, uint8_t tabId);
	virtual uint8_t findEmptyItemSlot(uint8_t tabId);
public:
	const static uint8_t NON_EQUIPMENT_START_SLOT = 12;
	const static uint8_t TAB_SIZE = 30;

	PlayerInventory() {
		setItem(ItemTypeList::MONEY.getInventorySlotId(), MoneyItem(0));
	}
	virtual ~PlayerInventory() {

	}
	uint8_t getSlotForItem(const Item& item);

	__inline uint32_t getMoneyAmount() const {
		return ((const Item&)getItem(0)).getAmount();
	}

	__inline void addMoney(uint32_t additionalMoney) {
		getItem(0).addAmount(additionalMoney);
	}

	__inline void decreaseMoney(uint32_t moneyAmount) {
		getItem(0).setAmount(getItem(0).getAmount() - moneyAmount);
	}

	__inline void setMoney(uint32_t money) {
		getItem(0).setAmount(money);
	}
};

#endif