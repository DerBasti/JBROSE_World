#ifndef __ROSE_UPDATE_EXPERIENCE_RESPONSEPACKET__
#define __ROSE_UPDATE_EXPERIENCE_RESPONSEPACKET__

#include "../../../JBROSE_Common/Packet.h"

class UpdateExperienceResponsePacket : public ResponsePacket {
private:
	const static uint16_t DEFAULT_PACKET_LENGTH = 14;
	uint32_t experience;
	uint16_t stamina;
	uint16_t unknown;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x79B;
	UpdateExperienceResponsePacket(class Player* player);
	virtual ~UpdateExperienceResponsePacket();
	virtual std::string toPrintable() const;

};
#endif //__ROSE_UPDATE_EXPERIENCE_RESPONSEPACKET__