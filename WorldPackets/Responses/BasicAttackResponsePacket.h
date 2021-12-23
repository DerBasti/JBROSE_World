#ifndef __ROSE_BASIC_ATTACK_RESPONSE_PACKET__
#define __ROSE_BASIC_ATTACK_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"
#include "../../BasicTypes/Combat.h"

class BasicAttackResponsePacket : public ResponsePacket {
private:
	const static uint16_t DEFAULT_PACKET_LENGTH = 14;
	DamageHit damageHit;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x799;
	BasicAttackResponsePacket(DamageHit hit);
	virtual ~BasicAttackResponsePacket();

	virtual std::string toPrintable() const;

	__inline void setDamage(DamageHit damage) {
		damage = std::move(damage);
	}
};

#endif //__ROSE_BASIC_ATTACK_RESPONSE_PACKET__