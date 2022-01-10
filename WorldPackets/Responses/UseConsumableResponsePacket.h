#ifndef __ROSE_USE_CONSUMABLE_RESPONSE_PACKET__
#define __ROSE_USE_CONSUMABLE_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class UseConsumableResponsePacket : public ResponsePacket {
private:
	uint16_t itemUserLocalId;
	uint16_t consumableItemId;
	uint8_t inventorySlotId;
protected:
	virtual void appendContentToSendable(SendablePacket& sendable) const;
public:
	const static uint16_t ID = 0x7a3;
	UseConsumableResponsePacket(uint16_t itemUserLocalId, uint16_t consumableItemId);
	virtual ~UseConsumableResponsePacket();

	__inline void setInventorySlotId(uint8_t inventorySlotId) {
		this->inventorySlotId = inventorySlotId;
	}
};

#endif //__ROSE_USE_CONSUMABLE_RESPONSE_PACKET__