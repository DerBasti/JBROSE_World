#include "Item.h"

Item::Item() : Item(ItemTypeList::UNKNOWN, 0, 0) {}

Item::Item(const ItemType& type, const uint16_t id) : Item(type, id, 1) {}

Item::Item(const ItemType& _type, const uint16_t id, const uint32_t amount) : type(_type) {
	this->id = id;
	this->amount = amount;
	lifespan = 1000;
	durability = 35;

	refineLevel = 0;
	statId = 0;

	appraised = true;
	socketed = false;
}

Item::~Item() {

}

std::ostream& operator<<(std::ostream& out, const Item& item) {
	//Ex: [T: 5, ID: 101 (n: 1)]
	out << "[T: " << item.getType() << ", ID: " << item.getId() << " (n: " << item.getAmount() << ")]";
	return out;
}

std::wostream& operator<<(std::wostream& out, const Item& item) {
	out << "[T: " << item.getType() << ", ID: " << item.getId() << " (n: " << item.getAmount() << ")]";
	return out;
}


const uint32_t ItemVisuality::toVisualityBytes(const Item& item) {
	uint32_t resultBytes = item.getId() | (item.getRefineLevel() * 0x10000);
	if (item.getStatId() > 0) {
		resultBytes |= 0xd0000 + ((item.getStatId() - 320) * 0x400);
	}
	return resultBytes;
}

const uint16_t ItemVisuality::toPacketHeader(const Item& item) {
	uint16_t resultBytes = 0x00;
	if (item.getAmount() > 0) {
		resultBytes = ((item.getId() << 5) & 0xFFE0) | (static_cast<uint8_t>(item.getType().getTypeId()) & 0x1F);
	}
	return resultBytes;
}

const uint32_t ItemVisuality::toPacketBody(const Item& item) {
	const uint8_t type = item.getType();
	if ((type >= ItemTypeList::CONSUMABLE.getTypeId() && type <= ItemTypeList::QUEST.getTypeId()) || type == ItemTypeList::MONEY.getTypeId() || item.getAmount() == 0x00) {
		return item.getAmount();
	}
	uint32_t result = (item.getRefineLevel() >> 4) << 28;
	result |= item.isAppraised() << 27;
	result |= item.isSocketed() << 26;
	result |= item.getLifespan() << 16;
	result |= item.getDurability() << 9;
	result |= item.getStatId();

	return result;
}