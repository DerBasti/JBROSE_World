#ifndef __ROSE_COMBAT_VALUES__
#define __ROSE_COMBAT_VALUES__

#include <cstdint>

class CombatValues {
private:
	uint16_t attackPower;
	uint16_t defense;
	uint16_t magicDefense;
	uint16_t critRate;
	uint16_t attackSpeed;
	uint32_t attackRange;
public:
	CombatValues() {
		attackPower = 30;
		defense = 10;
		magicDefense = 10;
		critRate = 15;
		attackSpeed = 115;
		attackRange = 100;
	}
	virtual ~CombatValues() {

	}

	__inline uint16_t getAttackPower() const {
		return attackPower;
	}
	__inline void setAttackPower(const uint16_t power) {
		attackPower = power;
	}
	__inline uint16_t getDefense() const {
		return defense;
	}
	__inline void setDefense(uint16_t newDefense) {
		defense = newDefense;
	}
	__inline uint16_t getMagicDefense() const {
		return magicDefense;
	}
	__inline void setMagicDefense(uint16_t newDefense) {
		magicDefense = newDefense;
	}
	__inline uint16_t getCritRate() const {
		return critRate;
	}
	__inline void setCritRate(uint16_t newCritRate) {
		critRate = newCritRate;
	}
	__inline uint16_t getAttackSpeed() const {
		return attackSpeed;
	}
	__inline void setAttackSpeed(const uint16_t speed) {
		attackSpeed = speed;
	}
	__inline uint16_t getAttackRange() const {
		return attackRange;
	}
	__inline void setAttackRange(const uint16_t newRange) {
		attackRange = newRange;
	}
};

#endif //__ROSE_COMBAT_VALUES__