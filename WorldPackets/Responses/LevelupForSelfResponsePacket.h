#ifndef __ROSE_LEVELUP_FOR_SELF_RESPONSEPACKET__
#define __ROSE_LEVELUP_FOR_SELF_RESPONSEPACKET__

#include "../../../JBROSE_Common/Packet.h"

class LevelupForSelfResponsePacket : public ResponsePacket {
private:
	const static uint16_t DEFAULT_PACKET_LENGTH = 18;
	uint16_t localId;
	uint16_t level;
	uint32_t experience;
	uint16_t statPoints;
	uint16_t skillPoints;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x79E;
	LevelupForSelfResponsePacket(class Player* player);
	virtual ~LevelupForSelfResponsePacket();
	virtual std::string toPrintable() const;
};

#endif //__ROSE_LEVELUP_FOR_SELF_RESPONSEPACKET__