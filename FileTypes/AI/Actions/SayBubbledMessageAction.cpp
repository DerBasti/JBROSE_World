#include "SayBubbledMessageAction.h"

SayBubbledMessageAction::SayBubbledMessageAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);

	this->messageTableId = reader.readUShort();
}

SayBubbledMessageAction::~SayBubbledMessageAction() {

}

void SayBubbledMessageAction::performAction(AIContext& context) {

}

std::shared_ptr<char> SayBubbledMessageAction::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[SayBubbledMessageAction] StringId in Speech-Table: %i", messageTableId);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}