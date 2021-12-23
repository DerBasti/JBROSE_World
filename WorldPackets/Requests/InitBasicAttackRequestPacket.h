#ifndef __ROSE_BASIC_ATTACK_REQUEST_PACKET__
#define __ROSE_BASIC_ATTACK_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class InitBasicAttackRequestPacket : public Packet {
private:
	uint16_t targetLocalId;
public:
	const static uint16_t ID = 0x798;
	InitBasicAttackRequestPacket(const Packet* p);
	virtual ~InitBasicAttackRequestPacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getTargetLocalId() const {
		return targetLocalId;
	}
};

#endif //__ROSE_BASIC_ATTACK_REQUEST_PACKET__