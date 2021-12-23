#ifndef __ROSE_COLLISION_RESPONSEPACKET__
#define __ROSE_COLLISION_RESPONSEPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"
#include "..\..\BasicTypes\Position.h"

class CollisionResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
	Position collisionPosition;
	uint16_t z;
	const static uint16_t DEFAULT_LENGTH = 18;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x771;
	CollisionResponsePacket();
	virtual ~CollisionResponsePacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getLocalEntityId() const {
		return localId;
	}
	__inline void setEntityLocalId(const uint16_t id) {
		localId = id;
	}
	__inline const Position& getCollisionPosition() const {
		return collisionPosition;
	}
	__inline void setCollisionPosition(const Position& pos) {
		collisionPosition = pos;
	}
	__inline uint16_t getZ() const {
		return z;
	}
	__inline void setZ(const uint16_t z) {
		this->z = z;
	}
};

#endif //__ROSE_COLLISION_RESPONSEPACKET__