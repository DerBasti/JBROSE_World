#ifndef __ROSE_STATS__
#define __ROSE_STATS__

#include <cstdint>
#include "CombatValues.h"

class EntityStats {
private:
	uint16_t level;
	uint32_t currentHp;
	uint32_t maxHp;
	uint32_t currentMp;
	uint32_t maxMp;
	uint16_t movementSpeed;
protected:
	CombatValues *combatValues;
public:
	EntityStats() {
		level = 1;
		currentHp = currentMp = 100;
		maxHp = maxMp = 100;
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
		return maxHp;
	}
	__inline void setMaxHp(uint32_t newMaxHp) {
		this->maxHp = newMaxHp;
	}
	__inline uint32_t getCurrentHp() const {
		return currentHp;
	}
	__inline void addToCurrentHp(const uint32_t additionalHp) {
		setCurrentHp(getCurrentHp() + additionalHp);
	}
	__inline void setCurrentHp(const uint32_t currentHp) {
		this->currentHp = currentHp;
		if (this->currentHp > getMaxHp()) {
			this->currentHp = getMaxHp();
		}
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
		return currentMp;
	}
	__inline void setCurrentMp(const uint32_t currentMp) {
		this->currentMp = currentMp;
		if (this->currentMp > getMaxMp()) {
			this->currentMp = getMaxMp();
		}
	}
	__inline void addToCurrentMp(const uint32_t additionalMp) {
		setCurrentMp(getCurrentMp() + additionalMp);
	}
	__inline uint32_t getMaxMp() const {
		return maxMp;
	}
	__inline void setMaxMp(uint32_t newMaxMp) {
		this->maxMp = newMaxMp;
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
	uint16_t stamina;
	uint16_t availableStatPoints;
	uint16_t availableSkillPoints;
	uint16_t maximumWeight;
public:
	PlayerStats() {
		availableStatPoints = availableSkillPoints = 0;
		experiencePoints = 0;
		stamina = 5000;
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