#ifndef __STANCE_RESPONSEPACKET__
#define __STANCE_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class StanceResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
	uint8_t stanceType;
	uint16_t movementSpeed;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x782;
	const static uint16_t DEFAULT_LENGTH = 11;
	StanceResponsePacket();
	virtual ~StanceResponsePacket();

	__inline uint16_t getLocalId() const {
		return localId;
	}
	__inline void setLocalEntityId(const uint16_t id) {
		localId = id;
	}
	__inline uint16_t getStanceType() const {
		return stanceType;
	}
	__inline void setStanceType(const uint8_t stanceType) {
		this->stanceType = stanceType;
	}
	__inline uint16_t getMovementSpeed() const {
		return movementSpeed;
	}
	__inline void setMovementSpeed(const uint16_t speed) {
		movementSpeed = speed;
	}
};

#endif //__STANCE_RESPONSEPACKET__