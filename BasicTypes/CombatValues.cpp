#include "CombatValues.h"
#include "../WorldClient.h"
#include "../WorldServer.h"

void Player::updateAttackAnimation() {
	ZMO* animation = WorldServer::getInstance()->getAttackAnimationForPlayer(this);
	getCombat()->setAttackAnimation(animation);
}

void Player::updateAttackPower() {
	uint16_t totalAttackPower = 0;
	uint16_t weaponAttackPower = 0;
	uint16_t weaponType = 0x00;
	WorldServer *worldServer = WorldServer::getInstance();

	const Item& weapon = getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId());
	if (weapon.isValid()) {
		auto weaponEntries = worldServer->getEquipmentSTB(weapon.getType());
		uint16_t magicAttack = weaponEntries->getMagicAttackPowerOfEntry(weapon.getId());
		uint16_t physicalAttack = weaponEntries->getAttackPowerOfEntry(weapon.getId());
		weaponAttackPower = (std::max)(magicAttack, physicalAttack);
		weaponType = weaponEntries->getEquipmentType(weapon.getId());
	}
	else {
		uint16_t dexterityPart = static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.3);
		uint16_t strengthPart = static_cast<uint16_t>(getAttributes()->getStrength()->getPointsTotal() * 0.5);
		uint16_t levelPart = static_cast<uint16_t>(getStats()->getLevel() * 0.2);

		totalAttackPower = dexterityPart + strengthPart + levelPart;
	}
	switch (weaponType) {
		case WeaponType::MELEE_ONE_HANDED_SWORD:
		case WeaponType::MELEE_ONE_HANDED_BLUNT:
		case WeaponType::MELEE_TWO_HANDED_AXE:
		case WeaponType::MELEE_TWO_HANDED_SPEAR:
		case WeaponType::MELEE_TWO_HANDED_SWORD:
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getStrength()->getPointsTotal() * 0.75);
			totalAttackPower += static_cast<uint16_t>(getStats()->getLevel() * 0.2);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getStrength()->getPointsTotal() * 0.05) + 29) * weaponAttackPower / 30.0);
		break;
		case WeaponType::MELEE_DOUBLE_SWORD:
			totalAttackPower += static_cast<uint16_t>(getStats()->getLevel() * 0.2);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getStrength()->getPointsTotal() * 0.63);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.45);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getDexterity()->getPointsTotal() * 0.05) + 25) * weaponAttackPower / 26.0);
		break;
		case WeaponType::MELEE_KATAR:
			totalAttackPower += static_cast<uint16_t>(getStats()->getLevel() * 0.2);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getStrength()->getPointsTotal() * 0.42);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.55);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getDexterity()->getPointsTotal() * 0.05) + 20) * weaponAttackPower / 21.0);
		break;
		case WeaponType::RANGE_BOW:
			totalAttackPower += static_cast<uint16_t>((getAttributes()->getStrength()->getPointsTotal() + getStats()->getLevel()) * 0.1);
			totalAttackPower += static_cast<uint16_t>((getAttributes()->getDexterity()->getPointsTotal() * 0.04) + (getAttributes()->getSensibility()->getPointsTotal() * 0.03) + 29);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.52);
			totalAttackPower += static_cast<uint16_t>(worldServer->getEquipmentSTB(ItemTypeList::OTHER)->getQualityOfEntry(getInventory()->getItem(InventorySlot::ARROWS).getId()) * 0.5);
		break;
		case WeaponType::RANGE_GUN:
		case WeaponType::RANGE_DUAL_GUN:
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getSensibility()->getPointsTotal() * 0.47);
			totalAttackPower += static_cast<uint16_t>(worldServer->getEquipmentSTB(ItemTypeList::OTHER)->getQualityOfEntry(getInventory()->getItem(InventorySlot::BULLETS).getId()) * 0.8);
			totalAttackPower += static_cast<uint16_t>(getStats()->getLevel() * 0.1);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.3);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getConcentration()->getPointsTotal() * 0.04) + (getAttributes()->getSensibility()->getPointsTotal() * 0.05) + 29) * weaponAttackPower / 30.0);
		break;
		case WeaponType::RANGE_LAUNCHER:
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getConcentration()->getPointsTotal() * 0.47);
			totalAttackPower += worldServer->getEquipmentSTB(ItemTypeList::OTHER)->getQualityOfEntry(getInventory()->getItem(InventorySlot::CANNONSHELLS).getId());
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getStrength()->getPointsTotal() * 0.32);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getConcentration()->getPointsTotal() * 0.45);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getConcentration()->getPointsTotal() * 0.04) + (getAttributes()->getSensibility()->getPointsTotal() * 0.05) + 29) * weaponAttackPower / 30.0);
		break;
		case WeaponType::RANGE_CROSSBOW:
			totalAttackPower += static_cast<uint16_t>((getAttributes()->getStrength()->getPointsTotal() + getStats()->getLevel()) * 0.1);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.04);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getDexterity()->getPointsTotal() * 0.52);
			totalAttackPower += static_cast<uint16_t>(worldServer->getEquipmentSTB(ItemTypeList::OTHER)->getQualityOfEntry(getInventory()->getItem(InventorySlot::ARROWS).getId()) * 0.5);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getSensibility()->getPointsTotal() * 0.03) + 29) * weaponAttackPower / 30.0);
		break;
		case WeaponType::MAGIC_WAND:
			totalAttackPower += static_cast<uint16_t>(getStats()->getLevel() * 0.2);
			totalAttackPower += static_cast<uint16_t>(getAttributes()->getIntelligence()->getPointsTotal() * 0.6);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getSensibility()->getPointsTotal() * 0.1) + 26) * weaponAttackPower / 27.0);
		break;
		case WeaponType::MAGIC_STAFF:
			totalAttackPower += static_cast<uint16_t>(getStats()->getLevel() * 0.2);
			totalAttackPower += static_cast<uint16_t>((getAttributes()->getIntelligence()->getPointsTotal() + getAttributes()->getStrength()->getPointsTotal()) * 0.4);
			totalAttackPower += static_cast<uint16_t>(((getAttributes()->getIntelligence()->getPointsTotal() * 0.05) + 29) * weaponAttackPower / 30.0);
		break;
	}
	getStats()->getCombatValues()->setAttackPower(totalAttackPower);
}

void Player::updateDefense() {
	uint16_t defense = 0x00;
	if (getTraits()->getJob().isSecondJob()) {
		defense = 25;
	}
	defense += static_cast<uint16_t>((getAttributes()->getStrength()->getPointsTotal() + 5) * 0.35);
	defense += static_cast<uint16_t>((getStats()->getLevel() + 15) * 0.7);

	for (uint8_t i = ItemTypeList::FACE.getInventorySlotId(); i <= ItemTypeList::SHIELD.getInventorySlotId(); i++) {
		const Item& item = getInventory()->getItem(i);
		if (!item.isValid() || i == ItemTypeList::WEAPON.getInventorySlotId()) {
			continue;
		}
		defense += WorldServer::getInstance()->getEquipmentSTB(i)->getDefenseOfEntry(item.getId());
	}

	getStats()->getCombatValues()->setDefense(defense);
}

void Player::updateMagicDefense() {

}

void Player::updateCriticalRate() {

}

void Player::updateAttackSpeed() {
	uint16_t attackSpeed = 115;
	const Item& weapon = getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId());
	if (weapon.isValid()) {
		attackSpeed = 1500 / (5 + WorldServer::getInstance()->getEquipmentSTB(weapon.getType())->getAttackSpeedOfEntry(weapon.getId()));
	}
	this->getStats()->getCombatValues()->setAttackSpeed(attackSpeed);
	getCombat()->onAttackspeedUpdate();
}

void Player::updateAttackRange() {
	uint16_t attackRange = 150;
	const Item& weapon = getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId());
	if (weapon.isValid()) {
		attackRange += WorldServer::getInstance()->getEquipmentSTB(weapon.getType())->getAttackRangeOfEntry(weapon.getId());
	}
	getStats()->getCombatValues()->setAttackRange(attackRange);
}

void Player::updateMovementSpeed() {
	uint16_t movementSpeed = 200;
	if (getStance()->isRunning()) {
		movementSpeed = 425;
		const Item& shoes = getInventory()->getItem(ItemTypeList::SHOES.getInventorySlotId());
		if (shoes.isValid()) {
			movementSpeed = WorldServer::getInstance()->getEquipmentSTB(shoes.getType())->getMovementSpeedOfEntry(shoes.getId());
		}
	}
	getStats()->setMovementSpeed(movementSpeed);
}

void Player::updateMaximumHp() {
	float multiplier = 2.36f;
	int additionalPart1 = 26;
	int additionalPart2 = 4;
	//Jobs
	switch (getTraits()->getJob()) {
		case JobId::SOLDIER_ID:
			multiplier = 3.5f;
			additionalPart1 = 20;
			additionalPart2 = 5;
		break;
		case JobId::KNIGHT_ID:
			multiplier = 3.5f;
			additionalPart1 = 28;
			additionalPart2 = 5;
		break;
		case JobId::CHAMPION_ID:
			multiplier = 3.5f;
			additionalPart1 = 22;
			additionalPart2 = 5;
		break;

		case JobId::MUSE_ID:
			multiplier = 2.36f;
			additionalPart1 = 26;
			additionalPart2 = 4;
		break;
		case JobId::MAGE_ID:
			multiplier = 2.37f;
			additionalPart1 = 26;
			additionalPart2 = 5;
		break;
		case JobId::CLERIC_ID:
			multiplier = 2.4f;
			additionalPart1 = 26;
			additionalPart2 = 7;
		break;

		case JobId::HAWKER_ID:
			multiplier = 2.7f;
			additionalPart1 = 26;
			additionalPart2 = 4;
		break;
		case JobId::RAIDER_ID:
			multiplier = 3.0f;
			additionalPart1 = 26;
			additionalPart2 = 5;
		break;
		case JobId::SCOUT_ID:
			multiplier = 2.7f;
			additionalPart1 = 26;
			additionalPart2 = 7;
		break;

		case JobId::DEALER_ID:
		case JobId::BOURGEOIS_ID:
		case JobId::ARTISAN_ID:
			multiplier = 2.7f;
			additionalPart1 = 20;
			additionalPart2 = 5;
		break;

	}

	uint16_t maximumHp = static_cast<uint16_t>((::sqrt(static_cast<double>(additionalPart1 + getStats()->getLevel()))) * (additionalPart2 + getStats()->getLevel()) * multiplier);
	maximumHp += getAttributes()->getStrength()->getPointsTotal() << 1;

	//if Jobs == 2nd Job --> +300HP
	if (getTraits()->getJob().isSecondJob()) {
		maximumHp += 300;
	}

	getStats()->setMaxHp(maximumHp);
}

void Player::updateMaximumMp() {

}
void Player::updateMaximumWeight() {
	uint16_t newMaximum = 1100 + (getStats()->getLevel() * 5) + (getAttributes()->getStrength()->getPointsTotal());
	getStats()->setMaximumPossibleWeight(newMaximum);
}