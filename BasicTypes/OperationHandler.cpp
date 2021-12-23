#include "OperationHandler.h"
#include "../BasicTypes/Entity.h"
#include "../WorldClient.h"

const OperationType OperationType::EQUAL = OperationType(OperationTypeId::EQUAL, "EQUAL");
const OperationType OperationType::BIGGER = OperationType(OperationTypeId::BIGGER, "BIGGER");
const OperationType OperationType::BIGGER_EQUAL = OperationType(OperationTypeId::BIGGER_EQUAL, "BIGGER_EQUAL");
const OperationType OperationType::SMALLER = OperationType(OperationTypeId::SMALLER, "SMALLER");
const OperationType OperationType::SMALLER_EQUAL = OperationType(OperationTypeId::SMALLER_EQUAL, "SMALLER_EQUAL");
const OperationType OperationType::RETURN_OTHER_VALUE = OperationType(OperationTypeId::RETURN_OTHER_VALUE, "RETURN_OTHER_VALUE");
const OperationType OperationType::ADDITION = OperationType(OperationTypeId::ADDITION, "ADDITION");
const OperationType OperationType::SUBTRACTION = OperationType(OperationTypeId::SUBTRACTION, "SUBTRACTION");
const OperationType OperationType::MULTIPLICATION = OperationType(OperationTypeId::MULTIPLICATION, "MULTIPLICATION");
const OperationType OperationType::INCREMENTATION = OperationType(OperationTypeId::INCREMENTATION, "INCREMENTATION");
const OperationType OperationType::NOT_EQUAL = OperationType(OperationTypeId::NOT_EQUAL, "NOT_EQUAL");

std::map<uint8_t, OperationType> OperationHandler::operationTypeMap{
	{static_cast<uint8_t>(OperationTypeId::EQUAL), OperationType::EQUAL},
	{static_cast<uint8_t>(OperationTypeId::BIGGER), OperationType::BIGGER},
	{static_cast<uint8_t>(OperationTypeId::BIGGER_EQUAL), OperationType::BIGGER_EQUAL},
	{static_cast<uint8_t>(OperationTypeId::SMALLER), OperationType::SMALLER},
	{static_cast<uint8_t>(OperationTypeId::SMALLER_EQUAL), OperationType::SMALLER_EQUAL},
	{static_cast<uint8_t>(OperationTypeId::RETURN_OTHER_VALUE), OperationType::RETURN_OTHER_VALUE},
	{static_cast<uint8_t>(OperationTypeId::ADDITION), OperationType::ADDITION},
	{static_cast<uint8_t>(OperationTypeId::SUBTRACTION), OperationType::SUBTRACTION},
	{static_cast<uint8_t>(OperationTypeId::MULTIPLICATION), OperationType::MULTIPLICATION},
	{static_cast<uint8_t>(OperationTypeId::INCREMENTATION), OperationType::INCREMENTATION},
	{static_cast<uint8_t>(OperationTypeId::NOT_EQUAL), OperationType::NOT_EQUAL}
};

const AbilityType AbilityType::LEVEL = AbilityType(AbilityTypeId::LEVEL, "Level");
const AbilityType AbilityType::ATTACK_POWER = AbilityType(AbilityTypeId::ATTACK_POWER, "Attack Power");
const AbilityType AbilityType::DEFENSE = AbilityType(AbilityTypeId::DEFENSE, "Defense");
const AbilityType AbilityType::MAGIC_DEFENSE = AbilityType(AbilityTypeId::MAGIC_DEFENSE, "Magic Defense");
const AbilityType AbilityType::CHARM = AbilityType(AbilityTypeId::CHARM, "Charm");
const AbilityType AbilityType::UNKNOWN = AbilityType(AbilityTypeId::UNKNOWN, "Unknown");

std::map<uint8_t, AbilityType> OperationHandler::abilityTypeMap{
	{static_cast<uint8_t>(AbilityTypeId::LEVEL), AbilityType::LEVEL},
	{static_cast<uint8_t>(AbilityTypeId::ATTACK_POWER),AbilityType::ATTACK_POWER },
	{static_cast<uint8_t>(AbilityTypeId::DEFENSE), AbilityType::DEFENSE},
	{static_cast<uint8_t>(AbilityTypeId::MAGIC_DEFENSE), AbilityType::MAGIC_DEFENSE},
	{static_cast<uint8_t>(AbilityTypeId::CHARM), AbilityType::CHARM},
	{static_cast<uint8_t>(AbilityTypeId::UNKNOWN), AbilityType::UNKNOWN}
};

uint16_t OperationHandler::getAbilityValueOfEntity(Entity* entity, AbilityType type) {
	uint16_t resultValue = 0x00;
	switch (type) {
		case AbilityTypeId::LEVEL:
			resultValue = static_cast<uint16_t>(entity->getStats()->getLevel());
		break;
		case AbilityTypeId::ATTACK_POWER:
			resultValue = static_cast<uint16_t>(entity->getStats()->getCombatValues()->getAttackPower());
		break;
		case AbilityTypeId::DEFENSE:
			resultValue = static_cast<uint16_t>(entity->getStats()->getCombatValues()->getDefense());
		break;
		case AbilityTypeId::MAGIC_DEFENSE:
			resultValue = static_cast<uint16_t>(entity->getStats()->getCombatValues()->getMagicDefense());
		break;
		case AbilityTypeId::CHARM:
			if (entity->isPlayer()) {
				resultValue = static_cast<uint16_t>(dynamic_cast<Player*>(entity)->getAttributes()->getCharm()->getPointsTotal());
			}
		break;
	}
	return resultValue;
}