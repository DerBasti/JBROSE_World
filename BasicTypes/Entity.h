#ifndef __ROSE_ENTITY__
#define __ROSE_ENTITY__
#pragma once

#include "Position.h"
#include "LocationData.h"
#include "Visuality.h"

class Entity {
private:
	std::shared_ptr<LocationData> locationData;
	std::shared_ptr<PositionProcessor> positionProcessor;
	bool ingameFlag;
protected:
	std::shared_ptr<class VisualityProcessor> visualityProcessor;
public:
	Entity();
	virtual ~Entity();

	bool updateMovement();
	bool updateVisuality();

	virtual bool spawnVisually(Entity* entity);
	virtual bool despawnVisually(Entity* entity);
	virtual bool despawnVisually(uint16_t entityId);

	__inline std::shared_ptr<LocationData> getLocationData() const {
		return locationData;
	}
	__inline std::shared_ptr<class VisualityProcessor> getVisualityProcessor() const {
		return visualityProcessor;
	}

	__inline virtual bool isPlayer() const {
		return false;
	}
	__inline virtual bool isNPC() const {
		return false;
	}
	__inline virtual bool isMonster() const {
		return false;
	}
	__inline virtual bool isDrop() const {
		return false;
	}
	__inline bool isIngame() const {
		return ingameFlag;
	}
	__inline void setIsIngame(bool ingame) {
		ingameFlag = ingame;
	}
};

class EntityComparator {
public:
	EntityComparator() {}
	virtual ~EntityComparator() {}

	bool operator()(const Entity* left, const Entity* right) const;
};

#endif //__ROSE_ENTITY__