#ifndef __ROSE_USE_CONSUMABLE_REQUEST_PACKET__
#define __ROSE_USE_CONSUMABLE_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class UseConsumableRequestPacket : public Packet {
private:
	uint8_t slotId;
	uint8_t repairSlotId;
public:
	const static uint16_t ID = 0x7a3;
	UseConsumableRequestPacket(const Packet* packet);
	virtual ~UseConsumableRequestPacket();

	__inline uint8_t getInventorySlot() const {
		return slotId;
	}
};

#endif //__ROSE_USE_CONSUMABLE_REQUEST_PACKET__