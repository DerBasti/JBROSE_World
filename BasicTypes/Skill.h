#ifndef __ROSE_SKILL__
#define __ROSE_SKILL__

#include <inttypes.h>
#include <memory>
#include <limits>
#include "../FileTypes/STB.h"

class Skill { 
private:
	uint16_t id;
	uint8_t level;
	uint16_t requiredPointsPerLevelup;
public:
	Skill();
	Skill(uint16_t id, const SkillSTBFile* skillSTB);

	virtual ~Skill() {

	}

	__inline uint16_t getId() const {
		return id;
	}
	__inline uint16_t getBasicId() const {
		return (id - level + 1);
	}
	__inline uint16_t getLevel() const {
		return level;
	}
	__inline uint16_t getRequiredSkillpointsPerLevelup() const {
		return requiredPointsPerLevelup;
	}
	__inline bool isValid() const {
		return id == 0 || level == 0;
	}
};

template<uint16_t AMOUNT>
class SkillList {
private:
	Skill skills[AMOUNT];
	const SkillSTBFile* skillSTB;

	uint16_t findSlotOfSkill(uint16_t skillId) const;
public:
	constexpr static uint16_t INVALID_SKILL_SLOT = (std::numeric_limits<uint16_t>::max)();
	SkillList() : SkillList<AMOUNT>(nullptr) {

	}
	SkillList(const SkillSTBFile* skillSTB) {
		this->skillSTB = skillSTB;
	}

	virtual ~SkillList() {

	}

	bool addSkillToList(Skill skill);
	const Skill* findSkillById(uint16_t id) const;
	bool findSkillAndIncreaseLevel(uint16_t id);

	__inline const Skill* getSkillBySlot(uint16_t slot) const {
		if (slot < getMaximumAmount()) {
			return &skills[slot];
		}
		return nullptr;
	}

	__inline uint16_t getMaximumAmount() const {
		return AMOUNT;
	}
};

#endif //__ROSE_SKILL__