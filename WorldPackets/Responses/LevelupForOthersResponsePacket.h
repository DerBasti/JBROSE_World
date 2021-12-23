#ifndef __ROSE_LEVELUP_FOR_OTHERS_RESPONSEPACKET__
#define __ROSE_LEVELUP_FOR_OTHERS_RESPONSEPACKET__

#include "../../../JBROSE_Common/Packet.h"

class LevelupForOthersResponsePacket : public ResponsePacket {
private:
	const static uint16_t DEFAULT_PACKET_LENGTH = 8;
	uint16_t localId;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x79E;
	LevelupForOthersResponsePacket(uint16_t localId);
	virtual ~LevelupForOthersResponsePacket();
	virtual std::string toPrintable() const;
};

#endif //__ROSE_LEVELUP_FOR_OTHERS_RESPONSEPACKET__