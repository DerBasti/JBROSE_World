#include "Monster.h"
#include "..\WorldPackets\Responses\SpawnMonsterVisuallyResponsePacket.h"
#include "../Map/MonsterRecoveryPoint.h"

Monster::Monster(NPCDefaultStatValues* defaultValues, AIP* ai, MonsterRecoveryPoint* spawnPoint) : Monster(defaultValues, ai, spawnPoint->getRandomPositionFromSpawn()) {
	this->spawnPoint = spawnPoint;
}

Monster::Monster(NPCDefaultStatValues* defaultValues, AIP* ai, const Position& spawnPosition) : NPC(defaultValues, ai, spawnPosition) {
	logger.setLoggerName("Monster");
	if (visualityProcessor) {
		delete visualityProcessor; //from NPC
	}
	visualityProcessor = new MonsterVisualityProcessor(this);

	getCombat()->setTeamId(EntityDefaultTeamId::MONSTER);
}

Monster::~Monster() {
}


Monster* NPCCreationFactory::createMonster(const uint32_t id, const Position& spawnPosition) {
	return new Monster(npcDefaultStatValues[id], aiProtocols[npcDefaultStatValues[id]->getAiId()], spawnPosition);
}

Monster* NPCCreationFactory::createMonster(const uint32_t id, MonsterRecoveryPoint* spawn) {
	return new Monster(npcDefaultStatValues[id], aiProtocols[npcDefaultStatValues[id]->getAiId()], spawn);
}