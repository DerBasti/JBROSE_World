#include "MoveToLastFoundTarget.h"
#include "../../../Entities/NPC.h"

MoveToLastFoundTargetAction::MoveToLastFoundTargetAction(std::shared_ptr<char>& rawAction) : AIAction(rawAction) {
	LoadedDataReader interpreter(rawAction.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);
	stanceId = interpreter.readByte();
}

MoveToLastFoundTargetAction::~MoveToLastFoundTargetAction() {

}

void MoveToLastFoundTargetAction::performAction(AIContext& context) {
	if (context.getLastFoundTarget() == nullptr) {
		return;
	}
	NPC* npc = context.getSourceEntity();
	npc->getStance()->setStanceById(stanceId);
	npc->getLocationData()->getMapPosition()->setDestinationPosition(
		std::move(PositionProcessor::generateRandomPointAroundPosition(context.getLastFoundTarget()->getLocationData()->getMapPosition()->getCurrentPosition(), 200))
	);
}

std::shared_ptr<char> MoveToLastFoundTargetAction::toPrintable() const {
	char *buffer = new char[0x90];
	sprintf_s(buffer, 0x90, "[MoveToLastFoundTargetAction] Move to last found target with stance: %i", stanceId);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}