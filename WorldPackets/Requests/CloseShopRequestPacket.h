#ifndef __ROSE_CLOSE_SHOP_REQUEST_PACKET__
#define __ROSE_CLOSE_SHOP_REQUEST_PACKET__

#include "../../../JBROSE_Common/Packet.h"

class CloseShopRequestPacket : public Packet {
private:

public:
	const static uint16_t ID = 0x7c3;
	CloseShopRequestPacket(const Packet* packet);
	virtual ~CloseShopRequestPacket();
};

#endif //__ROSE_CLOSE_SHOP_REQUEST_PACKET__