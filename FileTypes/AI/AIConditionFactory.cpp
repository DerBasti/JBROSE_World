#include "AIConditionFactory.h"
#include "AIConditions.h"

AIConditionFactory::AIConditionFunctionPointer AIConditionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new FightOrDelayCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new HasEnoughDamageReceivedCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new HasEnoughTargetsCondition(rawData)); }, //HasEnoughTargets
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new DistanceFromSpawnCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CheckDistanceToTargetCondition(rawData)); }, //DistanceToTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CompareAbilityDifferenceCondition(rawData)); }, //AbilityDifference
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CheckCurrentHpPercentageCondition(rawData)); }, //Check Hp percentage
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new RandomPercentageCondition(rawData)); }, //Check Random percentage
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new FindFirstBestTargetWithinReachCondition(rawData)); }, //FindNearestBestTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new HasTargetChangedCondition(rawData)); }, //HasTargetChanged
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CompareAbilityValueOfEntityCondition(rawData)); }, //CompareAbilities
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new StatSufficientCondition(rawData)); }, //IsStatSufficient
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new CertainMapTimeTypeCondition(rawData)); }, //HasDayTimeArrived
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //HasCertainBuffType
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //ObjectVarValidity
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //WorldVarValidity
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //EconomyVarValidity
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //CertainNpcTypeNearby
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //DistanceToOwner
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //CheckActualZoneTime
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //AreOwnStatsSufficient
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //HasNoOwner
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //HasOwner
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //WorldTimeCheck
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //WeekdayCheck
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AICondition>(new AICondition(rawData)); }, //MonthDayCheck
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