#ifndef __ASSIGN_LOCAL_PLAYER_ID_RESPONSEPACKET__
#define __ASSIGN_LOCAL_PLAYER_ID_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class AssignLocalPlayerIdResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
	uint16_t currentHp;
	uint16_t currentMp;
	uint32_t experiencePoints;
	uint32_t unusedExpPoints;

	uint16_t unknown1;
	uint32_t unknown2;
	uint16_t unknown3;
	uint32_t unknown4;
	uint32_t unknown5;
	uint32_t unknown6;

	uint32_t pvpMapFlag;
	uint32_t mapTime;
	uint32_t teamId;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x753;
	const static uint16_t DEFAULT_LENGTH = 40;
	AssignLocalPlayerIdResponsePacket(const class Player* player);
	virtual ~AssignLocalPlayerIdResponsePacket();
};

#endif //__ASSIGN_LOCAL_PLAYER_ID_RESPONSEPACKET__