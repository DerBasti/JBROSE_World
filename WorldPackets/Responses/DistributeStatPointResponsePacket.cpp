#include "DistributeStatPointResponsePacket.h"
#include "../../WorldClient.h"

DistributeStatPointResponsePacket::DistributeStatPointResponsePacket(PlayerAttributes* attribute, uint8_t statType) : ResponsePacket(ID, 9) {
	this->statType = statType;
	this->attribute = attribute;
}

DistributeStatPointResponsePacket::~DistributeStatPointResponsePacket() {

}

void DistributeStatPointResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(statType);
	packet.addData(attribute->getPointsLearned());
}

std::string DistributeStatPointResponsePacket::toPrintable() const {
	char buf[0xA0] = { 0x00 };
	sprintf_s(buf, "[DistributeStatPointResponsePacket]\n\t* StatType: %s\n\t* Distributed points: %i", attribute->getAttributeName(), statType);
	return std::string(buf);
}