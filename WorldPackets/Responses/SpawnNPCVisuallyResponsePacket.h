#ifndef __ROSE_SPAWN_NPC_VISUALLY_RESPONSEPACKET__
#define __ROSE_SPAWN_NPC_VISUALLY_RESPONSEPACKET__

#pragma once

#include "SpawnEntityVisuallyResponsePacket.h"

class SpawnNPCVisuallyResponsePacket : public SpawnEntityVisuallyResponsePacket {
private:
	uint16_t typeId;
	uint16_t conversationId;
	float direction;
	uint16_t unknown;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x791;
	SpawnNPCVisuallyResponsePacket(class NPC* monster);
	virtual ~SpawnNPCVisuallyResponsePacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getNPCTypeId() const {
		return typeId;
	}
	__inline uint16_t getConversationId() const {
		return conversationId;
	}
	__inline float getDirection() const {
		return direction;
	}
	__inline uint16_t getUnknown() const {
		return unknown;
	}
};

#endif //__ROSE_SPAWN_NPC_VISUALLY_RESPONSEPACKET__