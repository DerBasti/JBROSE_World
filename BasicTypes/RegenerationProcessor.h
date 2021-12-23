#ifndef __ROSE_REGENERATION_PROCESSOR__
#define __ROSE_REGENERATION_PROCESSOR__

#include "../../JBROSE_Common/Timer.h"
#include "../../JBROSE_Common/Logger.h"
#include "Item.h"
#include <map>
#include <vector>
#include <mutex>

class ConsumedItem {
private:
	Timer timer;
	uint16_t abilityType;
	uint16_t scriptExecutionType;
	uint32_t maximumValue;
	uint32_t alreadyConsumedValue;
	uint32_t valuePerSecond;
public:
	ConsumedItem();
	ConsumedItem(const ConsumedItem&) = default;
	ConsumedItem(class STBEntry* consumableEntry, class StatusSTBFile* statusFile);
	virtual ~ConsumedItem();

	
};

class ConsumableItemList {
private:
	static std::map<uint16_t, ConsumedItem> itemList;
	ConsumableItemList();
public:
	virtual ~ConsumableItemList() { }
	static void loadListFromStb(const class ConsumeSTBFile* consumablesSTBFile);
	static ConsumedItem createConsumable(const uint16_t itemId);
};

class RegenerationProcessor {
private:
	ROSELogger logger;
	class Player* player;
	Timer naturalHealTimer;
	std::vector<ConsumedItem> consumedItems;
	std::mutex mutex;

	void checkRegenerationFromItems();
	void checkRegenerationFromNaturalHealing();

	const static uint16_t TIME_TILL_NATURAL_HEAL_IN_MILLIS = 8000;
public:
	RegenerationProcessor(class Player* player);
	virtual ~RegenerationProcessor();

	void checkRegeneration();

	__inline void addConsumedItem(const ConsumedItem& newItem) {
		std::lock_guard<std::mutex> lockGuard(mutex);
		consumedItems.push_back(newItem);
	}
};

#endif //__ROSE_REGENERATION_PROCESSOR__