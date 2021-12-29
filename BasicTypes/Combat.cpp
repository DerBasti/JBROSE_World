#include "Combat.h"
#include "Entity.h"
#include "../FileTypes/AIP.h"
#include "../Entities/NPC.h"
#include "../Map/Map.h"
#include "../WorldPackets/Responses/BasicAttackResponsePacket.h"
#include "../FileTypes/ZMO.h"
#include <type_traits>

const CombatType CombatType::NONE = CombatType(CombatTypeId::NONE);
const CombatType CombatType::BASIC_ATTACK = CombatType(CombatTypeId::BASIC_ATTACK);
const CombatType CombatType::SKILL_ATTACK = CombatType(CombatTypeId::SKILL_ATTACK);

CombatType::CombatType() : CombatType(CombatTypeId::NONE) {

}

CombatType::~CombatType() {

}

Combat::Combat(Entity* owner) {
	this->attackTimer.hardStop();
	this->attackTimer.setDurationForProcInMillis(600);
	this->attackTimer.setDurationForWrappingInMillis(1200);
	this->owner = owner;
	target = nullptr;
	teamId = 0;
}

Combat::~Combat() {
	owner = nullptr;
	target = nullptr;
}

float Combat::getAttackTimeInMilliseconds() const {
	return getAttackTimeRatio() * 1000.0f;
}

float Combat::getAttackTimeRatio() const {
	float attackspeedModifier = 100.0f / static_cast<float>(owner->getStats()->getCombatValues()->getAttackSpeed());
	return attackspeedModifier;
}

void Combat::onMovementUpdate() {
	refresh();
}

void Combat::onTargetReached() {
	if (getTarget() != nullptr) {
		attackTimer.restart();
	}
}

DamageHit Combat::doBasicAttack() {
	randomizer.setNewBoundries(1, 5);
	uint16_t attackPower = owner->getStats()->getCombatValues()->getAttackPower();
	uint16_t enemyDefense = target->getStats()->getCombatValues()->getDefense();

	int16_t damage = 20;
	float ratio = attackPower / static_cast<float>(enemyDefense + attackPower + 1);
	if (target->isPlayer()) {
		damage = attackPower * ratio - (enemyDefense * (1.1f - ratio));
	}
	else {
		damage = attackPower * ratio;
	}
	damage += randomizer.generateRandomValue();
	if (damage < 5) {
		damage = 5;
	}
	DamageHit hit(owner, target, damage);
	logger.logDebug("Attacking for: ", hit.getDamageAmount(), " damage.");
	target->getStats()->decreaseCurrentHpBy(damage);
	target->onDamageReceived(owner, damage);
	if (target->getStats()->isDead()) {
		hit.setDeadlyHitFlag();
		target->onDeath();
		clear();
	}
	return hit;
}

bool Combat::isTargetInReach() const {
	if (target == nullptr) {
		return false;
	}
	float distance = PositionProcessor::getDistanceBetweenPoints(owner->getLocationData()->getMapPosition()->getCurrentPosition(),
		target->getLocationData()->getMapPosition()->getCurrentPosition());

	const static float defaultReach = 150;
	float reach = defaultReach;
	switch (getCombatType()) {
		case CombatTypeId::NONE:
			if (target->isDrop()) {
				reach = 300;
			}
		break;
		case CombatTypeId::BASIC_ATTACK:
			reach = owner->getStats()->getCombatValues()->getAttackRange();
		break;
		default:
			logger.logWarn("Target in reach not applicable for combat type '", static_cast<uint16_t>(getCombatType().getTypeId()), "'");
	}
	return distance <= reach;
}

void Combat::clear() {
	target = nullptr;
	combatType = CombatType::NONE;
	attackTimer.softStop();
}

void Combat::onAttackspeedUpdate() {
	if (!isAttackRunning()) {
		return;
	}
	switch (getCombatType()) {
		case CombatTypeId::BASIC_ATTACK:
		{
			float ratio = getAttackTimeRatio();
			this->attackTimer.setDurationForProcInMillis(static_cast<uint64_t>(attackAnimation->getAttackFrames().getCurrentFrame().getTriggerTimepointInMillis() * ratio));
			this->attackTimer.setDurationForWrappingInMillis(static_cast<uint64_t>(attackAnimation->getDefaultPlayTime() * ratio));
		}
		break;
		default:
			logger.logWarn("No attackspeed update necessary for attacktype: ", getCombatType().getTypeId());
	}
}

void Combat::setAttackRoutine(std::function<bool()> proc) {
	attackTimer.hardStop();
	switch (getCombatType()) {
		case CombatTypeId::NONE:
			attackTimer.setCombatProcMethod(nullptr);
		break;
		case CombatTypeId::BASIC_ATTACK:
		{
			uint64_t totalTime = static_cast<uint64_t>(getAttackTimeInMilliseconds()) + 300;
			uint64_t procTime = 500;
			if (attackAnimation != nullptr) {
				float ratio = getAttackTimeRatio();
				procTime = static_cast<uint64_t>(attackAnimation->getAttackFrames().getCurrentFrame().getTriggerTimepointInMillis() * ratio);
				totalTime = static_cast<uint64_t>(attackAnimation->getDefaultPlayTime() * ratio);
			}
			logger.logDebug("[", owner->getName(), " #", owner->getLocationData()->getLocalId(), "] Total time: ", totalTime, "ms. ProcTime: ", procTime, "ms.");
			this->attackTimer.setDurationForProcInMillis(procTime);
			this->attackTimer.setDurationForWrappingInMillis(totalTime);
			attackTimer.setCombatProcMethod(proc);
		}
		break;
		default:
			logger.logWarn("Unknown attack type for AttackRoutine: ", getCombatType().getTypeId());
	}
}

void Combat::setTarget(Entity* target, const CombatType& type) {
	this->combatType = type;
	if (this->target != nullptr) {
		this->target->getCombat()->removeFromTargetedByList(owner);
	}
	this->target = target;
	if (target != nullptr && type != CombatTypeId::NONE) {
		target->getCombat()->addToTargetedByList(owner);
	}
	switch (getCombatType()) {
		case CombatTypeId::NONE:
			setAttackRoutine(nullptr);
		break;
		case CombatTypeId::BASIC_ATTACK:
		{
			auto attackCallback = [this]() -> bool {
				return this->owner->doAttack();
			};
			setAttackRoutine(attackCallback);
		}
		break;
		default:
			logger.logWarn("Unknown attack type for SetTarget: ", getCombatType().getTypeId());
	}
	if (type != CombatType::NONE) {
		this->owner->onNewTarget();
	}
}

void Combat::addToTargetedByList(Entity* entity) {
	auto it = targetedByMap.find(entity->getLocationData()->getLocalId());
	if (it == targetedByMap.cend()) {
		targetedByMap.emplace(entity->getLocationData()->getLocalId(), entity);
	}
}

void Combat::removeFromTargetedByList(Entity* entity) {
	auto it = targetedByMap.find(entity->getLocationData()->getLocalId());
	if (it != targetedByMap.cend()) {
		targetedByMap.erase(entity->getLocationData()->getLocalId());
	}
}

void Combat::clearSelfFromTargetsCombat() {
	for (auto pair : targetedByMap) {
		pair.second->getCombat()->clear();
		pair.second->getCombat()->removeFromTargetedByList(owner);
	}
	targetedByMap.clear();
}

std::ostream& operator<<(std::ostream& out, const CombatType& type) {
	out << type.getTypeId();
	return out;
}

std::wostream& operator<<(std::wostream& out, const CombatType& type) {
	out << type.getTypeId();
	return out;
}

std::ostream& operator<<(std::ostream& out, const CombatTypeId& type) {
	out << static_cast<uint8_t>(type);
	return out;
}
std::wostream& operator<<(std::wostream& out, const CombatTypeId& type) {
	out << static_cast<uint8_t>(type);
	return out;
}