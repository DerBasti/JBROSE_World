#include "Monster.h"
#include "..\WorldPackets\Responses\SpawnMonsterVisuallyResponsePacket.h"

Monster::Monster(NPCDefaultStatValues* defaultValues, const Position& spawnPosition) : NPC(defaultValues, spawnPosition) {
	delete visualityProcessor; //from NPC
	visualityProcessor = new MonsterVisualityProcessor(this);
}

Monster::~Monster() {
}