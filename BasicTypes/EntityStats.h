#ifndef __ROSE_STATS__
#define __ROSE_STATS__

#include <cstdint>
#include "CombatValues.h"
#include "../../JBROSE_Common/WrappingNumeric.h"

class EntityStats {
private:
	uint16_t level;
	LimitingUInt healthPoints;
	LimitingUInt manaPoints;
	uint16_t movementSpeed;
protected:
	CombatValues *combatValues;
public:
	EntityStats() {
		level = 1;
		healthPoints.setMaximumLimit(100);
		healthPoints = 100;
		manaPoints.setMaximumLimit(100);
		manaPoints = 100;
		movementSpeed = 425;
		combatValues = new CombatValues();
	}
	virtual ~EntityStats() {
		delete combatValues;
		combatValues = nullptr;
	}
	virtual void addLevel() {
		setLevel(getLevel() + 1);
	}
	__inline uint32_t getLevel() const {
		return level;
	}
	__inline void setLevel(const uint32_t level) {
		this->level = level;
	}
	__inline uint32_t getMaxHp() const {
		return healthPoints.getMaximumLimit();
	}
	__inline void setMaxHp(uint32_t newMaxHp) {
		this->healthPoints.setMaximumLimit(newMaxHp);
	}
	__inline uint32_t getCurrentHp() const {
		return healthPoints;
	}
	__inline void addToCurrentHp(const uint32_t additionalHp) {
		setCurrentHp(getCurrentHp() + additionalHp);
	}
	__inline void setCurrentHp(const uint32_t currentHp) {
		healthPoints = currentHp;
	}
	void decreaseCurrentHpBy(const uint32_t damageAmount) {
		if (damageAmount >= getCurrentHp()) {
			setCurrentHp(0);
		}
		else {
			setCurrentHp(getCurrentHp() - damageAmount);
		}
	}
	__inline bool isDead() const {
		return getCurrentHp() <= 0;
	}
	__inline uint32_t getCurrentMp() const {
		return manaPoints;
	}
	__inline void setCurrentMp(const uint32_t currentMp) {
		manaPoints = currentMp;
	}
	__inline void addToCurrentMp(const uint32_t additionalMp) {
		setCurrentMp(getCurrentMp() + additionalMp);
	}
	__inline uint32_t getMaxMp() const {
		return manaPoints.getMaximumLimit();
	}
	__inline void setMaxMp(uint32_t newMaxMp) {
		this->manaPoints.setMaximumLimit(newMaxMp);
	}
	virtual void updateMaxHp() {

	}
	__inline uint16_t getMovementSpeed() const {
		return movementSpeed;
	}
	__inline void setMovementSpeed(const uint16_t speed) {
		movementSpeed = speed;
	}
	virtual void updateMovementSpeed() {
	}
	__inline CombatValues* getCombatValues() const {
		return combatValues;
	}
};

class PlayerStats : public EntityStats {
private:
	uint32_t experiencePoints;
	LimitingUShort stamina;
	uint16_t availableStatPoints;
	uint16_t availableSkillPoints;
	uint16_t maximumWeight;

	const static uint16_t MAX_STAMINA = 5000;
public:
	PlayerStats() {
		availableStatPoints = availableSkillPoints = 0;
		experiencePoints = 300;
		stamina = LimitingUShort(MAX_STAMINA, MAX_STAMINA);
		maximumWeight = 1100;
	}
	__inline void addExperience(const uint32_t expGained) {
		setExperiencePoints(getExperiencePoints() + expGained);
	}
	__inline uint32_t getExperiencePoints() const {
		return experiencePoints;
	}
	__inline void setExperiencePoints(const uint32_t experiencePoints) {
		this->experiencePoints = experiencePoints;
	}
	__inline uint16_t getStamina() const {
		return stamina;
	}
	__inline void setStamina(const uint16_t stamina) {
		this->stamina = stamina;
	}

	__inline uint16_t getAvailableStatPoints() const {
		return availableStatPoints;
	}
	__inline void setAvailableStatPoints(const uint16_t availableStatPoints) {
		this->availableStatPoints = availableStatPoints;
	}
	__inline void addAvailableStatPoints(uint16_t additionalPoints) {
		setAvailableStatPoints(getAvailableStatPoints() + additionalPoints);
	}

	__inline uint16_t getAvailableSkillPoints() const {
		return availableSkillPoints;
	}
	__inline void setAvailableSkillPoints(const uint16_t availableSkillPoints) {
		this->availableSkillPoints = availableSkillPoints;
	}
	__inline void addAvailableSkillPoints(uint16_t additionalPoints) {
		setAvailableSkillPoints(getAvailableSkillPoints() + additionalPoints);
	}
	__inline uint16_t getMaximumPossibleWeight() const {
		return maximumWeight;
	}
	__inline void setMaximumPossibleWeight(uint16_t newMaximum) {
		maximumWeight = newMaximum;
	}
};

#endif //