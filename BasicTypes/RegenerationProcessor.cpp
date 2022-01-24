#include "RegenerationProcessor.h"
#include "../WorldClient.h"
#include "../BasicTypes/EntityStatHandler.h"

RegenerationProcessor::RegenerationProcessor(Player* player) {
	this->player = player;
}

RegenerationProcessor::~RegenerationProcessor() {
	player = nullptr;
}

void RegenerationProcessor::checkRegeneration() {
	if (!player->isIngame()) {
		naturalHealTimer.updateTimestamp();
		return;
	}
	checkRegenerationFromItems();
	checkRegenerationFromNaturalHealing();
}

bool RegenerationProcessor::addConsumedItem(std::unique_ptr<ConsumedItem>& newItem) {
	bool allowedToBeAdded = true;
	switch (newItem->getInfluencedAbilityType()) {
		case EntityAbilityTypeId::CURRENT_HP:
			allowedToBeAdded = player->getStats()->getCurrentHp() < player->getStats()->getMaxHp();
		break;
		case EntityAbilityTypeId::CURRENT_MP:
			allowedToBeAdded = player->getStats()->getCurrentMp() < player->getStats()->getMaxMp();
		break;
	}
	if (!allowedToBeAdded) {
		return false;
	}
	std::lock_guard<std::mutex> lockGuard(mutex);
	consumedItems.push_back(std::move(newItem));
	return true;
}

bool RegenerationProcessor::addImmediateUseItem(std::unique_ptr<ConsumedItem>& newItem) {
	switch (newItem->getInfluencedAbilityType()) {
		case EntityAbilityTypeId::SKILL_POINTS:
			player->getStats()->setAvailableSkillPoints(player->getStats()->getAvailableSkillPoints() + newItem->getMaximumValue());
		break;
		case EntityAbilityTypeId::STAMINA:
			player->getStats()->setStamina(player->getStats()->getStamina() + newItem->getMaximumValue());
		break;
	}
	return true;
}

/*
template<class _StatType>
void RegenerationProcessor::executeConsumableScript(_StatType(PlayerStats::*getFunction)() const, void (PlayerStats::*setFunction)(const _StatType newValue), _StatType valueToUse, ResultOperationType resultOperation) {
	auto playerStats = player->getStats();
	_StatType value = (playerStats->*getFunction)();
	_StatType result = OperationHandler::executeResultOperation(value, valueToUse, resultOperation);
	(playerStats->*setFunction)(result);
}

template<class _StatType>
void RegenerationProcessor::executeConsumableScript(_StatType(EntityStats::*getFunction)() const, void (EntityStats::*setFunction)(const _StatType newValue), _StatType valueToUse, ResultOperationType resultOperation) {
	auto playerStats = dynamic_cast<EntityStats*>(player->getStats());
	_StatType value = (playerStats->*getFunction)();
	_StatType result = OperationHandler::executeResultOperation(value, valueToUse, resultOperation);
	(playerStats->*setFunction)(result);
}
*/

void RegenerationProcessor::checkRegenerationFromItems() {
	auto it = consumedItems.begin();
	while(it != consumedItems.end()) {
		uint32_t amount = (*it)->updateUsageValue();
		if (amount > 0) {
			switch ((*it)->getInfluencedAbilityType()) {
				case EntityAbilityTypeId::CURRENT_HP:
				{
					auto hpGetterAndSetter = EntityStatHandler::getNumericGetterAndSetterOfEntityByEntityAbilityType<uint32_t>(player, EntityAbilityType::CURRENT_HP);
					hpGetterAndSetter.set(hpGetterAndSetter.get() + amount);
				}
				break;
				case EntityAbilityTypeId::CURRENT_MP:
				{
					auto mpGetterAndSetter = EntityStatHandler::getNumericGetterAndSetterOfEntityByEntityAbilityType<uint32_t>(player, EntityAbilityType::CURRENT_MP);
					mpGetterAndSetter.set(mpGetterAndSetter.get() + amount);
				}
				break;
			}
		}
		if ((*it)->isFullyUsed()) {
			it = consumedItems.erase(it);
			logger.logDebug("Item fully consumed. Deleting...");
			continue;
		}
		++it;
	}
}

void RegenerationProcessor::checkRegenerationFromNaturalHealing() {
	uint64_t timePassed = naturalHealTimer.getPassedTimeInMillis();
	if (timePassed < RegenerationProcessor::TIME_TILL_NATURAL_HEAL_IN_MILLIS) {
		return;
	}
	float healingAmountForHp = std::ceil(player->getStats()->getMaxHp() * 2.0f / 100.0f);
	float healingAmountForMp = std::ceil(player->getStats()->getMaxMp() * 2.0f / 100.0f);
	if (player->getStance()->isSitting()) {
		healingAmountForHp *= 4;
		healingAmountForMp *= 4;
	}
	uint32_t actualHealingAmountForHp = static_cast<uint32_t>(healingAmountForHp);
	uint32_t actualHealingAmountForMp = static_cast<uint32_t>(healingAmountForMp);
	logger.logDebug("Healing naturally for ", actualHealingAmountForHp, "Hp and ", actualHealingAmountForMp, "Mp");
	player->getStats()->addToCurrentHp(actualHealingAmountForHp);
	player->getStats()->addToCurrentMp(actualHealingAmountForMp);

	naturalHealTimer.updateTimestamp();
}