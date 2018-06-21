#ifndef __ROSE_WORLD_ENCRYPTION_PACKET__
#define __ROSE_WORLD_ENCRYPTION_PACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class WorldEncryptionPacket : public ResponsePacket {
private:
	uint8_t unknown1;
	uint32_t encryptionKey;
	uint32_t unknown2;

	const static uint16_t DEFAULT_SIZE = 15;
protected:
	void appendContentToSendable(SendablePacket& packet) const {
		packet.addData(unknown1);
		packet.addData(encryptionKey);
		packet.addData(unknown2);
	}
public:
	const static uint16_t ID = 0x70C;
	WorldEncryptionPacket() : ResponsePacket(ID, DEFAULT_SIZE) {
		unknown1 = 0x00;
		encryptionKey = 0x00;
		unknown2 = 0x00;
	}
	virtual ~WorldEncryptionPacket() {

	}

	__inline void setEncryptionKey(const uint32_t encryption) {
		encryptionKey = encryption;
	}
};

#endif //__ROSE_WORLD_ENCRYPTION_PACKET__