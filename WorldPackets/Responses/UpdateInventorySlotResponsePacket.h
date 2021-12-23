#ifndef __ROSE_UPDATE_INVENTORYSLOTS_RESPONSEPACKET__
#define __ROSE_UPDATE_INVENTORYSLOTS_RESPONSEPACKET__

#include "../../../JBROSE_Common/Packet.h"

class UpdateInventorySlotsResponsePacket : public ResponsePacket {
private:
	std::vector<uint8_t> slotIdUpdates;
	std::vector<class Item> itemUpdates;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x718;
	UpdateInventorySlotsResponsePacket();
	virtual ~UpdateInventorySlotsResponsePacket();
	virtual std::string toPrintable() const;

	void addItemToUpdate(uint8_t slotId, const Item& item);
};

#endif //__ROSE_UPDATE_INVENTORYSLOTS_RESPONSEPACKET__