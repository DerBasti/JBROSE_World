#include "SummonedMonster.h"


SummonedMonster::SummonedMonster(NPCDefaultStatValues* defaultValues, AIP* ai, const Position& spawnPosition) : Monster(defaultValues, ai, spawnPosition) {

}

SummonedMonster::~SummonedMonster() {

}

SummonedMonster* NPCCreationFactory::createSummonedMonster(const uint32_t id, const Position& spawnPosition, Entity* owner) {
	SummonedMonster* summon = new SummonedMonster(npcDefaultStatValues[id], aiProtocols[npcDefaultStatValues[id]->getAiId()], spawnPosition);
	summon->setOwner(owner);
	return summon;
}