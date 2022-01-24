#include "AIActionFactory.h"
#include "AIActions.h"

AIActionFactory::AIActionFunctionPointer AIActionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new StopImmediatelyAction(rawData)); }, //Stop
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new ShowEmoteAction(rawData)); }, //Emote
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new SayBubbledMessageAction(rawData)); }, //Bubbled Message
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MoveToRandomPointFromCurrentPositionAction(rawData)); }, //Random point from current
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MoveToRandomPointFromSpawnPositionAction(rawData)); }, //Random point from spawn
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MoveToLastFoundTargetAction(rawData)); }, //Random point from target
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AttackBestTargetAction(rawData)); }, //AttackTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //SpecialAttack (Skill?)
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //MoveToTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Convert To Different Npc
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //SpawnPet
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new CallAlliesForHelpAction(rawData)); }, //CallAlliesForHelp
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AttackNearestTargetAction(rawData)); }, //Attack NearestTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AttackFoundTargetAction(rawData)); }, //AttackFoundTarget
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MakeFamilyAttackSameTargetWithinDistanceAction(rawData)); }, //Attack
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AttackDesignatedTargetAction()); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new RunAwayFromTargetAction(rawData)); }, //Run Away
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Drop Item
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //CallFewAlliesForHelp
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new MakeFamilyAttackSameTargetWithinDistanceAction(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //SpawnPet
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Kill self
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //??
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //?
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Cast Skill
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //ChangeNpcVar
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //ChanceWorldVar
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //ChangeEconomyVar
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Say Message (Local, Shout, Announcement)
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Follow Owner (as Summon)
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Run Quest Trigger
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Attack Owner's target
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Set PK Flag of Map
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //???
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Gift Owner item
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Set AI Variable
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Spawn Pets (from Monster?)
	[](std::shared_ptr<char>& rawData) { return std::shared_ptr<AIAction>(new AIAction(rawData)); }, //Spawn Pets (based on position - Source, Designated Target, current target)
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
	uint32_t operationCode = rawDataAsInt[1] - 1;

	auto result = creatorFunctions[operationCode & 0xFF](rawData);
	return result;
}