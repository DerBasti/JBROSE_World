#ifndef __PING_REQUESTPACKET__
#define __PING_REQUESTPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class PingRequestPacket : public Packet {
public:
	const static uint16_t ID = 0x700;
	PingRequestPacket(const Packet* packet);
	virtual ~PingRequestPacket();
	virtual std::string toPrintable() const;
};

#endif //__PING_REQUESTPACKET__