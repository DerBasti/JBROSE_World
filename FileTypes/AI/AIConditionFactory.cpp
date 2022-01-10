#include "AIConditionFactory.h"
#include "AIConditions.h"

AIConditionFactory::AIConditionFunctionPointer AIConditionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new FightOrDelayCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new HasEnoughDamageReceivedCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new HasEnoughTargetsCondition(rawData)); }, //HasEnoughTargets
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new DistanceFromSpawnCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CheckDistanceToTargetCondition(rawData)); }, //DistanceToTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //AbilityDifference
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CheckCurrentHpPercentageCondition(rawData)); }, //Check Hp percentage
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new RandomPercentageCondition(rawData)); }, //Check Random percentage
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //FindNearestBestTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //HasTargetChanged
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CompareAbilityValueOfEntityCondition(rawData)); }, //CompareAbilities
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new StatSufficientCondition(rawData)); }, //IsStatSufficient
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //HasDayTimeArrived
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }
};

std::shared_ptr<AICondition> AIConditionFactory::createConditionFromRawData(std::shared_ptr<char>& rawData) {
	uint32_t* rawDataAsInt = reinterpret_cast<uint32_t*>(rawData.get());
	uint32_t operationCode = rawDataAsInt[1] - 1;

	auto result = creatorFunctions[operationCode & 0xFF](rawData);
	return result;
}