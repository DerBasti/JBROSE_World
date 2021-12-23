#ifndef __ROSE_COLLISION_REQUESTPACKET__
#define __ROSE_COLLISION_REQUESTPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"
#include "..\..\BasicTypes\Position.h"

class CollisionRequestPacket : public Packet {
private:
	Position collisionPosition;
	uint16_t z;
	const static uint16_t DEFAULT_LENGTH = 16;
public:
	const static uint16_t ID = 0x771;
	CollisionRequestPacket(const Packet* packet);
	virtual ~CollisionRequestPacket();
	virtual std::string toPrintable() const;

	__inline const Position& getCollisionPosition() const {
		return collisionPosition;
	}
	__inline uint16_t getZ() const {
		return z;
	}
};

#endif //__ROSE_COLLISION_REQUESTPACKET__