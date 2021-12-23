#include "AIActionFactory.h"
#include "AIActions.h"

AIActionFactory::AIActionFunctionPointer AIActionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Stop
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Emote
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Bubbled Message
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MoveToRandomPointFromCurrentPositionAction(rawData)); }, //Random point from current
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MoveToRandomPointFromSpawnPositionAction(rawData)); }, //Random point from spawn
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MoveToLastFoundTargetAction(rawData)); }, //Random point from target
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MakeFamilyAttackSameTargetWithinDistanceAction(rawData)); }, //Attackhelper
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AttackDesignatedTargetAction()); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }
};

std::shared_ptr<AIAction> AIActionFactory::createActionFromRawData(std::shared_ptr<char>& rawData) {
	uint32_t* rawDataAsInt = reinterpret_cast<uint32_t*>(rawData.get());
	uint32_t operationCode = rawDataAsInt[1];

	auto result = creatorFunctions[operationCode - CONDITION_OPERATIONCODE_START](rawData);
	return result;
}