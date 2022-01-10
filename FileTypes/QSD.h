#ifndef __ROSE_QSD__
#define __ROSE_QSD__

#include <list>
#include "../../JBROSE_Common/FileStoredReader.h"
#include "../BasicTypes/OperationHandler.h"
#include "../../JBROSE_Common/Logger.h"

class QuestBlock {
private:
	uint32_t length;
	uint32_t commandId;
protected:
	ROSEThreadedLogger logger;
	constexpr static uint32_t HEADER_LENGTH = sizeof(uint32_t) * 2;
public:
	QuestBlock(std::shared_ptr<char> rawData);
	virtual ~QuestBlock();

	__inline uint32_t getLength() const {
		return length;
	}
	__inline uint32_t getCommandId() const {
		return commandId;
	}
};

class QuestCondition : public QuestBlock {
private:

public:
	QuestCondition(std::shared_ptr<char> rawData) : QuestBlock(rawData) {
	}
	virtual ~QuestCondition() {

	}

	virtual bool isConditionFulfilled() const { 
		return false; 
	}
};

class QuestDataCheck {
private:
	uint32_t type;
	uint16_t value;
	CheckOperationType operation;
public:
	QuestDataCheck() {
		type = 0;
		value = 0;
		operation = CheckOperationType::UNKNOWN;
	}
	QuestDataCheck(LoadedDataReader& reader) {
		type = reader.readUInt();
		value = reader.readUShort();
		operation = OperationHandler::fromCheckOperationTypeId(reader.readByte());
	}
	virtual ~QuestDataCheck() {

	}
	__inline uint32_t getType() const {
		return type;
	}
	__inline uint16_t getValue() const {
		return value;
	}
	__inline CheckOperationType getCheckOperationType() const {
		return operation;
	}
};

class QuestAbilityTypeCheck {
private:
	EntityAbilityType type;
	int32_t value;
	CheckOperationType operation;
public:
	QuestAbilityTypeCheck() {
		type = EntityAbilityType::UNKNOWN;
		value = 0;
		operation = CheckOperationType::UNKNOWN;
	}
	QuestAbilityTypeCheck(LoadedDataReader& reader) {
		type = OperationHandler::fromEntityAbilityTypeId(reader.readUInt());
		value = reader.readUInt();
		operation = OperationHandler::fromCheckOperationTypeId(reader.readByte());
	}
	virtual ~QuestAbilityTypeCheck() {

	}
	__inline EntityAbilityType getEntityAbilityType() const {
		return type;
	}
	__inline uint32_t getValue() const {
		return value;
	}
	__inline CheckOperationType getCheckOperationType() const {
		return operation;
	}
};

class QuestAction : public QuestBlock {
public:
	QuestAction(std::shared_ptr<char> rawData) : QuestBlock(rawData) {
	}
	virtual ~QuestAction() {

	}

	virtual void performAction() {

	}
};

class QuestRecord {
private:
	bool checkNextRecordFlag;
	std::shared_ptr<QuestRecord> nextRecord;
	uint32_t qsdId;
	std::shared_ptr<char> qsdName;
	std::list<std::unique_ptr<QuestCondition>> conditions;
	std::list<std::unique_ptr<QuestAction>> actions;

	void readConditions(FileInputReader& reader, uint32_t conditionAmount);
	void readActions(FileInputReader& reader, uint32_t actionAmount);
public:
	QuestRecord(FileInputReader& reader);
	virtual ~QuestRecord();

	__inline uint32_t getQuestHash() const {
		return qsdId;
	}
	__inline void setNextRecord(std::shared_ptr<QuestRecord>& nextRecord) {
		this->nextRecord = nextRecord;
	}
	__inline bool hasToCheckNextRecord() const {
		return checkNextRecordFlag;
	}
};

class QSDFile {
private:
	std::map<uint32_t, std::shared_ptr<QuestRecord>> records;
public:
	QSDFile(const char* filePath);
	virtual ~QSDFile();

	__inline const std::map<uint32_t, std::shared_ptr<QuestRecord>>& getAllRecords() const {
		return records;
	}

	static uint32_t makeHash(const char* qsdName);
};

#endif //__ROSE_QSD__