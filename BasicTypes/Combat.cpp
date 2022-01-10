#include "Combat.h"
#include "Entity.h"
#include "../FileTypes/AIP.h"
#include "../Entities/NPC.h"
#include "../Map/Map.h"
#include "../WorldPackets/Responses/BasicAttackResponsePacket.h"
#include "../FileTypes/ZMO.h"
#include "../WorldServer.h"
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
	combatType = CombatType::NONE;
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
	if (getTarget() != nullptr && !getTarget()->isDrop()) {
		attackTimer.restart();
	}
}

DamageHit Combat::doBasicAttack() {
	randomizer.setNewBoundries(1, 5);
	uint16_t attackPower = owner->getStats()->getCombatValues()->getAttackPower();
	uint16_t enemyDefense = target->getStats()->getCombatValues()->getDefense();

	int16_t damage = 20;
	float ratio = attackPower / static_cast<float>(enemyDefense + attackPower + 1);
	if (getEntitySelf()->isPlayer() && target->isPlayer()) {
		damage = attackPower * ratio - (enemyDefense * (1.1f - ratio));
	}
	else {
		//Minimum Attackpower: 2
		//Minimum Defense: 5

		//Maximum Attackpower: 2000
		//Maximum Defense: 1500

		//2 * (2 - 5 + 700) / ((5-5) * 40 = 5.903
		//56 * (56 - 30 + 700) / ((30 / 8) = 13.01
		//11 * (11 - 30 + 700) / ((30 / 8) = 5.58


		//11 * 681 / (130 / 8) = sqrt(21.47) = 4,63 + 0,2
		//56 * 726 / (130 / 8) = sqrt(50.01) = 7,07 + 5,6
		//2000 * (2000 - 1500 + 700) / (1600 / 8) = 109,54 + 200

		damage = (sqrtf(attackPower * (attackPower - enemyDefense + 700) / ((enemyDefense+100) / 8.0f)) + (attackPower * 0.1f)) / 2;
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
				reach = 250;
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

bool Combat::isAttackingEnemy() const {
	auto entity = getTarget();
	return entity != nullptr && !entity->isDrop() && !entity->isNPC() && !entity->isAlliedTo(getEntitySelf());
}

void Combat::clear() {
	this->setTarget(nullptr, CombatType::NONE);
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
		this->target->getCombat()->removeFromTargetedByList(getEntitySelf());
	}
	this->target = target;
	if (target != nullptr && type != CombatTypeId::NONE) {
		target->getCombat()->addToTargetedByList(owner);
	}
	switch (getCombatType()) {
		case CombatTypeId::NONE:
			setAttackRoutine(nullptr);
			attackTimer.softStop();
			attackTimer.updateTimestamp();
		break;
		case CombatTypeId::BASIC_ATTACK:
		{
			auto attackCallback = [this]() -> bool {
				if (isWeaponAbleToAttack()) {
					return this->owner->doAttack();
				}
				return true;
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
	if (targetedByMap.empty()) {
		return;
	}
	auto iterator = targetedByMap.begin();
	while(iterator != targetedByMap.end()) {
		Entity* currentTarget = iterator->second;
		currentTarget->getCombat()->clear();
		currentTarget->getLocationData()->getMapPosition()->setDestinationPositionToCurrentPosition();
		iterator = targetedByMap.begin();
	}
	targetedByMap.clear();
}


bool Combat::isWeaponAbleToAttack() const {
	if (!getEntitySelf()->isPlayer()) {
		return true;
	}
	uint16_t typeId = dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId()).getCategoryId();
	switch (typeId) {
		case WeaponType::RANGE_BOW:
		case WeaponType::RANGE_CROSSBOW:
			return isBowReadyForAttack();
		case WeaponType::RANGE_LAUNCHER:
			return isLauncherReadyForAttack();
		case WeaponType::RANGE_GUN:
		case WeaponType::RANGE_DUAL_GUN:
			return isGunReadyForAttack();
	}
	return true;
}

bool Combat::isBowEquipped() const {
	uint16_t typeId = dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId()).getCategoryId();
	switch (typeId) {
		case WeaponType::RANGE_BOW:
		case WeaponType::RANGE_CROSSBOW:
			return true;
	}
	return false;
}

bool Combat::isBowReadyForAttack() const {
	return dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(InventorySlot::ARROWS).isValid();
}

bool Combat::isBowEquippedAndReadyForAttack() const {
	if (isBowEquipped()) {
		return isBowReadyForAttack();
	}
	return false;
}

bool Combat::isLauncherEquipped() const {
	uint16_t typeId = dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId()).getCategoryId();
	switch (typeId) {
		case WeaponType::RANGE_LAUNCHER:
			return true;
	}
	return false;
}

bool Combat::isLauncherReadyForAttack() const {
	return dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(InventorySlot::CANNONSHELLS).isValid();
}

bool Combat::isLauncherEquippedAndReadyForAttack() const {
	if (isLauncherEquipped()) {
		return isLauncherReadyForAttack();
	}
	return false;
}

bool Combat::isGunEquipped() const {
	uint16_t typeId = dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId()).getCategoryId();
	switch (typeId) {
		case WeaponType::RANGE_GUN:
		case WeaponType::RANGE_DUAL_GUN:
			return true;
	}
	return false;
}

bool Combat::isGunReadyForAttack() const {
	return dynamic_cast<Player*>(getEntitySelf())->getInventory()->getItem(InventorySlot::BULLETS).isValid();
}

bool Combat::isGunEquippedAndReadyForAttack() const {
	if (isLauncherEquipped()) {
		return isGunReadyForAttack();
	}
	return true;
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