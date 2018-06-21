#ifndef __NEW_DESTINATION_RESPONSEPACKET__
#define __NEW_DESTINATION_RESPONSEPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"
#include "..\..\BasicTypes\Position.h"

class NewDestinationResponsePacket : public ResponsePacket {
private:
	const static uint16_t DEFAULT_LENGTH = 16;
	uint16_t entityId;
	uint16_t localTargetId;
	uint16_t unknown;
	Position position;
	uint16_t z;
protected:
	NewDestinationResponsePacket(const uint16_t overrideId);
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x79A;
	NewDestinationResponsePacket();
	virtual ~NewDestinationResponsePacket();

	__inline uint16_t getEntityId() const {
		return entityId;
	}
	__inline void setEntityLocalId(const uint16_t entityId) {
		this->entityId = entityId;
	}
	__inline uint16_t getLocalTargetId() const {
		return localTargetId;
	}
	__inline void setLocalTargetId(const uint16_t targetId) {
		localTargetId = targetId;
	}
	__inline Position getPosition() const {
		return position;
	}
	__inline void setDestinationPosition(const Position& pos) {
		position = pos;
	}
};

#endif //__NEW_DESTINATION_RESPONSEPACKET__