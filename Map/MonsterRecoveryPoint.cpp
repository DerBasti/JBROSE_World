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
	averageTacticalPointsGiven = (uint32_t)std::round(spawnInformation->getTacticalPointsNecessary() / static_cast<float>(spawnInformation->getAverageMonsterAmountPerSpawn() * spawnInformation->getBasicRounds().size()));
	averageTacticalPointsGiven = (uint32_t)(averageTacticalPointsGiven * sqrtf(15.0f / (float)averageTacticalPointsGiven));
	logger.setLoggerName("MonsterSpawnPoint");
	randomizer.setNewBoundries(-spawnInformation->getMaximumRadius(), spawnInformation->getMaximumRadius());
}

MonsterRecoveryPoint::~MonsterRecoveryPoint() {

}
void MonsterRecoveryPoint::spawnBasicRound(std::shared_ptr<IFOMonsterSpawnEntry::Round> basicRound) {
	for (uint32_t i = 0; i < basicRound->getMonsterAmount(); i++) {
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
	if (timeDifference >= spawnInformation->getRespawnInterval()) {
		while (monstersSpawned.size() <= spawnInformation->getMaximumAmountOfMonsters()) {
			const std::vector<std::shared_ptr<IFOMonsterSpawnEntry::Round>>& basicRounds = spawnInformation->getBasicRounds();
			MonsterSpawnTacticalType variation = MonsterSpawnTacticalType::BASIC_ONLY_ROUND;
			if (spawnInformation->hasTacticalRounds()) {
				variation = calculateTacticalVariation();
				tacticalPointsGained += averageTacticalPointsGiven;
			}
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
		spawnCheckTimer.updateTimestamp();
	}
}


MonsterSpawnTacticalType MonsterRecoveryPoint::calculateTacticalVariation() {
	float percentageDone = static_cast<float>(tacticalPointsGained / static_cast<double>(spawnInformation->getTacticalPointsNecessary()));
	logger.logDebug("Percentage done till tactical: ", (percentageDone*100));

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