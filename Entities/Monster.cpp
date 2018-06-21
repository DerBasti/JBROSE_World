#include "Monster.h"
#include "..\WorldPackets\Responses\SpawnMonsterVisuallyResponsePacket.h"

Monster::Monster(std::shared_ptr<NPCDefaultStatValues>& defaultValues, const Position& spawnPosition) : NPC(defaultValues, spawnPosition) {
	visualityProcessor = std::shared_ptr<MonsterVisualityProcessor>(new MonsterVisualityProcessor(this));
}

Monster::~Monster() {
}