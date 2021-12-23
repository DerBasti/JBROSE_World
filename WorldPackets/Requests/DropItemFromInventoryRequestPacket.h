#ifndef __ROSE_DROP_ITEM_FROM_INVENTORY_REQUESTPACKET__
#define __ROSE_DROP_ITEM_FROM_INVENTORY_REQUESTPACKET__

#include "../../../JBROSE_Common/Packet.h"

class DropItemFromInventoryRequestPacket : public Packet {
private:
	uint8_t slotId;
	uint32_t amount;
	constexpr static uint16_t DEFAULT_LENGTH = 11;
public:
	constexpr static uint16_t ID = 0x7A4;

	DropItemFromInventoryRequestPacket(const Packet *packet);
	virtual ~DropItemFromInventoryRequestPacket();
	virtual std::string toPrintable() const;

	uint8_t getSlotId() const {
		return slotId;
	}
	uint32_t getAmount() const {
		return amount;
	}
};

#endif //__ROSE_DROP_ITEM_FROM_INVENTORY_REQUESTPACKET__