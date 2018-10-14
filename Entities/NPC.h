#ifndef __ROSE_NPC__
#define __ROSE_NPC__
#pragma once

#include "..\BasicTypes\Entity.h"

class NPCDefaultStatValues {
private:
	uint16_t id;
	uint8_t level;
	uint16_t maxHp;
	uint16_t hpPerLevel;
	uint32_t experiencePoints;
	uint16_t walkingSpeed;
	uint16_t runningSpeed;
public:
	NPCDefaultStatValues(uint16_t id, class STBEntry* entry);
	virtual ~NPCDefaultStatValues();

	__inline uint16_t getId() const {
		return id;
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
};

class NPC : public Entity {
private:
	NPCDefaultStatValues* defaultStatValues;
public:
	NPC(NPCDefaultStatValues* defaultValues, const Position& spawnPosition);
	virtual ~NPC();

	__inline NPCDefaultStatValues* getDefaultStatValues() const {
		return defaultStatValues;
	}

	__inline bool isNPC() const {
		return true;
	}
};

#endif //__ROSE_NPC__