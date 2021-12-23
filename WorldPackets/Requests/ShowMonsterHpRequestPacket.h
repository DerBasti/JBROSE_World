#ifndef __ROSE_SHOW_MONSTER_HP_REQUEST_PACKET__
#define __ROSE_SHOW_MONSTER_HP_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class ShowMonsterHpRequestPacket : public Packet {
private:
	uint16_t monsterLocalId;
	const static uint16_t DEFAULT_LENGTH = DEFAULT_MINIMUM_SIZE + sizeof(uint16_t);
public:
	const static uint16_t ID = 0x79F;
	ShowMonsterHpRequestPacket(const Packet *packet);
	virtual ~ShowMonsterHpRequestPacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getMonsterLocalId() const {
		return monsterLocalId;
	}
};

#endif //__ROSE_SHOW_MONSTER_HP_REQUEST_PACKET__