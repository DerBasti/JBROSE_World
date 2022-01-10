#include "OperationHandler.h"
#include "../BasicTypes/Entity.h"
#include "../WorldClient.h"

const CheckOperationType CheckOperationType::EQUAL = CheckOperationType(CheckOperationTypeId::EQUAL, "EQUAL");
const CheckOperationType CheckOperationType::BIGGER = CheckOperationType(CheckOperationTypeId::BIGGER, "BIGGER");
const CheckOperationType CheckOperationType::BIGGER_EQUAL = CheckOperationType(CheckOperationTypeId::BIGGER_EQUAL, "BIGGER_EQUAL");
const CheckOperationType CheckOperationType::SMALLER = CheckOperationType(CheckOperationTypeId::SMALLER, "SMALLER");
const CheckOperationType CheckOperationType::SMALLER_EQUAL = CheckOperationType(CheckOperationTypeId::SMALLER_EQUAL, "SMALLER_EQUAL");
const CheckOperationType CheckOperationType::NOT_EQUAL = CheckOperationType(CheckOperationTypeId::NOT_EQUAL, "NOT_EQUAL");
const CheckOperationType CheckOperationType::UNKNOWN = CheckOperationType(CheckOperationTypeId::UNKNOWN, "UNKNOWN");

const ResultOperationType ResultOperationType::RETURN_OTHER_VALUE = ResultOperationType(ResultOperationType::RETURN_OTHER_VALUE, "RETURN_OTHER_VALUE");
const ResultOperationType ResultOperationType::ADDITION = ResultOperationType(ResultOperationTypeId::ADDITION, "ADDITION");
const ResultOperationType ResultOperationType::SUBTRACTION = ResultOperationType(ResultOperationTypeId::SUBTRACTION, "SUBTRACTION");
const ResultOperationType ResultOperationType::MULTIPLICATION = ResultOperationType(ResultOperationTypeId::MULTIPLICATION, "MULTIPLICATION");
const ResultOperationType ResultOperationType::INCREMENTATION = ResultOperationType(ResultOperationTypeId::INCREMENTATION, "INCREMENTATION");
const ResultOperationType ResultOperationType::UNKNOWN = ResultOperationType(ResultOperationTypeId::UNKNOWN, "UNKNOWN");

std::map<uint8_t, CheckOperationType> OperationHandler::checkOperationTypeMap{
	{static_cast<uint8_t>(CheckOperationTypeId::EQUAL), CheckOperationType::EQUAL},
	{static_cast<uint8_t>(CheckOperationTypeId::BIGGER), CheckOperationType::BIGGER},
	{static_cast<uint8_t>(CheckOperationTypeId::BIGGER_EQUAL), CheckOperationType::BIGGER_EQUAL},
	{static_cast<uint8_t>(CheckOperationTypeId::SMALLER), CheckOperationType::SMALLER},
	{static_cast<uint8_t>(CheckOperationTypeId::SMALLER_EQUAL), CheckOperationType::SMALLER_EQUAL},
	{static_cast<uint8_t>(CheckOperationTypeId::NOT_EQUAL), CheckOperationType::NOT_EQUAL}
};

std::map<uint8_t, ResultOperationType> OperationHandler::resultOperationTypeMap{
	{static_cast<uint8_t>(ResultOperationTypeId::RETURN_OTHER_VALUE), ResultOperationType::RETURN_OTHER_VALUE},
	{static_cast<uint8_t>(ResultOperationTypeId::ADDITION), ResultOperationType::ADDITION},
	{static_cast<uint8_t>(ResultOperationTypeId::SUBTRACTION), ResultOperationType::SUBTRACTION},
	{static_cast<uint8_t>(ResultOperationTypeId::MULTIPLICATION), ResultOperationType::MULTIPLICATION},
	{static_cast<uint8_t>(ResultOperationTypeId::INCREMENTATION), ResultOperationType::INCREMENTATION}
};

const AIPAbilityType AIPAbilityType::LEVEL = AIPAbilityType(AIPAbilityTypeId::LEVEL, "Level");
const AIPAbilityType AIPAbilityType::ATTACK_POWER = AIPAbilityType(AIPAbilityTypeId::ATTACK_POWER, "Attack Power");
const AIPAbilityType AIPAbilityType::DEFENSE = AIPAbilityType(AIPAbilityTypeId::DEFENSE, "Defense");
const AIPAbilityType AIPAbilityType::MAGIC_DEFENSE = AIPAbilityType(AIPAbilityTypeId::MAGIC_DEFENSE, "Magic Defense");
const AIPAbilityType AIPAbilityType::CHARM = AIPAbilityType(AIPAbilityTypeId::CHARM, "Charm");
const AIPAbilityType AIPAbilityType::UNKNOWN = AIPAbilityType(AIPAbilityTypeId::UNKNOWN, "Unknown");

std::map<uint8_t, AIPAbilityType> OperationHandler::abilityTypeMap{
	{static_cast<uint8_t>(AIPAbilityTypeId::LEVEL), AIPAbilityType::LEVEL},
	{static_cast<uint8_t>(AIPAbilityTypeId::ATTACK_POWER),AIPAbilityType::ATTACK_POWER },
	{static_cast<uint8_t>(AIPAbilityTypeId::DEFENSE), AIPAbilityType::DEFENSE},
	{static_cast<uint8_t>(AIPAbilityTypeId::MAGIC_DEFENSE), AIPAbilityType::MAGIC_DEFENSE},
	{static_cast<uint8_t>(AIPAbilityTypeId::CHARM), AIPAbilityType::CHARM},
	{static_cast<uint8_t>(AIPAbilityTypeId::UNKNOWN), AIPAbilityType::UNKNOWN}
};


const EntityAbilityType EntityAbilityType::BIRTHSTONE_TYPE = EntityAbilityType(EntityAbilityTypeId::BIRTHSTONE_TYPE, "BIRTHSTONE_TYPE");
const EntityAbilityType EntityAbilityType::JOB = EntityAbilityType(EntityAbilityTypeId::JOB, "JOB");
const EntityAbilityType EntityAbilityType::STRENGTH = EntityAbilityType(EntityAbilityTypeId::STRENGTH, "STRENGTH");
const EntityAbilityType EntityAbilityType::DEXTERITY = EntityAbilityType(EntityAbilityTypeId::DEXTERITY, "DEXTERITY");
const EntityAbilityType EntityAbilityType::INTELLIGENCE = EntityAbilityType(EntityAbilityTypeId::INTELLIGENCE, "INTELLIGENCE");
const EntityAbilityType EntityAbilityType::CONCENTRATION = EntityAbilityType(EntityAbilityTypeId::CONCENTRATION, "CONCENTRATION");
const EntityAbilityType EntityAbilityType::CHARM = EntityAbilityType(EntityAbilityTypeId::CHARM, "CHARM");
const EntityAbilityType EntityAbilityType::SENSE = EntityAbilityType(EntityAbilityTypeId::SENSE, "SENSE");
const EntityAbilityType EntityAbilityType::CURRENT_HP = EntityAbilityType(EntityAbilityTypeId::CURRENT_HP, "CURRENT_HP");
const EntityAbilityType EntityAbilityType::CURRENT_MP = EntityAbilityType(EntityAbilityTypeId::CURRENT_MP, "CURRENT_MP");
const EntityAbilityType EntityAbilityType::ATTACK_POWER = EntityAbilityType(EntityAbilityTypeId::ATTACK_POWER, "ATTACK_POWER");
const EntityAbilityType EntityAbilityType::DEFENSE = EntityAbilityType(EntityAbilityTypeId::DEFENSE, "DEFENSE");
const EntityAbilityType EntityAbilityType::ACCURACY = EntityAbilityType(EntityAbilityTypeId::ACCURACY, "ACCURACY");
const EntityAbilityType EntityAbilityType::MAGIC_RESISTENCE = EntityAbilityType(EntityAbilityTypeId::MAGIC_RESISTENCE, "MAGIC_RESISTENCE");
const EntityAbilityType EntityAbilityType::DODGE_RATE = EntityAbilityType(EntityAbilityTypeId::DODGE_RATE, "DODGE_RATE");
const EntityAbilityType EntityAbilityType::MOVEMENT_SPEED = EntityAbilityType(EntityAbilityTypeId::MOVEMENT_SPEED, "MOVEMENT_SPEED");
const EntityAbilityType EntityAbilityType::ATTACK_SPEED = EntityAbilityType(EntityAbilityTypeId::ATTACK_SPEED, "ATTACK_SPEED");
const EntityAbilityType EntityAbilityType::INVENTORY_CAPACITY = EntityAbilityType(EntityAbilityTypeId::INVENTORY_CAPACITY, "INVENTORY_CAPACITY");
const EntityAbilityType EntityAbilityType::CRITICAL_RATE = EntityAbilityType(EntityAbilityTypeId::CRITICAL_RATE, "CRITICAL_RATE");
const EntityAbilityType EntityAbilityType::HP_RECOVERY_RATE = EntityAbilityType(EntityAbilityTypeId::HP_RECOVERY_RATE, "HP_RECOVERY_RATE");
const EntityAbilityType EntityAbilityType::MP_RECOVERY_RATE = EntityAbilityType(EntityAbilityTypeId::MP_RECOVERY_RATE, "MP_RECOVERY_RATE");
const EntityAbilityType EntityAbilityType::MP_CONSUMPTION = EntityAbilityType(EntityAbilityTypeId::MP_CONSUMPTION, "MP_CONSUMPTION");
const EntityAbilityType EntityAbilityType::EXP_RATE = EntityAbilityType(EntityAbilityTypeId::EXP_RATE, "EXP_RATE");
const EntityAbilityType EntityAbilityType::LEVEL = EntityAbilityType(EntityAbilityTypeId::LEVEL, "LEVEL");
const EntityAbilityType EntityAbilityType::SKILL_POINTS = EntityAbilityType(EntityAbilityTypeId::SKILL_POINTS, "SKILL_POINTS");
const EntityAbilityType EntityAbilityType::MAX_HP = EntityAbilityType(EntityAbilityTypeId::MAX_HP, "MAX_HP");
const EntityAbilityType EntityAbilityType::MAX_MP = EntityAbilityType(EntityAbilityTypeId::MAX_MP, "MAX_MP");
const EntityAbilityType EntityAbilityType::MONEY = EntityAbilityType(EntityAbilityTypeId::MONEY, "MONEY");
const EntityAbilityType EntityAbilityType::UNKNOWN = EntityAbilityType(EntityAbilityTypeId::UNKNOWN, "UNKNOWN");



std::map<uint8_t, EntityAbilityType> OperationHandler::entityAbilityTypeMap{
	{static_cast<uint8_t>(EntityAbilityTypeId::BIRTHSTONE_TYPE), EntityAbilityType::BIRTHSTONE_TYPE},
	{static_cast<uint8_t>(EntityAbilityTypeId::JOB), EntityAbilityType::JOB},
	{static_cast<uint8_t>(EntityAbilityTypeId::STRENGTH), EntityAbilityType::STRENGTH},
	{static_cast<uint8_t>(EntityAbilityTypeId::DEXTERITY), EntityAbilityType::DEXTERITY},
	{static_cast<uint8_t>(EntityAbilityTypeId::INTELLIGENCE), EntityAbilityType::INTELLIGENCE},
	{static_cast<uint8_t>(EntityAbilityTypeId::CONCENTRATION), EntityAbilityType::CONCENTRATION},
	{static_cast<uint8_t>(EntityAbilityTypeId::CHARM), EntityAbilityType::CHARM},
	{static_cast<uint8_t>(EntityAbilityTypeId::SENSE), EntityAbilityType::SENSE},
	{static_cast<uint8_t>(EntityAbilityTypeId::CURRENT_HP), EntityAbilityType::CURRENT_HP},
	{static_cast<uint8_t>(EntityAbilityTypeId::CURRENT_MP), EntityAbilityType::CURRENT_MP},
	{static_cast<uint8_t>(EntityAbilityTypeId::ATTACK_POWER), EntityAbilityType::ATTACK_POWER},
	{static_cast<uint8_t>(EntityAbilityTypeId::DEFENSE), EntityAbilityType::DEFENSE},
	{static_cast<uint8_t>(EntityAbilityTypeId::ACCURACY), EntityAbilityType::ACCURACY},
	{static_cast<uint8_t>(EntityAbilityTypeId::MAGIC_RESISTENCE), EntityAbilityType::MAGIC_RESISTENCE},
	{static_cast<uint8_t>(EntityAbilityTypeId::DODGE_RATE), EntityAbilityType::DODGE_RATE},
	{static_cast<uint8_t>(EntityAbilityTypeId::MOVEMENT_SPEED), EntityAbilityType::MOVEMENT_SPEED},
	{static_cast<uint8_t>(EntityAbilityTypeId::ATTACK_SPEED), EntityAbilityType::ATTACK_SPEED},
	{static_cast<uint8_t>(EntityAbilityTypeId::INVENTORY_CAPACITY), EntityAbilityType::INVENTORY_CAPACITY},
	{static_cast<uint8_t>(EntityAbilityTypeId::CRITICAL_RATE), EntityAbilityType::CRITICAL_RATE},
	{static_cast<uint8_t>(EntityAbilityTypeId::HP_RECOVERY_RATE), EntityAbilityType::HP_RECOVERY_RATE},
	{static_cast<uint8_t>(EntityAbilityTypeId::MP_RECOVERY_RATE), EntityAbilityType::MP_RECOVERY_RATE},
	{static_cast<uint8_t>(EntityAbilityTypeId::MP_CONSUMPTION), EntityAbilityType::MP_CONSUMPTION},
	{static_cast<uint8_t>(EntityAbilityTypeId::EXP_RATE), EntityAbilityType::EXP_RATE},
	{static_cast<uint8_t>(EntityAbilityTypeId::LEVEL), EntityAbilityType::LEVEL},
	{static_cast<uint8_t>(EntityAbilityTypeId::SKILL_POINTS), EntityAbilityType::SKILL_POINTS},
	{static_cast<uint8_t>(EntityAbilityTypeId::MAX_HP), EntityAbilityType::MAX_HP},
	{static_cast<uint8_t>(EntityAbilityTypeId::MAX_MP), EntityAbilityType::MAX_MP},
	{static_cast<uint8_t>(EntityAbilityTypeId::MONEY), EntityAbilityType::MONEY},
	{static_cast<uint8_t>(EntityAbilityTypeId::UNKNOWN), EntityAbilityType::UNKNOWN }
};


uint16_t OperationHandler::getAIPAbilityValueOfEntity(Entity* entity, AIPAbilityType type) {
	uint16_t resultValue = 0x00;
	switch (type) {
		case AIPAbilityTypeId::LEVEL:
			resultValue = static_cast<uint16_t>(entity->getStats()->getLevel());
		break;
		case AIPAbilityTypeId::ATTACK_POWER:
			resultValue = static_cast<uint16_t>(entity->getStats()->getCombatValues()->getAttackPower());
		break;
		case AIPAbilityTypeId::DEFENSE:
			resultValue = static_cast<uint16_t>(entity->getStats()->getCombatValues()->getDefense());
		break;
		case AIPAbilityTypeId::MAGIC_DEFENSE:
			resultValue = static_cast<uint16_t>(entity->getStats()->getCombatValues()->getMagicDefense());
		break;
		case AIPAbilityTypeId::CHARM:
			if (entity->isPlayer()) {
				resultValue = static_cast<uint16_t>(dynamic_cast<Player*>(entity)->getAttributes()->getCharm()->getPointsTotal());
			}
		break;
	}
	return resultValue;
}