#ifndef __ROSE_EXIT_PACKETRESPONSE__
#define __ROSE_EXIT_PACKETRESPONSE__

#pragma once

#include "../../../JBROSE_Common/Packet.h"

class ExitResponsePacket : public ResponsePacket {
private:
	const static uint16_t DEFAULT_SIZE = Packet::DEFAULT_MINIMUM_SIZE + sizeof(uint16_t);
	uint16_t timeUntilDisconnect;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x707;

	ExitResponsePacket();
	virtual ~ExitResponsePacket();
	virtual std::string toPrintable() const;

	__inline void setSecondsTillDisconnect(const uint16_t secondsTillDisconnect) {
		timeUntilDisconnect = secondsTillDisconnect;
	}
};

#endif //__ROSE_EXIT_PACKETRESPONSE__