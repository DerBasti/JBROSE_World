#ifndef __ROSE_STANCE_REQUEST_PACKET__
#define __ROSE_STANCE_REQUEST_PACKET__

#include "..\..\..\JBROSE_Common\Packet.h"

class StanceRequestPacket : public Packet {
	uint8_t stanceId;
public:
	const static uint16_t ID = 0x782;
	StanceRequestPacket(const Packet *packet);
	virtual ~StanceRequestPacket();
	virtual std::string toPrintable() const;

	__inline uint8_t getStanceId() const {
		return stanceId;
	}
};

#endif //__ROSE_STANCE_REQUEST_PACKET__