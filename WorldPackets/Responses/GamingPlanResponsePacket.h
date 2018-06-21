#ifndef __GAMING_PLAN_RESPONSEPACKET__
#define __GAMING_PLAN_RESPONSEPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class GamingPlanResponsePacket : public ResponsePacket {
private:
	uint16_t unknown1;
	uint32_t planType;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x7DE;
	GamingPlanResponsePacket();
	virtual ~GamingPlanResponsePacket();

	__inline uint32_t getPlanType() const {
		return planType;
	}
	__inline void setPlanType(const uint32_t type) {
		planType = type;
	}
};

#endif //__GAMING_PLAN_RESPONSEPACKET__