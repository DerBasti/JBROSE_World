#ifndef __ROSE_MONSTER__
#define __ROSE_MONSTER__
#pragma once

#include "NPC.h"

class Monster : public NPC {
public:
	Monster(NPCDefaultStatValues* defaultValues, AIP* ai, const Position& spawnPosition);
	Monster(NPCDefaultStatValues* defaultValues, AIP* ai, MonsterRecoveryPoint* spawnPosition);
	virtual ~Monster();

	__inline bool isNPC() const {
		return false;
	}

	__inline bool isMonster() const {
		return true;
	}
};

#endif //__ROSE_MONSTER__