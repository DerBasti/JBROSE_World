#ifndef __ROSE_PICKUP_DROP_RESPONSE_PACKET__
#define __ROSE_PICKUP_DROP_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"
#include "../../BasicTypes/Item.h"

enum class PickupDropMessageType : uint16_t {
	OKAY,
	NOT_OWNER = 2,
	INVENTORY_FULL = 3
};

class PickupDropResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
	PickupDropMessageType messageType;
	uint8_t inventorySlot;
	Item itemToAdd;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	constexpr static uint16_t ID = 0x7A7;
	PickupDropResponsePacket(uint16_t localId);
	virtual ~PickupDropResponsePacket();
	virtual std::string toPrintable() const;

	__inline PickupDropMessageType getDropMessageType() const {
		return messageType;
	}
	__inline void setDropMessageType(PickupDropMessageType messageType) {
		this->messageType = messageType;
	}
	__inline void setItemToAdd(uint8_t slotId, const Item& item) {
		inventorySlot = slotId;
		itemToAdd = item;
	}
	__inline uint8_t getSlotId() const {
		return inventorySlot;
	}
	const Item& getItem() const {
		return itemToAdd;
	}
};

#endif //__ROSE_PICKUP_DROP_RESPONSE_PACKET__