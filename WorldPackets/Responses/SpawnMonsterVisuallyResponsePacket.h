#ifndef __ROSE_SPAWN_MONSTER_VISUALLY_RESPONSEPACKET__
#define __ROSE_SPAWN_MONSTER_VISUALLY_RESPONSEPACKET__

#pragma once

#include "SpawnEntityVisuallyResponsePacket.h"

class SpawnMonsterVisuallyResponsePacket : public SpawnEntityVisuallyResponsePacket {
private:
	uint16_t typeId;
	uint16_t unknown;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x792;
	SpawnMonsterVisuallyResponsePacket(class Monster* monster);
	virtual ~SpawnMonsterVisuallyResponsePacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getMonsterTypeId() const {
		return typeId;
	}
	__inline uint16_t getUnknown() const {
		return unknown;
	}
};

#endif //__ROSE_SPAWN_MONSTER_VISUALLY_RESPONSEPACKET__