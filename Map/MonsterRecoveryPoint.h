#ifndef __ROSE_MONSTERSPAWN__
#define __ROSE_MONSTERSPAWN__

#include <unordered_map>
#include <memory>
#include "../FileTypes/IFO.h"
#include "../../JBROSE_Common/Randomizer.h"
#include "../../JBROSE_Common/WrappingNumeric.h"

enum class MonsterSpawnTacticalType : uint32_t {
	BASIC_ONLY_ROUND = 0,
	TACTICAL_ONLY_ROUND = 1,
	TACTICAL_WITH_GUARDS_ROUND = 2
};

class MonsterRecoveryPoint {
private:
	ROSEThreadedLogger logger;
	std::function<void(class Monster*)> monsterSpawnedFunction;
	std::unordered_map<uint16_t, class Monster*> monstersSpawned;
	std::shared_ptr<IFOMonsterSpawnEntry> spawnInformation;
	Timer spawnCheckTimer;
	uint32_t tacticalPointsGained;
	uint32_t averageTacticalPointsGiven;
	WrappingUInt currentBasicRoundId;
	MonsterSpawnTacticalType calculateTacticalVariation();
	NumericRandomizer<float> randomizer;

	__inline void onNewMonsterSpawned(Monster *monster) {
		if (monsterSpawnedFunction) {
			monsterSpawnedFunction(monster);
		}
	}

	void spawnBasicRound(std::shared_ptr<IFOMonsterSpawnEntry::Round> basicRound);
	void spawnTacticRoundOnly(const std::vector<std::shared_ptr<IFOMonsterSpawnEntry::Round>>& tacticalRounds);
	void spawnTacticRoundWithGuards(const std::vector<std::shared_ptr<IFOMonsterSpawnEntry::Round>>& tacticalRounds, std::shared_ptr<IFOMonsterSpawnEntry::Round> lastBasicRound);
public:
	MonsterRecoveryPoint(std::shared_ptr<IFOMonsterSpawnEntry> spawnEntry, std::function<void(Monster*)> onNewMonsterFunction);
	virtual ~MonsterRecoveryPoint();

	void checkForNewSpawns();
	Position getRandomPositionFromSpawn();

	__inline void removeFromActivelySpawned(const uint16_t monsterLocalId) {
		monstersSpawned.erase(monsterLocalId);
	}
	__inline const Position& getPosition() const {
		return spawnInformation->getPosition();
	}
};

#endif //__ROSE_MONSTERSPAWN__