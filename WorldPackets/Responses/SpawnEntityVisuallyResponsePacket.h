#ifndef __ROSE_SPAWN_ENTITY_VISUALLY_RESPONSEPACKET__
#define __ROSE_SPAWN_ENTITY_VISUALLY_RESPONSEPACKET__

#include "..\..\..\JBROSE_Common\Packet.h"
#include "..\..\BasicTypes\Position.h"

class SpawnEntityVisuallyResponsePacket : public ResponsePacket {
private:
	enum VisualityAction : uint16_t {
		IDLE, MOVING, FIGHTING, DEAD 
	};
	uint16_t entityLocalId;
	Position currentPosition;
	Position destinationPosition;
	VisualityAction currentAction;
	uint16_t targetLocalId;
	uint8_t currentStance;
	uint32_t currentHp;
	uint32_t teamId;
	uint32_t buffBits;
	const static uint16_t ID = 0x00;
protected:
	SpawnEntityVisuallyResponsePacket(uint16_t packetId);
	SpawnEntityVisuallyResponsePacket(uint16_t packetId, class Entity* entity);
	virtual void appendContentToSendable(SendablePacket& packet) const;
public:
	virtual ~SpawnEntityVisuallyResponsePacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getEntityLocalId() const {
		return entityLocalId;
	}
	__inline Position getCurrentPosition() const {
		return currentPosition;
	}
	__inline Position getDestinationPosition() const {
		return destinationPosition;
	}
	__inline uint8_t getActionBits() const {
		return currentAction;
	}
	__inline uint16_t getTargetLocalId() const {
		return targetLocalId;
	}
	__inline uint8_t getCurrentStance() const {
		return currentStance;
	}
	__inline uint32_t getCurrentHp() const {
		return currentHp;
	}
	__inline uint32_t getTeamId() const {
		return teamId;
	}
	__inline uint32_t getBuffBits() const {
		return buffBits;
	}
};

#endif //__ROSE_SPAWN_ENTITY_VISUALLY_RESPONSEPACKET__