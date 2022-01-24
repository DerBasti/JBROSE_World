#ifndef __INVENTORY_RESPONSEPACKET__
#define __INVENTORY_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class InventoryResponsePacket : public ResponsePacket {
private:
	uint64_t moneyAmount;
	uint16_t unknown1;
	uint32_t unknown2;

	uint16_t itemHeader[140];
	uint32_t itemData[140];
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x716;

	InventoryResponsePacket(class PlayerInventory* inventory);
	virtual ~InventoryResponsePacket();
};

#endif //__INVENTORY_RESPONSEPACKET__