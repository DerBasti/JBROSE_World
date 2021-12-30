#ifndef __ROSE_NPC__
#define __ROSE_NPC__
#pragma once

#include "..\BasicTypes\Entity.h"

class NPCDefaultStatValues {
private:
	uint16_t id;
	const char* name;
	uint8_t level;
	float size;
	uint16_t maxHp;
	uint16_t attackPower;
	uint16_t defense;
	uint16_t magicDefense;
	uint16_t attackSpeed;
	uint16_t attackRange;
	uint16_t dropItemRow;
	uint16_t dropItemChance;
	uint16_t dropMoneyChance;
	bool npcFlag;
	uint16_t hpPerLevel;
	uint32_t experiencePoints;
	uint16_t walkingSpeed;
	uint16_t runningSpeed;
	uint32_t aiId;
public:
	NPCDefaultStatValues(uint16_t id, class STBEntry* entry);
	virtual ~NPCDefaultStatValues();

	__inline uint16_t getId() const {
		return id;
	}
	__inline const char* getName() const {
		return name;
	}
	__inline float getSize() const {
		return size;
	}
	__inline uint8_t getLevel() const {
		return level;
	}
	__inline bool isNpc() const {
		return npcFlag;
	}
	__inline uint16_t getWalkingSpeed() const {
		return walkingSpeed;
	}
	__inline uint16_t getRunningSpeed() const {
		return runningSpeed;
	}
	__inline uint32_t getMaxHP() const {
		return maxHp;
	}
	__inline uint32_t getExperiencePoints() const {
		return experiencePoints;
	}
	__inline uint32_t getAiId() const {
		return aiId;
	}
	__inline uint16_t getAttackPower() const {
		return attackPower;
	}
	__inline uint16_t getDefense() const {
		return defense;
	}
	__inline uint16_t getMagicDefense() const {
		return magicDefense;
	}
	__inline uint16_t getAttackSpeed() const {
		return attackSpeed;
	}
	__inline uint16_t getAttackRange() const {
		return attackRange;
	}
	__inline uint16_t getDefaultMoneyDropChance() const {
		return dropMoneyChance;
	}
	__inline uint16_t getDefaultOwnDropChance() const {
		return dropItemChance;
	}
	__inline uint16_t getDropTableRowId() const {
		return dropItemRow;
	}
};

class MonsterRecoveryPoint;

class NPCCreationFactory {
private:
	NPCCreationFactory() {}
	static std::unordered_map<uint32_t, NPCDefaultStatValues*> npcDefaultStatValues;
	static std::unordered_map<uint32_t, class AIP*> aiProtocols;
	std::mutex mapMutex;
public:
	virtual ~NPCCreationFactory() {}
	static void initializeFromSTB(class STBFile *npcStbFile, STBFile* aiStbFile);
	static void deleteAllEntries();

	static class NPC* createNpc(const uint32_t id, const Position& spawnPosition);
	static class NPC* createNpc(const uint32_t id, MonsterRecoveryPoint* spawn);
	static class Monster* createMonster(const uint32_t id, const Position& spawnPosition);
	static class Monster* createMonster(const uint32_t id, MonsterRecoveryPoint* spawn);

	static NPCDefaultStatValues* getNPCDefaultValue(const uint32_t id) {
		return (npcDefaultStatValues.find(id) == npcDefaultStatValues.cend() ? nullptr : npcDefaultStatValues.at(id));
	}

	static AIP* getAi(const uint32_t aiId) {
		return (aiProtocols.find(aiId) == aiProtocols.cend() ? nullptr : aiProtocols.at(aiId));
	}
};

typedef NPCCreationFactory MonsterCreationFactory;


class NPC : public Entity {
private:
	class AIPProcessor* aiProcessor;
	class AIP* aiProtocol;
	NPCDefaultStatValues* defaultStatValues;
protected:
	virtual void updateAttackPower();
	virtual void updateDefense();
	virtual void updateMagicDefense();
	virtual void updateCriticalRate();
	virtual void updateAttackSpeed();
	virtual void updateAttackRange();
	MonsterRecoveryPoint* spawnPoint;
public:
	NPC(NPCDefaultStatValues* defaultValues, AIP* aiProtocol, const Position& spawnPosition);
	NPC(NPCDefaultStatValues* defaultValues, AIP* aiProtocol, MonsterRecoveryPoint* spawn);
	virtual ~NPC();

	virtual void updateMovementSpeed();
	virtual void updateAttackAnimation();

	virtual void onDamageReceived(Entity* attacker, uint32_t damageAmount);
	virtual void onDeath();
	virtual void onIdle();
	virtual void onMoving();
	virtual void onUpdate() { }
	
	__inline void updateAiProcessor(enum class AIEvent eventType) {
		updateAiProcessor(eventType, nullptr);
	}
	void updateAiProcessor(enum class AIEvent eventType, class Entity* designatedTarget);
	void updateAiTriggerTime();

	virtual const char* getName() const;

	__inline virtual bool isNPC() const {
		return true;
	}
	__inline NPCDefaultStatValues* getDefaultStatValues() const {
		return defaultStatValues;
	}

	__inline AIP* getArtificialIntelligence() const {
		return aiProtocol;
	}
	__inline MonsterRecoveryPoint* getMonsterRecoveryPoint() const {
		return spawnPoint;
	}
};

#endif //__ROSE_NPC__