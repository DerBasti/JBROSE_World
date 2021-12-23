#ifndef __ROSE_AIP_FILEFORMAT__
#define __ROSE_AIP_FILEFORMAT__

#pragma once

#include <string>
#include <stdint.h>
#include <type_traits>
#include <vector>
#include "..\..\JBROSE_Common\FileReader.h"
#include "../../JBROSE_Common/Logger.h"

enum class AIEvent : int {
	INVALID_EVENT = -1,
	SPAWNED,
	IDLE,
	ATTACKING,
	DAMAGED,
	ENEMY_KILLED,
	DEATH,
	EVENT_AMOUNT
};

class AIContext {
private:
	class NPC* sourceEntity;
	class Entity* designatedTarget;
	class Entity* nearestTarget;
	class Entity* lastFound;
	AIEvent& eventType;
public:
	AIContext(class NPC* sourceNpc, AIEvent& event) : AIContext(sourceNpc, event, nullptr) {}
	AIContext(class NPC* sourceNpc, AIEvent& event, class Entity* designatedTarget) : eventType(event) {
		sourceEntity = sourceNpc;
		this->designatedTarget = designatedTarget;
		nearestTarget = lastFound = nullptr;
	}
	virtual ~AIContext() {
		sourceEntity = nullptr;
		designatedTarget = nearestTarget = lastFound = nullptr;
		eventType = AIEvent::INVALID_EVENT;
	}
	__inline NPC* getSourceEntity() const {
		return sourceEntity;
	}
	__inline Entity* getDesignatedTarget() const {
		return designatedTarget;
	}
	__inline void setDesignatedTarget(Entity* entity) {
		designatedTarget = entity;
	}
	__inline Entity* getNearestTarget() const {
		return nearestTarget;
	}
	__inline void setNearestTarget(Entity* entity) {
		nearestTarget = entity;
	}
	__inline Entity* getLastFoundTarget() const {
		return lastFound;
	}
	__inline void setLastFoundTarget(Entity* entity) {
		lastFound = entity;
	}
	__inline const AIEvent& getAIEventType() const {
		return eventType;
	}
};

class AIDataBlock {
private:
	uint32_t length;
	uint32_t operationCode;
	uint32_t lengthOfOperationData;
protected:
	__inline uint32_t getLengthTotal() const {
		return length;
	}
	__inline uint32_t getLengthOfContextData() const {
		return lengthOfOperationData;
	}
	__inline uint32_t getOperationCode() const {
		return operationCode;
	}
public:
	const static uint32_t DEFAULT_HEADER_LENGTH = sizeof(uint32_t) * 2;
	AIDataBlock(std::shared_ptr<char>& datablock) {
		uint32_t* intDataPointer = reinterpret_cast<uint32_t*>(datablock.get());
		length = intDataPointer[0];
		operationCode = intDataPointer[1];

		lengthOfOperationData = length - DEFAULT_HEADER_LENGTH;
	}
	AIDataBlock(uint32_t operationCode, uint32_t length, const char* contextData) {
		this->length = length;
		this->operationCode = operationCode;
		lengthOfOperationData = length - DEFAULT_HEADER_LENGTH;
	}
	virtual ~AIDataBlock() {
	}
};

class AICondition : public AIDataBlock {
public:
	AICondition(std::shared_ptr<char>& datablock) : AIDataBlock(datablock) {}
	AICondition(uint32_t operationCode, uint32_t length, const char* contextData) : AIDataBlock(operationCode, length, contextData) {}
	virtual ~AICondition() {}

	virtual bool isFulfilled(AIContext& context) {
		return false;
	}
};

class AIAction : public AIDataBlock {
public:
	AIAction(std::shared_ptr<char>& datablock) : AIDataBlock(datablock) {}
	AIAction(uint32_t operationCode, uint32_t length, const char* contextData) : AIDataBlock(operationCode, length, contextData) {}
	virtual ~AIAction() {}

	virtual void performAction(AIContext& context) {
	}
};

class AIStateRecord {
private:
	std::vector<std::shared_ptr<AICondition>> conditions;
	std::vector<std::shared_ptr<AIAction>> actions;

	void readConditions(FileReader& reader);
	void readActions(FileReader& reader);
public:
	AIStateRecord();
	AIStateRecord(FileReader& reader);
	virtual ~AIStateRecord();

	bool conditionsFulfilled(AIContext& context);
	void performAction(AIContext& context);
};

class AIState {
private:
	uint32_t amountOfRecords;
	AIStateRecord** records;
public:
	AIState();
	AIState(FileReader& reader, uint32_t amountOfRecords);
	virtual ~AIState();

	uint32_t getAmountOfRecords() const {
		return amountOfRecords;
	}
	AIStateRecord* getRecord(const uint32_t recordId) const {
		return records[recordId];
	}
};

class AIP {
private:
	uint32_t checkingIntervalInMilliseconds;
	uint32_t damageAmountTillTrigger;
	ROSELogger logger;

	const static uint32_t DEFAULT_STATE_AMOUNT = 0x06;
	AIState** states;
public:
	AIP(const char* filePath);
	virtual ~AIP();

	AIState* getRecordsForEvent(AIEvent& event) {
		int eventId = static_cast<int>(event);
		return states[eventId];
	}

	__inline uint64_t getCheckingIntervallInMilliseconds() const {
		return checkingIntervalInMilliseconds;
	}
};


#endif //__ROSE_AIP_FILEFORMAT__
