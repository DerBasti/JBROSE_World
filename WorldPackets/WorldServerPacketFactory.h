#ifndef __WORLD_SERVER_PACKETFACTORY__
#define __WORLD_SERVER_PACKETFACTORY__

#include "..\..\JBROSE_Common\PacketFactory.h"

class WorldServerPacketFactory : public PacketFactory {
public:
	WorldServerPacketFactory();
	virtual ~WorldServerPacketFactory();
};

#endif //__WORLD_SERVER_PACKETFACTORY__