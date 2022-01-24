#ifndef __ROSE_QSD__
#define __ROSE_QSD__

#include <list>
#include "../../JBROSE_Common/FileStoredReader.h"
#include "../BasicTypes/OperationHandler.h"
#include "../../JBROSE_Common/Logger.h"
#include "Quests/PlayerQuestJournal.h"

class QuestTriggerContext {
private:
	class Entity* triggerEntity;
	PlayerQuest* selectedQuest;
	class Entity* selectedEntity;
public:
	QuestTriggerContext(class Entity* triggerEntity) {
		this->triggerEntity = triggerEntity;
	}
	virtual ~QuestTriggerContext() {

	}
	__inline Entity* getTriggerEntity() const {
		return triggerEntity;
	}
	__inline PlayerQuest* getSelectedQuest() const {
		return selectedQuest;
	}
	__inline void setSelectedQuest(PlayerQuest* selectedQuest) {
		this->selectedQuest = selectedQuest;
	}
	__inline Entity* getSelectedEntity() const {
		return selectedEntity;
	}
	__inline void setSelectedEntity(class Entity* selectedEntity) {
		this->selectedEntity = selectedEntity;
	}
};

class QuestBlock {
private:
	uint32_t length;
	uint32_t commandId;
	uint32_t questHash;
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
	__inline uint32_t getQuestHash() const {
		return questHash;
	}
	__inline void setQuestHash(uint32_t questHash) {
		this->questHash = questHash;
	}
	virtual std::shared_ptr<char> toPrintable() const {
		char *buffer = new char[0x50];
		sprintf_s(buffer, 0x50, "Command: 0x%08x | Length: %i", getCommandId(), getLength());
		return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	}
};

class QuestCondition : public QuestBlock {
private:

public:
	QuestCondition(std::shared_ptr<char> rawData) : QuestBlock(rawData) {
	}
	virtual ~QuestCondition() {

	}

	virtual bool isConditionFulfilled(QuestTriggerContext& context) const {
		return false; 
	}
};

class QuestDataCheck {
private:
	uint8_t varNumber;
	uint16_t varType;
	uint16_t value;
	CheckOperationType operation;
public:
	QuestDataCheck() {
		varNumber = 0;
		varType = 0;
		value = 0;
		operation = CheckOperationType::UNKNOWN;
	}
	QuestDataCheck(LoadedDataReader& reader) {
		varNumber = (uint8_t)reader.readUShort();
		varType = reader.readUShort();
		value = reader.readUShort();
		operation = OperationHandler::fromCheckOperationTypeId(reader.readByte());
	}
	virtual ~QuestDataCheck() {

	}
	__inline uint16_t getVariableType() const {
		return varType;
	}
	__inline uint8_t getVariableNumber() const {
		return varNumber;
	}
	__inline uint16_t getValue() const {
		return value;
	}
	__inline CheckOperationType getCheckOperationType() const {
		return operation;
	}
};

class QuestDataReward {
private:
	uint8_t varNumber;
	uint16_t varType;
	uint16_t value;
	ResultOperationType operation;
public:
	QuestDataReward() {
		varNumber = 0;
		varType = 0;
		value = 0;
		operation = ResultOperationType::UNKNOWN;
	}
	QuestDataReward(LoadedDataReader& reader) {
		varNumber = (uint8_t)reader.readUShort();
		varType = reader.readUShort();
		value = reader.readUShort();
		operation = OperationHandler::fromResultOperationTypeId(reader.readByte());
	}
	virtual ~QuestDataReward() {

	}
	__inline uint16_t getVariableType() const {
		return varType;
	}
	__inline uint8_t getVariableNumber() const {
		return varNumber;
	}
	__inline uint16_t getValue() const {
		return value;
	}
	__inline ResultOperationType getResultOperationType() const {
		return operation;
	}
};

class QuestAbilityTypeCheck {
private:
	EntityAbilityType type;
	uint8_t typeAsNumeric;
	int32_t value;
	CheckOperationType operation;
public:
	QuestAbilityTypeCheck() {
		type = EntityAbilityType::UNKNOWN;
		typeAsNumeric = static_cast<uint8_t>(type.getTypeId());
		value = 0;
		operation = CheckOperationType::UNKNOWN;
	}
	QuestAbilityTypeCheck(LoadedDataReader& reader) {
		typeAsNumeric = static_cast<uint8_t>(reader.readUInt());
		type = OperationHandler::fromEntityAbilityTypeId(typeAsNumeric);
		value = reader.readUInt();
		operation = OperationHandler::fromCheckOperationTypeId(reader.readUInt() & 0xFF);
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


class QuestAbilityTypeReward {
private:
	EntityAbilityType type;
	uint8_t typeAsNumeric;
	int32_t value;
	ResultOperationType operation;
protected:
	QuestAbilityTypeReward() {
		type = EntityAbilityType::UNKNOWN;
		typeAsNumeric = static_cast<uint8_t>(type.getTypeId());
		value = 0;
		operation = ResultOperationType::UNKNOWN;
	}
public:
	QuestAbilityTypeReward(LoadedDataReader& reader) {
		typeAsNumeric = static_cast<uint8_t>(reader.readUInt());
		type = OperationHandler::fromEntityAbilityTypeId(typeAsNumeric);
		value = reader.readUInt();
		operation = OperationHandler::fromResultOperationTypeId(reader.readUInt() & 0xFF);
	}
	virtual ~QuestAbilityTypeReward() {

	}
	__inline EntityAbilityType getEntityAbilityType() const {
		return type;
	}
	__inline uint32_t getValue() const {
		return value;
	}
	__inline ResultOperationType getCheckOperationType() const {
		return operation;
	}
};

class RequestedQuestItem {
private:
	Item questItem;
	uint8_t inventorySlot;
	CheckOperationType operation;
public:
	RequestedQuestItem() {
		questItem.clear();
		operation = CheckOperationType::UNKNOWN;
	}
	RequestedQuestItem(LoadedDataReader& reader) {
		uint32_t combinedItemTypeAndId = reader.readUInt();
		ItemType itemType = ItemTypeList::toItemType(combinedItemTypeAndId / 1000);
		uint16_t id = combinedItemTypeAndId % 1000;

		inventorySlot = static_cast<uint8_t>(reader.readUInt());

		questItem = Item(itemType, id);
		questItem.setAmount(reader.readUInt());

		uint32_t operationAsInt = reader.readUInt();
		operation = OperationHandler::fromCheckOperationTypeId(operationAsInt & 0xFF);
	}
	virtual ~RequestedQuestItem() {

	}
	__inline const Item& getRequestedItem() const {
		return questItem;
	}
	__inline uint8_t getInventorySlot() const {
		return inventorySlot;
	}
	__inline CheckOperationType getOperationType() const {
		return operation;
	}
};

class QuestReward : public QuestBlock {
public:
	QuestReward(std::shared_ptr<char> rawData) : QuestBlock(rawData) {
	}
	virtual ~QuestReward() {

	}

	virtual void performAction(QuestTriggerContext& context) {

	}
};

class QuestRecord {
private:
	bool checkNextRecordFlag;
	std::shared_ptr<QuestRecord> nextRecord;
	uint32_t qsdHash;
	std::shared_ptr<char> qsdName;
	std::shared_ptr<char> blockName;
	std::list<std::unique_ptr<QuestCondition>> conditions;
	std::list<std::unique_ptr<QuestReward>> actions;

	void readConditions(FileInputReader& reader, uint32_t conditionAmount);
	void readActions(FileInputReader& reader, uint32_t actionAmount);
public:
	QuestRecord(FileInputReader& reader, std::shared_ptr<char> blockName);
	virtual ~QuestRecord();

	__inline uint32_t getQuestHash() const {
		return qsdHash;
	}
	__inline std::shared_ptr<QuestRecord> getNextRecord() const {
		return nextRecord;
	}
	__inline void setNextRecord(std::shared_ptr<QuestRecord>& nextRecord) {
		this->nextRecord = nextRecord;
	}
	__inline std::shared_ptr<char> getBlockName() const {
		return blockName;
	}
	__inline bool hasToCheckNextRecord() const {
		return checkNextRecordFlag;
	}
	bool conditionsFulfilled(QuestTriggerContext& context);
	void handleRewards(QuestTriggerContext& context);

	__inline const std::list<std::unique_ptr<QuestCondition>>& getConditions() const {
		return conditions;
	}
	__inline const std::list<std::unique_ptr<QuestReward>>& getActions() const {
		return actions;
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