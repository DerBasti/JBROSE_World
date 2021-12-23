#ifndef __ROSE_DISTRIBUTE_STAT_POINT_RESPONSE_PACKET__
#define __ROSE_DISTRIBUTE_STAT_POINT_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class DistributeStatPointResponsePacket : public ResponsePacket {
private:
	uint8_t statType;
	class PlayerAttributes* attribute;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x7A9;
	DistributeStatPointResponsePacket(class PlayerAttributes* attribute, uint8_t statType);
	virtual ~DistributeStatPointResponsePacket();
	virtual std::string toPrintable() const;

};

#endif //__ROSE_DISTRIBUTE_STAT_POINT_RESPONSE_PACKET__