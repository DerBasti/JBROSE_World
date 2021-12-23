#ifndef __ROSE_PICKUP_DROP_REQUEST_PACKET__
#define __ROSE_PICKUP_DROP_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class PickupDropRequestPacket : public Packet {
private:
	uint16_t localId;
public:
	constexpr static uint16_t ID = 0x7A7;
	PickupDropRequestPacket(const Packet* packet);
	virtual ~PickupDropRequestPacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getLocalId() const {
		return localId;
	}
};

#endif //__ROSE_PICKUP_DROP_REQUEST_PACKET__