#ifndef __ROSE_EXIT_PACKET__
#define __ROSE_EXIT_PACKET__
#pragma once

#include "../../../JBROSE_Common/Packet.h"


class ExitRequestPacket : public Packet {
private:
	const static uint16_t DEFAULT_LENGTH = Packet::DEFAULT_MINIMUM_SIZE;
public:
	const static uint16_t ID = 0x707;
	ExitRequestPacket(const Packet* packet);
	virtual ~ExitRequestPacket();

	virtual std::string toPrintable() const {
		char buf[0x40] = { 0x00 };
		sprintf_s(buf, "[ExitRequestPacket - %i bytes]", getLength());
		return std::string(buf);
	}
};

#endif //__ROSE_EXIT_PACKET__