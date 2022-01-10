#ifndef __ROSE_ITEM__
#define __ROSE_ITEM__

#pragma once

#include <inttypes.h>
#include <iostream>

class InventorySlot {
public:
	constexpr static uint8_t UNKNOWN_SLOT = -1;
	constexpr static uint8_t MONEY = 0;
	constexpr static uint8_t FACE_SLOT = 1;
	constexpr static uint8_t HEADGEAR_SLOT = 2;
	constexpr static uint8_t ARMOR_SLOT = 3;
	constexpr static uint8_t BACK_SLOT = 4;
	constexpr static uint8_t GLOVE_SLOT = 5;
	constexpr static uint8_t SHOES_SHOT = 6;
	constexpr static uint8_t WEAPON_SLOT = 7;
	constexpr static uint8_t SHIELD_SLOT = 8;

	constexpr static uint8_t ARROWS = 132;
	constexpr static uint8_t BULLETS = 133;
	constexpr static uint8_t CANNONSHELLS = 134;
	constexpr static uint8_t CART_FRAME = 135;
	constexpr static uint8_t CART_ENGINE = 136;
	constexpr static uint8_t CART_WHEELS = 137;
	constexpr static uint8_t CART_WEAPON = 138;
	constexpr static uint8_t CART_ABILITY = 139;
private:
	InventorySlot() {}
	~InventorySlot() {}
};

class ItemType {
private:
	uint8_t typeId;
	uint8_t slot;
	const char* typeName;
public:
	constexpr ItemType(const uint8_t type, const char* itemTypeName, const uint8_t inventorySlot) : typeId(type), slot(inventorySlot), typeName(itemTypeName) {
	}

	__inline const uint8_t getTypeId() const {
		return typeId;
	}

	__inline const uint8_t getInventorySlotId() const {
		return slot;
	}
	__inline const char* getTypeName() const {
		return typeName;
	}

	operator unsigned char() const {
		return getTypeId();
	}
};

class ItemTypeList {
public:
	constexpr static ItemType UNKNOWN = ItemType(0, "Unknown", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType FACE = ItemType(1, "Face", InventorySlot::FACE_SLOT);
	constexpr static ItemType HEADGEAR = ItemType(2, "Headgear", InventorySlot::HEADGEAR_SLOT);
	constexpr static ItemType ARMOR = ItemType(3, "Armor", InventorySlot::ARMOR_SLOT);
	constexpr static ItemType GLOVES = ItemType(4, "Gloves", InventorySlot::GLOVE_SLOT);
	constexpr static ItemType SHOES = ItemType(5, "Shoes", InventorySlot::SHOES_SHOT);
	constexpr static ItemType BACK = ItemType(6, "Back", InventorySlot::BACK_SLOT);
	constexpr static ItemType JEWELRY = ItemType(7, "Jewelry", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType WEAPON = ItemType(8, "Weapon", InventorySlot::WEAPON_SLOT);
	constexpr static ItemType SHIELD = ItemType(9, "Shield", InventorySlot::SHIELD_SLOT);
	constexpr static ItemType CONSUMABLE = ItemType(10, "Consumable", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType JEWELS = ItemType(11, "Jewels", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType OTHER = ItemType(12, "Etc", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType QUEST = ItemType(13, "Quest", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType PAT = ItemType(14, "PAT", InventorySlot::UNKNOWN_SLOT);
	constexpr static ItemType MONEY = ItemType(31, "Money", InventorySlot::MONEY);
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
		PAT,
		MONEY
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
	uint32_t amount;
	uint16_t itemCategoryId;
	uint8_t durability;
	uint16_t lifespan;
	uint16_t statId;
	uint16_t refineLevel;

	bool socketed;
	bool appraised;

public:
	Item();
	Item(const ItemType& type, const uint16_t id);
	Item(const ItemType& type, const uint16_t id, const uint32_t amount);
	virtual ~Item();

	__inline ItemType getType() const {
		return type;
	}
	__inline uint16_t getId() const {
		return id;
	}
	__inline uint32_t getAmount() const {
		return amount;
	}
	__inline void setAmount(const uint32_t amount) {
		this->amount = amount;
	}
	__inline void addAmount(const uint32_t amount) {
		this->amount += amount;
	}
	__inline uint8_t getDurability() const {
		return durability;
	}
	__inline void setDurability(const uint8_t newDurability) {
		durability = newDurability <= 120 ? newDurability : 120;
	}
	__inline uint16_t getLifespan() const {
		return lifespan;
	}
	__inline uint16_t getCategoryId() const {
		return itemCategoryId;
	}
	__inline void setCategoryId(uint16_t categoryId) {
		itemCategoryId = categoryId;
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
	virtual bool isValid() const {
		return getAmount() > 0 && getType() > 0 && (getType() == ItemTypeList::MONEY || getId() > 0);
	}
	virtual bool isStackable() const {
		return getType() == ItemTypeList::CONSUMABLE || getType() == ItemTypeList::JEWELS || getType() == ItemTypeList::MONEY || getType() == ItemTypeList::OTHER || getType() == ItemTypeList::QUEST;
	}
	void clear() {
		amount = 0;
		id = 0;
		durability = 0;
		lifespan = 0;
		statId = 0;
		refineLevel = 0;
		itemCategoryId = 0;
		type = ItemTypeList::UNKNOWN;
		socketed = false;
		appraised = true;
	}
};

class MoneyItem : public Item {
public:
	MoneyItem(uint32_t amount) : Item(ItemTypeList::MONEY, 0xCCCC, amount) { }
	virtual ~MoneyItem() { }

	 virtual bool isValid() const {
		return getAmount() > 0;
	}
	__inline bool isStackable() const {
		return true;
	}
};

class WeaponType {
private:
	WeaponType() {}
	virtual ~WeaponType() {}
public:
	const static uint16_t MELEE_ONE_HANDED_SWORD = 211;
	const static uint16_t MELEE_ONE_HANDED_BLUNT = 212;
	const static uint16_t MELEE_TWO_HANDED_SWORD = 221;
	const static uint16_t MELEE_TWO_HANDED_SPEAR = 222;
	const static uint16_t MELEE_TWO_HANDED_AXE = 223;
	const static uint16_t RANGE_BOW = 231;
	const static uint16_t RANGE_GUN = 232;
	const static uint16_t RANGE_LAUNCHER = 233;
	const static uint16_t MAGIC_WAND = 241;
	const static uint16_t MAGIC_STAFF = 242;
	const static uint16_t MELEE_KATAR = 251;
	const static uint16_t MELEE_DOUBLE_SWORD = 252;
	const static uint16_t RANGE_DUAL_GUN = 253;
	const static uint16_t RANGE_CROSSBOW = 271;
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

std::ostream& operator<<(std::ostream& out, const Item& item);
std::wostream& operator<<(std::wostream& out, const Item& item);

#endif //__ROSE_ITEM__