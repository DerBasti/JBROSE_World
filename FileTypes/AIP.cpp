#include "AIP.h"
#include "AI/AIActionFactory.h"
#include "AI/AIConditionFactory.h"

const static uint32_t DEFAULT_LENGTH_STATE_STRING = 0x20;

AIP::AIP(const char* filePath) {
	FileInputReader reader(filePath);
	if (reader.isValid()) {
		uint32_t amountOfStates = reader.readUInt();
		logger.logTrace("Amount of States: " , amountOfStates);
		if (amountOfStates == DEFAULT_STATE_AMOUNT) {
			this->checkingIntervalInMilliseconds = reader.readUInt() * 1000;
			this->damageAmountTillTrigger = reader.readUInt();
			logger.logTrace("Checking Interval: ", checkingIntervalInMilliseconds);

			uint32_t stringLength = reader.readUInt();
			reader.skipBytes(stringLength);

			states = new AIState*[amountOfStates];
			for (uint32_t i = 0; i < amountOfStates; i++) {
				reader.skipBytes(DEFAULT_LENGTH_STATE_STRING);

				uint32_t recordAmountInState = reader.readUInt();
				logger.logTrace("Amount of Records in State[", i, "]: ", recordAmountInState);
				states[i] = new AIState(reader, recordAmountInState);
			}
		}
		else {
			logger.logError("Not all states covered for AIP: ", reader.getFilePath().get());
			states = nullptr;
		}
	}
}

AIP::~AIP() {
	if (states) {
		for (uint32_t i = 0; i < DEFAULT_STATE_AMOUNT; i++) {
			delete[] states[i];
			states[i] = nullptr;
		}
		delete[] states;
		states = nullptr;
	}
}



AIState::AIState() {
	amountOfRecords = 0;
	records = nullptr;
}

AIState::AIState(FileReader& reader, uint32_t amountOfRecords) {
	this->amountOfRecords = amountOfRecords;
	this->records = (amountOfRecords == 0 ? nullptr : new AIStateRecord*[amountOfRecords]);
	for (uint32_t i = 0; i < amountOfRecords; i++) {
		reader.skipBytes(DEFAULT_LENGTH_STATE_STRING);
		this->records[i] = new AIStateRecord(reader);
	}
}

AIState::~AIState() {
	if (records) {
		for (uint32_t i = 0; i < amountOfRecords; i++) {
			delete[] records[i];
			records[i] = nullptr;
		}
		delete[] records;
		records = nullptr;
	}
}


AIStateRecord::AIStateRecord() {

}

AIStateRecord::AIStateRecord(FileReader& reader) {
	readConditions(reader);
	readActions(reader);
}

AIStateRecord::~AIStateRecord() {

}

bool AIStateRecord::conditionsFulfilled(AIContext& context) {
	for (uint32_t i = 0; i < conditions.size(); i++) {
		auto condition = conditions.at(i);
		if (!condition->isFulfilled(context)) {
			return false;
		}
	}
	return true;
}

void AIStateRecord::performAction(AIContext& context) {
	for (uint32_t i = 0; i < actions.size(); i++) {
		auto action = actions.at(i);
		action->performAction(context);
	}
}

void AIStateRecord::readConditions(FileReader& reader) {
	uint32_t conditionAmount = reader.readUInt();
	for (uint32_t i = 0; i < conditionAmount; i++) {
		uint32_t dataLength = reader.readUInt();
		reader.resetCaretTo(reader.getCaret() - sizeof(uint32_t));
		std::shared_ptr<char> condition = reader.readStringWrapped(dataLength);
		auto conditionBlock = AIConditionFactory::createConditionFromRawData(condition);
		conditions.push_back(conditionBlock);
	}
}

void AIStateRecord::readActions(FileReader& reader) {
	uint32_t actionAmount = reader.readUInt();
	for (uint32_t i = 0; i < actionAmount; i++) {
		uint32_t dataLength = reader.readUInt();
		reader.resetCaretTo(reader.getCaret() - sizeof(uint32_t));
		std::shared_ptr<char> action = reader.readStringWrapped(dataLength);
		auto actionBlock = AIActionFactory::createActionFromRawData(action);
		actions.push_back(actionBlock);
	}
}

