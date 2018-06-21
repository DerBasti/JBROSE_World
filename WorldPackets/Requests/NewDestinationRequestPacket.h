#ifndef __NEW_DESTINATION_REQUESTPACKET__
#define __NEW_DESTINATION_REQUESTPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class NewDestinationRequestPacket : public Packet {
private:
	const static uint16_t DEFAULT_LENGTH = 16;

	uint16_t targetLocalId;
	float destinationX;
	float destinationY;
public:
	const static uint16_t ID = 0x79A;
	NewDestinationRequestPacket(const Packet* packet);
	virtual ~NewDestinationRequestPacket();

	__inline uint16_t getTargetLocalId() const {
		return targetLocalId;
	}
	__inline float getDestinationX() const {
		return destinationX;
	}
	__inline float getDestinationY() const {
		return destinationY;
	}
};

#endif //__NEW_DESTINATION_REQUESTPACKET__