#include "UpdateMoneyResponsePacket.h"
	
UpdateMoneyResponsePacket::UpdateMoneyResponsePacket(uint64_t money) : ResponsePacket(ID) {
	currentMoney = money;
}

UpdateMoneyResponsePacket::~UpdateMoneyResponsePacket() {

}

void UpdateMoneyResponsePacket::appendContentToSendable(SendablePacket& sendable) const {
	sendable.addData(currentMoney);
}

std::string UpdateMoneyResponsePacket::toPrintable() const {
	char buf[0x70] = { 0x00 };
	sprintf_s(buf, "[UpdateMoneyResponsePacket]\n\t* Total amount of money: %llu", currentMoney);
	return std::string(buf);
}