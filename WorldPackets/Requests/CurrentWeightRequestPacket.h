#ifndef __ROSE_CURRENT_WEIGHT_REQUEST_PACKET__
#define __ROSE_CURRENT_WEIGHT_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class CurrentWeightRequestPacket : public Packet {
private:
	//uint8_t weightInPercent;
public:
	const static uint16_t ID = 0x762;
	CurrentWeightRequestPacket(const Packet* packet);
	virtual ~CurrentWeightRequestPacket();
	virtual std::string toPrintable() const;
};

#endif //__ROSE_CURRENT_WEIGHT_REQUEST_PACKET__