#ifndef __CURRENT_WEIGHT_RESPONSEPACKET__
#define __CURRENT_WEIGHT_RESPONSEPACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class CurrentWeightResponsePacket : public ResponsePacket {
private:
	uint16_t localPlayerId;
	uint8_t weightPercentage;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x762;
	const static uint16_t DEFAULT_LENGTH = 10;

	CurrentWeightResponsePacket();
	virtual ~CurrentWeightResponsePacket();
	virtual std::string toPrintable() const;

	__inline uint16_t getLocalPlayerId() const {
		return localPlayerId;
	}
	__inline void setLocalEntityId(const uint16_t id) {
		localPlayerId = id;
	}
	__inline uint8_t getWeightPercentage() const {
		return weightPercentage;
	}
	__inline void setWeightPercentage(const uint8_t percentage) {
		weightPercentage = percentage;
	}
};

#endif //__CURRENT_WEIGHT_RESPONSEPACKET__