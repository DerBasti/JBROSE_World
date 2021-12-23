#include "WorldServerPacketFactory.h"
#include "RequestPackets.h"

WorldServerPacketFactory::WorldServerPacketFactory() {
	addCommand<AssignLocalPlayerIdRequestPacket>(AssignLocalPlayerIdRequestPacket::ID);
	addCommand<CollisionRequestPacket>(CollisionRequestPacket::ID);
	addCommand<ChangeEquipmentRequestPacket>(ChangeEquipmentRequestPacket::ID);
	addCommand<ChangeRespawnTownRequestPacket>(ChangeRespawnTownRequestPacket::ID);
	addCommand<CurrentWeightRequestPacket>(CurrentWeightRequestPacket::ID);
	addCommand<DistributeStatPointRequestPacket>(DistributeStatPointRequestPacket::ID);
	addCommand<DropItemFromInventoryRequestPacket>(DropItemFromInventoryRequestPacket::ID);
	addCommand<ExitRequestPacket>(ExitRequestPacket::ID);
	addCommand<IdentifyAccountRequestPacket>(IdentifyAccountRequestPacket::ID);
	addCommand<InitBasicAttackRequestPacket>(InitBasicAttackRequestPacket::ID);
	addCommand<NewDestinationRequestPacket>(NewDestinationRequestPacket::ID);
	addCommand<PickupDropRequestPacket>(PickupDropRequestPacket::ID);
	addCommand<PingRequestPacket>(PingRequestPacket::ID);
	addCommand<ShowMonsterHpRequestPacket>(ShowMonsterHpRequestPacket::ID);
	addCommand<StanceRequestPacket>(StanceRequestPacket::ID);
	addCommand<TelegateRequestPacket>(TelegateRequestPacket::ID);
}

WorldServerPacketFactory::~WorldServerPacketFactory() {

}