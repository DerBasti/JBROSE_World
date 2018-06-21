#include "WorldServerPacketFactory.h"
#include "RequestPackets.h"

WorldServerPacketFactory::WorldServerPacketFactory() {
	addCommand<AssignLocalPlayerIdRequestPacket>(AssignLocalPlayerIdRequestPacket::ID);
	addCommand<CollisionRequestPacket>(CollisionRequestPacket::ID);
	addCommand<ChangeRespawnTownRequestPacket>(ChangeRespawnTownRequestPacket::ID);
	addCommand<IdentifyAccountRequestPacket>(IdentifyAccountRequestPacket::ID);
	addCommand<NewDestinationRequestPacket>(NewDestinationRequestPacket::ID);
	addCommand<PingRequestPacket>(PingRequestPacket::ID);
	addCommand<TelegateRequestPacket>(TelegateRequestPacket::ID);
}

WorldServerPacketFactory::~WorldServerPacketFactory() {

}