#include "StopImmediatelyAction.h"
#include "../../../Entities/NPC.h"

StopImmediatelyAction::StopImmediatelyAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {

}
StopImmediatelyAction::~StopImmediatelyAction() {

}
void StopImmediatelyAction::performAction(AIContext& context) {
	context.getSourceEntity()->getCombat()->clear();
	context.getSourceEntity()->getLocationData()->getMapPosition()->setDestinationPositionVisually(context.getSourceEntity()->getLocationData()->getMapPosition()->getCurrentPosition());
}

std::shared_ptr<char> StopImmediatelyAction::toPrintable() const {
	char *buffer = new char[0x60];
	sprintf_s(buffer, 0x60, "[StopImmediatelyAction] Stop combat and stand in current position");
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}