#include "Skill.h"

Skill::Skill() {
	id = 0;
	level = 0;
}

Skill::Skill(uint16_t id, const SkillSTBFile* skillSTB) {
	this->id = id;
	level = skillSTB->getSkillLevel(id);
	requiredPointsPerLevelup = skillSTB->getRequiredSkillpointsPerLevelup(id);
}

template<uint16_t Amount>
bool SkillList<Amount>::addSkillToList(Skill skill) {
	for (uint16_t i = 0; i < getMaximumAmount(); i++) {
		if (!skills[i].isValid()) {
			skills[i] = skill;
			return true;
		}
	}
	return false;
}

template<uint16_t Amount>
const Skill* SkillList<Amount>::findSkillById(uint16_t id) const {
	for (uint16_t i = 0; i < getMaximumAmount(); i++) {
		if (skills[i].isValid() && (skills[i].getId() == id || skills[i].getBasicId() == id)) {
			return &skills[i];
		}
	}
	return nullptr;
}

template<uint16_t Amount>
bool SkillList<Amount>::findSkillAndIncreaseLevel(uint16_t id) {
	uint16_t slot = findSlotOfSkill(id);
	if (slot == INVALID_SKILL_SLOT) {
		return false;
	}
	if (skillSTB->getBasicSkillId(id) == skillSTB->getBasicSkillId(id + 1)) {
		skills[slot] = Skill(id + 1, skillSTB);
		return true;
	}
	return false;
}

template<uint16_t Amount>
uint16_t SkillList<Amount>::findSlotOfSkill(uint16_t skillId) const {
	for (uint16_t i = 0; i < getMaximumAmount(); i++) {
		if (skills[i].isValid() && skills[i].getId() == skillId) {
			return i;
		}
	}
	return INVALID_SKILL_SLOT;
}