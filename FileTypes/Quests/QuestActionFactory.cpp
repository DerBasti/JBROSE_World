#include "QuestActionFactory.h"

QuestActionFactory::QuestActionFunctionPointer QuestActionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestAction>(new QuestAction(rawData)); }
};

std::unique_ptr<QuestAction> QuestActionFactory::createActionFromRawData(std::shared_ptr<char>& rawData) {
	uint32_t* rawDataAsInt = reinterpret_cast<uint32_t*>(rawData.get());
	uint32_t operationCode = rawDataAsInt[1] & 0xFFFF;

	auto result = creatorFunctions[operationCode](rawData);
	return result;
}