#ifndef __CHANGE_RESPAWN_TOWN_REQUESTPACKET__
#define __CHANGE_RESPAWN_TOWN_REQUESTPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class ChangeRespawnTownRequestPacket : public Packet {
private:

public:
	const static uint16_t ID = 0x756;
	ChangeRespawnTownRequestPacket(const Packet *packet);
	virtual ~ChangeRespawnTownRequestPacket();
};

#endif //__CHANGE_RESPAWN_TOWN_REQUESTPACKET__