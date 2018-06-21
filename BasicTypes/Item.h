#ifndef __ROSE_ITEM__
#define __ROSE_ITEM__

#pragma once

#include <inttypes.h>

class ItemType {
private:
	uint8_t typeId;
	uint8_t slot;
public:
	constexpr ItemType(const uint8_t type, const uint8_t inventorySlot) : typeId(type), slot(inventorySlot) {
	}

	const uint8_t getTypeId() const {
		return typeId;
	}

	const uint8_t getInventorySlot() const {
		return slot;
	}

	operator unsigned char() {
		return getTypeId();
	}
};

class ItemTypeList {
public:
	constexpr static ItemType UNKNOWN = ItemType(0, -1);
	constexpr static ItemType FACE = ItemType(1, 1);
	constexpr static ItemType HEADGEAR = ItemType(2, 2);
	constexpr static ItemType ARMOR = ItemType(3, 3);
	constexpr static ItemType GLOVES = ItemType(4, 5);
	constexpr static ItemType SHOES = ItemType(5, 6);
	constexpr static ItemType BACK = ItemType(6, 4);
	constexpr static ItemType JEWELRY = ItemType(7, -1);
	constexpr static ItemType WEAPON = ItemType(8, 7);
	constexpr static ItemType SHIELD = ItemType(9, 8);
	constexpr static ItemType CONSUMABLE = ItemType(10, -1);
	constexpr static ItemType JEWELS = ItemType(11, -1);
	constexpr static ItemType OTHER = ItemType(12, -1);
	constexpr static ItemType QUEST = ItemType(13, -1);
	constexpr static ItemType PAT = ItemType(14, -1);
	constexpr static ItemType MONEY = ItemType(31, -1);
private:
	constexpr static ItemType itemTypes[] = {
		UNKNOWN,
		FACE,
		HEADGEAR,
		ARMOR,
		GLOVES,
		SHOES,
		BACK,
		JEWELRY,
		WEAPON,
		SHIELD,
		CONSUMABLE,
		JEWELS,
		OTHER,
		QUEST,
		PAT
	};
public:
	static const ItemType& toItemType(const uint8_t type) {
		if (type > PAT.getTypeId()) {
			return MONEY;
		}
		return itemTypes[type];
	}
};

class Item {
private:
	ItemType type;
	uint16_t id;
	uint16_t amount;
	uint8_t durability;
	uint16_t lifespan;
	uint16_t statId;
	uint16_t refineLevel;

	bool socketed;
	bool appraised;

public:
	Item();
	Item(const ItemType& type, const uint16_t id);
	Item(const ItemType& type, const uint16_t id, const uint16_t amount);
	virtual ~Item();

	__inline ItemType getType() const {
		return type;
	}
	__inline uint16_t getId() const {
		return id;
	}
	__inline uint16_t getAmount() const {
		return amount;
	}
	__inline void setAmount(const uint16_t amount) {
		this->amount = amount;
	}
	__inline uint8_t getDurability() const {
		return durability;
	}
	__inline uint16_t getLifespan() const {
		return lifespan;
	}
	__inline void setLifespan(const uint16_t lifespan) {
		this->lifespan = lifespan;
	}
	__inline uint16_t getStatId() const {
		return statId;
	}
	__inline uint16_t getRefineLevel() const {
		return refineLevel;
	}
	__inline void setRefineLevel(const uint16_t level) {
		refineLevel = level;
	}
	__inline bool isSocketed() const {
		return socketed;
	}
	__inline void setSocketed(bool socketedFlag) {
		socketed = socketedFlag;
	}
	__inline bool isAppraised() const {
		return appraised;
	}
	__inline void setAppraised(bool appraisedFlag) {
		appraised = appraisedFlag;
	}
};



class ItemVisuality {
private:
	ItemVisuality() {}
	~ItemVisuality() {}
public:
	const static uint32_t toVisualityBytes(const Item& item);
	const static uint16_t toPacketHeader(const Item& item);
	const static uint32_t toPacketBody(const Item& item);
};

#endif //__ROSE_ITEM__