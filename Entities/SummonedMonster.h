#ifndef __ROSE_SUMMONED_MONSTER__
#define __ROSE_SUMMONED_MONSTER__

#include "Monster.h"

class SummonedMonster : public Monster {
private:
	Entity* owner;
public:
	SummonedMonster(NPCDefaultStatValues* defaultValues, AIP* ai, const Position& spawnPosition);
	virtual ~SummonedMonster();

	__inline Entity* getOwner() const {
		return owner;
	}
	__inline void setOwner(Entity* newOwner) {
		this->owner = newOwner;
	}

	virtual bool isSummoned() const {
		return true;
	}
};

#endif //__ROSE_SUMMONED_MONSTER__