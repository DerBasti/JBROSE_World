#include "MonsterRecoveryPoint.h"
#include "../Entities/Monster.h"
#include "Map.h"


MonsterRecoveryPoint::MonsterRecoveryPoint(std::shared_ptr<IFOMonsterSpawnEntry> spawnEntry, std::function<void(Monster*)> onNewMonsterFunction) {
	this->monsterSpawnedFunction = onNewMonsterFunction;
	this->spawnInformation = spawnEntry;
	spawnCheckTimer.setTimestamp(0);
	tacticalPointsGained = 0;
	currentBasicRoundId = 0;
	currentBasicRoundId.setMaximum(static_cast<uint32_t>(spawnEntry->getBasicRounds().size()));
	logger.setLoggerName("MonsterSpawnPoint");
	randomizer.setNewBoundries(-spawnInformation->getMaximumRadius(), spawnInformation->getMaximumRadius());
}

MonsterRecoveryPoint::~MonsterRecoveryPoint() {

}
void MonsterRecoveryPoint::spawnBasicRound(std::shared_ptr<IFOMonsterSpawnEntry::Round> basicRound) {
	for (uint32_t i = 0; i < basicRound->getMonsterAmount(); i++) {
		Position pos(spawnInformation->getPosition());
		pos.setX(pos.getX() + randomizer.generateRandomValue());
		pos.setY(pos.getY() + randomizer.generateRandomValue());
		Monster* newlySpawnedMonster = MonsterCreationFactory::createMonster(basicRound->getMonsterId(), this);
		onNewMonsterSpawned(newlySpawnedMonster);
		monstersSpawned.insert(std::make_pair(newlySpawnedMonster->getLocationData()->getLocalId(), newlySpawnedMonster));
	}
	logger.logDebug("Spawned a total of ", basicRound->getMonsterAmount(), " monsters of type (#", basicRound->getMonsterId(), ").");
}

void MonsterRecoveryPoint::spawnTacticRoundOnly(const std::vector<std::shared_ptr<IFOMonsterSpawnEntry::Round>>& tacticalRounds) {
	for (auto tacticalRound : tacticalRounds) {
		for (uint32_t i = 0; i < tacticalRound->getMonsterAmount(); i++) {
			Monster* newlySpawnedMonster = MonsterCreationFactory::createMonster(tacticalRound->getMonsterId(), this);
			onNewMonsterSpawned(newlySpawnedMonster);
			monstersSpawned.insert(std::make_pair(newlySpawnedMonster->getLocationData()->getLocalId(), newlySpawnedMonster));
		}
		logger.logDebug("Spawned a total of ", tacticalRound->getMonsterAmount(), " monsters of type (#", tacticalRound->getMonsterId(), ").");
	}
}

void MonsterRecoveryPoint::spawnTacticRoundWithGuards(const std::vector<std::shared_ptr<IFOMonsterSpawnEntry::Round>>& tacticalRounds, std::shared_ptr<IFOMonsterSpawnEntry::Round> lastBasicRound) {
	spawnTacticRoundOnly(tacticalRounds);
	spawnBasicRound(lastBasicRound);
}

void MonsterRecoveryPoint::checkForNewSpawns() {
	uint64_t timeDifference = spawnCheckTimer.getPassedTimeInMillis();
	std::vector<Monster*> listOfSpawnedMonsters;
	if (timeDifference >= spawnInformation->getRespawnInterval()) {
		while (monstersSpawned.size() <= spawnInformation->getMaximumAmountOfMonsters()) {
			const std::vector<std::shared_ptr<IFOMonsterSpawnEntry::Round>>& basicRounds = spawnInformation->getBasicRounds();
			MonsterSpawnTacticalType variation = calculateTacticalVariation();
			auto currentRound = basicRounds[currentBasicRoundId];
			switch (variation) {
				case MonsterSpawnTacticalType::BASIC_ONLY_ROUND:
					spawnBasicRound(currentRound);
					currentBasicRoundId++;
				break;
				case MonsterSpawnTacticalType::TACTICAL_ONLY_ROUND:
					spawnTacticRoundOnly(spawnInformation->getTacticalRounds());
					tacticalPointsGained = 1;
				break;
				case MonsterSpawnTacticalType::TACTICAL_WITH_GUARDS_ROUND:
					spawnTacticRoundWithGuards(spawnInformation->getTacticalRounds(), basicRounds.at(basicRounds.size() - 1));
					tacticalPointsGained = 1;
				break;
			}
		}
		tacticalPointsGained++;
		spawnCheckTimer.updateTimestamp();
	}
}


MonsterSpawnTacticalType MonsterRecoveryPoint::calculateTacticalVariation() {
	uint32_t maximumAmount = (spawnInformation->getMaximumAmountOfMonsters() * spawnInformation->getTacticalPointsNecessary());
	uint32_t variationAmount = (spawnInformation->getMaximumAmountOfMonsters() - static_cast<uint32_t>(monstersSpawned.size())) * tacticalPointsGained * (spawnInformation->getTacticalPointsNecessary()/2);
	uint32_t percentageDone = static_cast<uint32_t>(variationAmount * 100.0f / static_cast<double>(maximumAmount));
	logger.logTrace("Percentage done till tactical: ", percentageDone);

	//Spawn tacticals after 1
	MonsterSpawnTacticalType type = MonsterSpawnTacticalType::BASIC_ONLY_ROUND;
	if (!spawnInformation->getTacticalRounds().empty()) {
		if (percentageDone >= 0.75f && percentageDone <= 0.9f) {
			type = MonsterSpawnTacticalType::TACTICAL_ONLY_ROUND;
		}
		else if (percentageDone > 0.9f) {
			type = MonsterSpawnTacticalType::TACTICAL_WITH_GUARDS_ROUND;
		}
	}
	return type;
}