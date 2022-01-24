#include "HasTargetChangedCondition.h"
#include "../../../Entities/NPC.h"

HasTargetChangedCondition::HasTargetChangedCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {

}

HasTargetChangedCondition::~HasTargetChangedCondition() {

}

bool HasTargetChangedCondition::isFulfilled(AIContext& context) {
	return context.getSourceEntity()->getCombat()->getTarget() != context.getDesignatedTarget();
}

std::shared_ptr<char> HasTargetChangedCondition::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[HasTargetChangedCondition] Check for: Current Target != Designated Target");
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}