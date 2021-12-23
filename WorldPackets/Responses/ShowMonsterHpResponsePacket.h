#ifndef __ROSE_SHOW_MONSTER_HP_RESPONSE_PACKET__
#define __ROSE_SHOW_MONSTER_HP_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class ShowMonsterHpResponsePacket : public ResponsePacket {
private:
	uint16_t monsterLocalId;
	uint32_t monsterCurrentHp;
protected:
	virtual void appendContentToSendable(SendablePacket& sendable) const;
public:
	const static uint16_t ID = 0x79F;
	ShowMonsterHpResponsePacket(class Monster* monster);
	virtual ~ShowMonsterHpResponsePacket();
	virtual std::string toPrintable() const;
};

#endif //__ROSE_SHOW_MONSTER_HP_RESPONSE_PACKET__