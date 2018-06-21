#include "GamingPlanResponsePacket.h"

GamingPlanResponsePacket::GamingPlanResponsePacket() : ResponsePacket(ID) {
	unknown1 = 0x1001;
	planType = 0x02;
}

GamingPlanResponsePacket::~GamingPlanResponsePacket() {

}

void GamingPlanResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(unknown1);
	packet.addData(planType);
}