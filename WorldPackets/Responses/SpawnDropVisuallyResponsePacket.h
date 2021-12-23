#ifndef __ROSE_SPAWN_DROP_VISUALLY_RESPONSEPACKET__
#define __ROSE_SPAWN_DROP_VISUALLY_RESPONSEPACKET__

#pragma once

#include "SpawnEntityVisuallyResponsePacket.h"
#include "../../BasicTypes/Item.h"

class SpawnDropVisuallyResponsePacket : public SpawnEntityVisuallyResponsePacket {
private:
	Item item;
	uint16_t entityLocalId;
	Position currentPosition;
protected:
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x7A6;
	SpawnDropVisuallyResponsePacket(class Drop* drop);
	virtual ~SpawnDropVisuallyResponsePacket();

	virtual std::string toPrintable() const;
};

#endif //__ROSE_SPAWN_DROP_VISUALLY_RESPONSEPACKET__