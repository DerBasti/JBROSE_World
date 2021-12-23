#ifndef __ROSE_SPAWN_PLAYER_VISUALLY_RESPONSEPACKET__
#define __ROSE_SPAWN_PLAYER_VISUALLY_RESPONSEPACKET__
#pragma once

#include "SpawnEntityVisuallyResponsePacket.h"

class SpawnPlayerVisuallyResponsePacket : public SpawnEntityVisuallyResponsePacket {
private:
	uint8_t sex;
	uint16_t movementSpeed;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x793;
	SpawnPlayerVisuallyResponsePacket(class Player* entity);
	virtual ~SpawnPlayerVisuallyResponsePacket();
	virtual std::string toPrintable() const;

	__inline uint8_t getSex() const {
		return sex;
	}
	__inline uint16_t getMovementSpeed() const {
		return movementSpeed;
	}
};

#endif //__ROSE_SPAWN_PLAYER_VISUALLY_RESPONSEPACKET__