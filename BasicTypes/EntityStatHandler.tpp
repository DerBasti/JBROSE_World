#include <functional>
#include "OperationHandler.h"
#include "Entity.h"
#include "../WorldClient.h"

template<class _Numeric, class X>
GetterAndSetterTuple<_Numeric> EntityStatHandler::getNumericGetterAndSetterOfEntityByEntityAbilityType(Entity* entity, EntityAbilityType type) {
	switch (type) {
		case EntityAbilityTypeId::CURRENT_HP:
			return GetterAndSetterTuple<_Numeric>(std::bind(&EntityStats::getCurrentHp, entity->getStats()), std::bind(&EntityStats::setCurrentHp, entity->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::CURRENT_MP:
			return GetterAndSetterTuple<_Numeric>(std::bind(&EntityStats::getCurrentMp, entity->getStats()), std::bind(&EntityStats::setCurrentMp, entity->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::ATTACK_POWER:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getAttackPower, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setAttackPower, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::DEFENSE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getDefense, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setDefense, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::ACCURACY:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getAccuracy, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setAccuracy, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::MAGIC_RESISTENCE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getMagicDefense, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setMagicDefense, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::DODGE_RATE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getDodgeRate, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setDodgeRate, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::MOVEMENT_SPEED:
			return GetterAndSetterTuple<_Numeric>(std::bind(&EntityStats::getMovementSpeed, entity->getStats()), std::bind(&EntityStats::setMovementSpeed, entity->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::ATTACK_SPEED:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getAttackSpeed, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setAttackSpeed, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::CRITICAL_RATE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&CombatValues::getCritRate, entity->getStats()->getCombatValues()), std::bind(&CombatValues::setCritRate, entity->getStats()->getCombatValues(), std::placeholders::_1));
		case EntityAbilityTypeId::LEVEL:
			return GetterAndSetterTuple<_Numeric>(std::bind(&EntityStats::getLevel, entity->getStats()), std::bind(&EntityStats::setLevel, entity->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::POINT:
		case EntityAbilityTypeId::TENDENCY:
		case EntityAbilityTypeId::PK_LEVEL:
			return GetterAndSetterTuple<_Numeric>(std::function<_Numeric()>(), std::function<void(_Numeric)>());
		case EntityAbilityTypeId::MAX_HP:
			return GetterAndSetterTuple<_Numeric>(std::bind(&EntityStats::getMaxHp, entity->getStats()), std::bind(&EntityStats::setMaxHp, entity->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::MAX_MP:
			return GetterAndSetterTuple<_Numeric>(std::bind(&EntityStats::getMaxMp, entity->getStats()), std::bind(&EntityStats::setMaxMp, entity->getStats(), std::placeholders::_1));
	}
	return GetterAndSetterTuple<_Numeric>();
}

template<class _Numeric, class X>
GetterAndSetterTuple<_Numeric> EntityStatHandler::getNumericGetterAndSetterOfPlayerByEntityAbilityType(Player* player, EntityAbilityType type) {
	GetterAndSetterTuple<_Numeric> fromEntity = EntityStatHandler::getNumericGetterAndSetterOfEntityByEntityAbilityType<_Numeric>(dynamic_cast<Entity*>(player), type);
	if(fromEntity.isValid()) {
		return fromEntity;
	}
	switch (type) {
		case EntityAbilityTypeId::BIRTHSTONE_TYPE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerTraits::getBirthstone, player->getTraits()), std::bind(&PlayerTraits::setBirthstone, player->getTraits(), std::placeholders::_1));
		case EntityAbilityTypeId::JOB:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerTraits::getJobNumeric, player->getTraits()), std::bind(&PlayerTraits::setJobNumeric, player->getTraits(), std::placeholders::_1));
		case EntityAbilityTypeId::STRENGTH:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerAttributes::getPointsLearned, player->getAttributes()->getStrength()), std::bind(&PlayerAttributes::setPointsLearned, player->getAttributes()->getStrength(), std::placeholders::_1));
		case EntityAbilityTypeId::DEXTERITY:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerAttributes::getPointsLearned, player->getAttributes()->getDexterity()), std::bind(&PlayerAttributes::setPointsLearned, player->getAttributes()->getDexterity(), std::placeholders::_1));
		case EntityAbilityTypeId::INTELLIGENCE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerAttributes::getPointsLearned, player->getAttributes()->getIntelligence()), std::bind(&PlayerAttributes::setPointsLearned, player->getAttributes()->getIntelligence(), std::placeholders::_1));
		case EntityAbilityTypeId::CONCENTRATION:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerAttributes::getPointsLearned, player->getAttributes()->getConcentration()), std::bind(&PlayerAttributes::setPointsLearned, player->getAttributes()->getConcentration(), std::placeholders::_1));
		case EntityAbilityTypeId::CHARM:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerAttributes::getPointsLearned, player->getAttributes()->getCharm()), std::bind(&PlayerAttributes::setPointsLearned, player->getAttributes()->getCharm(), std::placeholders::_1));
		case EntityAbilityTypeId::SENSE:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerAttributes::getPointsLearned, player->getAttributes()->getSensibility()), std::bind(&PlayerAttributes::setPointsLearned, player->getAttributes()->getSensibility(), std::placeholders::_1));
		case EntityAbilityTypeId::SKILL_POINTS:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerStats::getAvailableSkillPoints, player->getStats()), std::bind(&PlayerStats::setAvailableSkillPoints, player->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::MONEY:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerInventory::getMoneyAmount, player->getInventory()), std::bind(&PlayerInventory::setMoney, player->getInventory(), std::placeholders::_1));
		case EntityAbilityTypeId::STAMINA:
			return GetterAndSetterTuple<_Numeric>(std::bind(&PlayerStats::getStamina, player->getStats()), std::bind(&PlayerStats::setStamina, player->getStats(), std::placeholders::_1));
		case EntityAbilityTypeId::INVENTORY_CAPACITY:
		case EntityAbilityTypeId::HP_RECOVERY_RATE:
		case EntityAbilityTypeId::MP_RECOVERY_RATE:
		case EntityAbilityTypeId::MP_CONSUMPTION:
		case EntityAbilityTypeId::EXP_RATE:
		case EntityAbilityTypeId::HEAD_SIZE:
		case EntityAbilityTypeId::BODY_SIZE:
		case EntityAbilityTypeId::PLANET_REQUIREMENT:
		case EntityAbilityTypeId::NO_STORAGE_CHARGE:
		case EntityAbilityTypeId::STORAGE_EXPANSION:
		case EntityAbilityTypeId::PERSONAL_SHOP_REMODELLING:
		case EntityAbilityTypeId::CART_GAUGE:
			return GetterAndSetterTuple<_Numeric>();
	}
	return GetterAndSetterTuple<_Numeric>();
}