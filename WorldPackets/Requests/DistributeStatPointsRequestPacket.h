#ifndef __ROSE_DISTRIBUTE_STAT_POINTS_REQUEST_PACKET__
#define __ROSE_DISTRIBUTE_STAT_POINTS_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class DistributeStatPointRequestPacket : public Packet {
private:
	const static uint16_t DEFAULT_LENGTH = 7;
	uint8_t statType;
public:
	const static uint16_t ID = 0x7A9;
	DistributeStatPointRequestPacket(const Packet* packet);
	virtual ~DistributeStatPointRequestPacket();
	virtual std::string toPrintable() const;

	__inline uint8_t getStatTypeId() const {
		return statType;
	}
};

#endif //__ROSE_DISTRIBUTE_STAT_POINTS_REQUEST_PACKET__