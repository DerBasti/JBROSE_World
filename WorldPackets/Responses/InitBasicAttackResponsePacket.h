#ifndef __INIT_BASICATTACK_RESPONSEPACKET__
#define __INIT_BASICATTACK_RESPONSEPACKET__
#pragma once

#include "../../../JBROSE_Common/Packet.h"
#include "../../BasicTypes/Position.h"

class InitBasicAttackResponsePacket : public ResponsePacket {
private:
	uint16_t selfLocalId;
	uint16_t targetLocalId;
	uint16_t unknown;
	Position targetPosition;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x798;
	InitBasicAttackResponsePacket(class Combat* combat);
	virtual ~InitBasicAttackResponsePacket();
	virtual std::string toPrintable() const;

	uint16_t getSelfLocalId() const {
		return selfLocalId;
	}
	uint16_t getTargetLocalId() const {
		return targetLocalId;
	}
	const Position& getTargetPosition() const {
		return targetPosition;
	}
};

#endif