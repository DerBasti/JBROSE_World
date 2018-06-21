#ifndef __ROSE_TELEGATE_REQUESTPACKET__
#define __ROSE_TELEGATE_REQUESTPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class TelegateRequestPacket : public Packet {
private:
	uint16_t telegateId;
	const static uint16_t DEFAULT_LENGTH = 8;
public:
	const static uint16_t ID = 0x7A8;
	TelegateRequestPacket(const Packet* packet);
	virtual ~TelegateRequestPacket();

	__inline uint16_t getTelegateId() const {
		return telegateId;
	}
};

#endif //__ROSE_TELEGATE_REQUESTPACKET__