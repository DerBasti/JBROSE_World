#ifndef __ROSE_MONSTER__
#define __ROSE_MONSTER__
#pragma once

#include "NPC.h"

class Monster : public NPC {
public:
	Monster(std::shared_ptr<NPCDefaultStatValues>& defaultValues, const Position& spawnPosition);
	virtual ~Monster();

	__inline bool isMonster() const {
		return true;
	}
};

#endif //__ROSE_MONSTER__