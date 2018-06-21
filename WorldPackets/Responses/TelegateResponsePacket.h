#ifndef __ROSE_TELEGATE_RESPONSEPACKET__
#define __ROSE_TELEGATE_RESPONSEPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"
#include "..\..\BasicTypes\Position.h"

class TelegateResponsePacket : public ResponsePacket {
private:
	uint16_t localEntityId;
	uint16_t mapId;
	Position position;
	uint16_t z;
	const static uint16_t DEFAULT_LENGTH = 22;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x7A8;
	TelegateResponsePacket();
	virtual ~TelegateResponsePacket();

	__inline void setLocalEntityId(const uint16_t localId) {
		localEntityId = localId;
	}
	__inline void setMapId(const uint16_t map) {
		mapId = map;
	}
	__inline void setPosition(const Position& destinationPosition) {
		position = destinationPosition;
	}
};

#endif //__ROSE_TELEGATE_RESPONSEPACKET__