#ifndef __ROSE_REGENERATION_PROCESSOR__
#define __ROSE_REGENERATION_PROCESSOR__

#include "../../JBROSE_Common/Timer.h"
#include "../../JBROSE_Common/Logger.h"
#include "OperationHandler.h"
#include "EntityStats.h"
#include "Item.h"
#include <map>
#include <list>
#include <mutex>
#include <memory>

enum class ConsumableExecutionType : uint16_t {
	INVALID,
	FOOD,
	SKILL_EXECUTION,
	CHARM_SCROLLS,
	EMOTION_EXECUTION,
	SUMMONS,
	SNOWBALL_THROWING,
	REPAIR_HAMMER,
	IMMEDIATE_APPLICATION,
	UNKNOWN,
	LEARN_SKILL
};

class ConsumedItem {
private:
	Timer timer;
	EntityAbilityType influencedAbilityType;
	ConsumableExecutionType scriptExecutionType;
	uint32_t maximumValue;
	uint32_t alreadyConsumedValue;
	uint32_t valuePerSecond;
	float durationInMilliseconds;

public:
	ConsumedItem();
	ConsumedItem(const ConsumedItem&);
	ConsumedItem(const class STBEntry* consumableEntry, const class StatusSTBFile* statusFile);
	virtual ~ConsumedItem();
	ConsumedItem& operator=(const ConsumedItem& item);

	uint32_t updateUsageValue();
	void markAsFullyUsed() {
		alreadyConsumedValue = maximumValue;
	}

	__inline ConsumableExecutionType getExecutionType() const {
		return scriptExecutionType;
	}
	__inline EntityAbilityType getInfluencedAbilityType() const {
		return influencedAbilityType;
	}
	__inline uint32_t getMaximumValue() const {
		return maximumValue;
	}
	__inline uint32_t getAlreadyConsumedValue() const {
		return alreadyConsumedValue;
	}
	__inline uint32_t getValuePerSecond() const {
		return valuePerSecond;
	}
	__inline bool isValid() const {
		return static_cast<uint16_t>(scriptExecutionType) > 0;
	}
	__inline bool isFullyUsed() const {
		return getAlreadyConsumedValue() >= getMaximumValue();
	}
};

class ConsumableItemList {
private:
	std::map<uint16_t, ConsumedItem> itemList;
	ROSEThreadedLogger logger;
public:
	ConsumableItemList(const class ConsumeSTBFile* consumablesSTBFile, const class StatusSTBFile* statusSTBFile);
	virtual ~ConsumableItemList() { }
	std::unique_ptr<ConsumedItem> createConsumable(const uint16_t itemId);
};

class RegenerationProcessor {
private:
	ROSEThreadedLogger logger;
	class Player* player;
	Timer naturalHealTimer;
	std::list<std::unique_ptr<ConsumedItem>> consumedItems;
	std::mutex mutex;

	void checkRegenerationFromItems();
	void checkRegenerationFromNaturalHealing();

	template<class _StatType>
	void executeConsumableScript(_StatType(PlayerStats::*getFunction)() const, void (PlayerStats::*setFunction)(_StatType newValue), _StatType valueToUse, ResultOperationType resultOperation);
	template<class _StatType>
	void executeConsumableScript(_StatType(EntityStats::*getFunction)() const, void (EntityStats::*setFunction)(_StatType newValue), _StatType valueToUse, ResultOperationType resultOperation);

	const static uint16_t TIME_TILL_NATURAL_HEAL_IN_MILLIS = 8000;
public:
	RegenerationProcessor(class Player* player);
	virtual ~RegenerationProcessor();

	void checkRegeneration();
	bool addConsumedItem(std::unique_ptr<ConsumedItem>& newItem);
};

#endif //__ROSE_REGENERATION_PROCESSOR__