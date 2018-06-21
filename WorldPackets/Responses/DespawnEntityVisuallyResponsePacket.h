#ifndef __ROSE_DESPAWN_ENTITY_VISUALLY_RESPONSEPACKET__
#define __ROSE_DESPAWN_ENTITY_VISUALLY_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class DespawnEntityVisuallyResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x794;
	DespawnEntityVisuallyResponsePacket(uint16_t localId);
	DespawnEntityVisuallyResponsePacket(class Entity* entity);
	virtual ~DespawnEntityVisuallyResponsePacket();

	__inline uint16_t getLocalId() const {
		return localId;
	}
	__inline void setLocalId(const uint16_t localId) {
		this->localId = localId;
	}
};

#endif //__ROSE_DESPAWN_ENTITY_VISUALLY_RESPONSEPACKET__