#ifndef __ROSE_ENTITY__
#define __ROSE_ENTITY__
#pragma once

#include "Position.h"
#include "LocationData.h"
#include "Stance.h"
#include "Visuality.h"
#include "EntityStats.h"
#include "Combat.h"
#include "PositionProcessor.h"
#include <type_traits>
#include <map>


class Entity {
private:
	Combat *combat;
	LocationData* locationData;
	PositionProcessor* positionProcessor;
	bool ingameFlag;
protected:
	std::unordered_map<uint16_t, uint32_t> damageMap;
	class VisualityProcessor* visualityProcessor;
	ROSEThreadedLogger logger;
	Stance* stance;
	EntityStats* stats;

	virtual void updateAttackPower() { }
	virtual void updateDefense() { }
	virtual void updateMagicDefense() { }
	virtual void updateCriticalRate() { }
	virtual void updateAttackSpeed() { }
	virtual void updateAttackRange() { }
	virtual void updateMaximumHp() { }
	virtual void updateMaximumMp() { }
public:
	Entity();
	virtual ~Entity();

	PositionUpdateResult updateMovement();
	bool updateVisuality();

	virtual void onIdle() { }
	virtual void onMoving() { }
	virtual void onUpdate() { }

	virtual void updateCombatValues() {
		updateAttackAnimation();
		updateAttackPower();
		updateDefense();
		updateMagicDefense();
		updateCriticalRate();
		updateAttackSpeed();
		updateAttackRange();
		updateMovementSpeed();
		updateMaximumHp();
		updateMaximumMp();
	}
	virtual void updateMovementSpeed() { }
	virtual void updateAttackAnimation() { }

	bool doAttack();
	bool onNewTarget();
	virtual bool sendDataToSelf(const class ResponsePacket& packet) {
		return true;
	}

	virtual const char* getName() const;

	virtual void onDamageReceived(Entity* attacker, uint32_t damageAmount) { }
	virtual void onDeath() { 
		getCombat()->clearSelfFromTargetsCombat();
		getCombat()->clear();
	}

	virtual bool spawnVisually(Entity* entity);
	virtual bool despawnVisually(Entity* entity);
	virtual bool despawnVisually(uint16_t entityId);
	virtual bool updateStanceVisually();

	virtual bool sendDataToEntityMap(const std::map<uint16_t, Entity*>& entityMap, const class ResponsePacket& packet) const;
	virtual bool sendDataToVisibleEntities(const class ResponsePacket& packet) const;
	virtual bool sendDataToVisibleExceptSelf(const class ResponsePacket& packet) const;

	__inline LocationData* getLocationData() const {
		return locationData;
	}
	__inline VisualityProcessor* getVisualityProcessor() const {
		return visualityProcessor;
	}
	__inline PositionProcessor* getPostionProcessor() const {
		return positionProcessor;
	}

	__inline Stance* getStance() const {
		return stance;
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
	__inline virtual bool isAlliedTo(Entity* entity) {
		return getCombat()->getTeamId() == entity->getCombat()->getTeamId();
	}
	__inline bool isIngame() const {
		return getLocationData()->getLocalId() > 0;
	}
	__inline void setIngame(bool flag) {
		ingameFlag = flag;
	}
	__inline Combat* getCombat() const {
		return combat;
	}
	template<class _T = EntityStats, class = typename std::enable_if<std::is_base_of<EntityStats, _T>::value>::type>
	__inline _T* getStats() const {
		return dynamic_cast<_T*>(stats);
	}
};

class EntityComparator {
public:
	EntityComparator() {}
	virtual ~EntityComparator() {}

	bool operator()(const Entity* left, const Entity* right) const;
};

#endif //__ROSE_ENTITY__