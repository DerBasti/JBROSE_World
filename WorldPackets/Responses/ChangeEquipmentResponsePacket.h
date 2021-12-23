#ifndef __ROSE_CHANGE_EQUIPMENT_RESPONSEPACKET__
#define __ROSE_CHANGE_EQUIPMENT_RESPONSEPACKET__

#include "../../../JBROSE_Common/Packet.h"

class ChangeEquipmentResponsePacket : public ResponsePacket {
private:
	uint16_t localId;
	uint16_t inventorySlotId;
	uint32_t itemVisualityBytes;
	uint16_t movementSpeed;
protected:
	virtual void appendContentToSendable(SendablePacket& sendable) const;
public:
	const static uint16_t ID = 0x7A5;
	ChangeEquipmentResponsePacket(class Player* player, uint8_t inventorySlot);
	virtual ~ChangeEquipmentResponsePacket();
	virtual std::string toPrintable() const;

};
#endif //__ROSE_CHANGE_EQUIPMENT_RESPONSEPACKET__