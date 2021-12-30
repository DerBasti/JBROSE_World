#ifndef __ROSE_INVENTORY__
#define __ROSE_INVENTORY__

#include "Item.h"

class Inventory {
public:
	const static uint8_t NON_EQUIPMENT_START_SLOT = 12;
	const static uint8_t TAB_SIZE = 30;
	const static uint8_t MAX_SLOTS = 140;
	const static uint16_t MAXIMUM_AMOUNT_PER_STACK = 999;
	const static uint8_t INVALID_SLOT = -1;
private:
	uint32_t money;
	Item inventorySlots[MAX_SLOTS];
	uint8_t weightPercentage;

	uint8_t findItemSlotForStacking(const Item& itemToStack, uint8_t tabId);
	uint8_t findEmptyItemSlot(uint8_t tabId);
public:
	Inventory() {
	}
	virtual ~Inventory() {}

	const Item& operator[](uint8_t slotId) const {
		return inventorySlots[slotId];
	}

	uint8_t addItemToInventory(const Item& item);
	bool reduceAmountOfItemInSlot(uint8_t slotId, uint32_t amount);
	uint8_t getSlotForItem(const Item& item);
	uint16_t getTotalWeight() const;

	__inline uint32_t getMoneyAmount() const {
		return money;
	}

	__inline void addMoney(uint32_t additionalMoney) {
		money += additionalMoney;
	}

	__inline void decreaseMoney(uint32_t moneyAmount) {
		money -= moneyAmount;
	}

	__inline void setMoney(uint32_t money) {
		this->money = money;
	}

	__inline Item& getItem(uint8_t slot) {
		return inventorySlots[slot];
	}

	__inline void setItem(const uint8_t slot, const Item& item) {
		inventorySlots[slot] = item;
	}

	__inline void clearItem(const uint8_t slot) {
		inventorySlots[slot] = Item();
	}
};

#endif