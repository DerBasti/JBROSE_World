#ifndef __ASSIGN_LOCAL_PLAYER_ID_REQUESTPACKET__
#define __ASSIGN_LOCAL_PLAYER_ID_REQUESTPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class AssignLocalPlayerIdRequestPacket : public Packet {
public:
	const static uint16_t ID = 0x753;
	AssignLocalPlayerIdRequestPacket(const Packet* p) : Packet(ID, p->getLength()) {

	}
	virtual ~AssignLocalPlayerIdRequestPacket() {

	}
};

#endif //__ASSIGN_LOCAL_PLAYER_ID_REQUESTPACKET__
