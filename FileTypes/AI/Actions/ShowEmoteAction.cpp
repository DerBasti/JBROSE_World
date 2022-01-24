#include "ShowEmoteAction.h"

ShowEmoteAction::ShowEmoteAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);
	emoteId = reader.readUShort();
}

ShowEmoteAction::~ShowEmoteAction() {

}

void ShowEmoteAction::performAction(AIContext& context) {

}

std::shared_ptr<char> ShowEmoteAction::toPrintable() const {
	char* buffer = new char[0x60];
	sprintf_s(buffer, 0x60, "[ShowEmoteAction] Play Emote Id: %i", emoteId);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}