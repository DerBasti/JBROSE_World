#ifndef __ROSE_UPDATE_MONEY_RESPONSE_PACKET__
#define __ROSE_UPDATE_MONEY_RESPONSE_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class UpdateMoneyResponsePacket : public ResponsePacket {
private:
	uint64_t currentMoney;
protected:
	virtual void appendContentToSendable(SendablePacket& sendable) const;
public:
	constexpr static uint16_t ID = 0x71D;
	UpdateMoneyResponsePacket(uint64_t money);
	virtual ~UpdateMoneyResponsePacket();
	virtual std::string toPrintable() const;
};

#endif //__ROSE_UPDATE_MONEY_RESPONSE_PACKET__