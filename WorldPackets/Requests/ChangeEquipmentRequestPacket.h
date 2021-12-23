#ifndef __ROSE_CHANGE_EQUIPMENT_REQUESTPACKET__
#define __ROSE_CHANGE_EQUIPMENT_REQUESTPACKET__

#include "../../../JBROSE_Common/Packet.h"

class ChangeEquipmentRequestPacket : public Packet {
private:
	uint8_t sourceSlotId;
	uint8_t destinationSlotId;
public:
	const static uint16_t ID = 0x7A5;
	ChangeEquipmentRequestPacket(const Packet *packet);
	virtual ~ChangeEquipmentRequestPacket();

	virtual std::string toPrintable() const;

	__inline uint8_t getSourceSlotId() const {
		return sourceSlotId;
	}
	__inline uint8_t getDestinationSlotId() const {
		return destinationSlotId;
	}
};

#endif //__ROSE_CHANGE_EQUIPMENT_REQUESTPACKET__