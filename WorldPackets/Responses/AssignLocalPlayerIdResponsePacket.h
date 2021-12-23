#ifndef __ASSIGN_LOCAL_PLAYER_ID_RESPONSEPACKET__
#define __ASSIGN_LOCAL_PLAYER_ID_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

enum class AssignLocalPlayerIdPVPFlag : uint32_t {
	NO_PVP = 0x00,
	PVP_ALLOWED = 0x01
};

class AssignLocalPlayerIdResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
	uint16_t currentHp;
	uint16_t currentMp;
	uint32_t experiencePoints;
	uint32_t unusedExpPoints;

	uint16_t productRate;
	uint32_t updateTime;
	uint16_t worldRate;
	uint8_t townRate;
	uint8_t itemRates[11];

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

	virtual std::string toPrintable() const;

	__inline void setPVPFlag(AssignLocalPlayerIdPVPFlag flag) {
		pvpMapFlag = static_cast<uint32_t>(flag);
	}
};

#endif //__ASSIGN_LOCAL_PLAYER_ID_RESPONSEPACKET__