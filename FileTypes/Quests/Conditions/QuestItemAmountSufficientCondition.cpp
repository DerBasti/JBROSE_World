#include "QuestItemAmountSufficientQuestCondition.h"
#include "../../../WorldClient.h"

QuestItemAmountSufficientQuestCondition::QuestItemAmountSufficientQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	this->amountOfItems = reader.readUInt();
	this->requestedItems = new RequestedQuestItem[amountOfItems];
	for (uint32_t i = 0; i < amountOfItems; i++) {
		requestedItems[i] = RequestedQuestItem(reader);
	}
}

QuestItemAmountSufficientQuestCondition::~QuestItemAmountSufficientQuestCondition() {
	delete[] requestedItems;
	requestedItems = nullptr;
}

bool QuestItemAmountSufficientQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	if (!context.getTriggerEntity()->isPlayer()) {
		logger.logWarn("NPC '", context.getTriggerEntity()->getName(), "' tried triggering QuestItemAmountSufficientQuestCondition.");
		return false;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	for (uint32_t i = 0; i < amountOfItems; i++) {
		const Item& requestedItem = requestedItems[i].getRequestedItem();
		std::map<uint8_t, Item> inventoryItemList;
		if (requestedItem.getType() == ItemTypeList::QUEST || requestedItems[i].getInventorySlot() == ItemTypeList::QUEST.getInventorySlotId()) {
			inventoryItemList = context.getSelectedQuest()->getQuestInventory()->getSameItemsInInventory(requestedItem);
			bool found = false;
			for (auto pair : inventoryItemList) {
				if (OperationHandler::executeCheckOperation(pair.second.getAmount(), requestedItem.getAmount(), requestedItems[i].getOperationType())) {
					found = true;
					break;
				}
			}
			if (!found) {
				return OperationHandler::executeCheckOperation(0u, requestedItem.getAmount(), requestedItems[i].getOperationType());
			}
		}
		else {
			const Item& inventoryItem = player->getInventory()->getItem(requestedItems[i].getInventorySlot());
			if (inventoryItem.getId() != requestedItem.getId() || !OperationHandler::executeCheckOperation(inventoryItem.getAmount(), requestedItem.getAmount(), requestedItems[i].getOperationType())) {
				return false;
			}
		}
	}
	return true;
}

std::shared_ptr<char> QuestItemAmountSufficientQuestCondition::toPrintable() const {
	char *buffer = new char[0x350];
	sprintf_s(buffer, 0x350, "[QuestItemAmountSufficientQuestCondition] Amount of Checks: %i", amountOfItems);
	for (uint32_t i=0;i< amountOfItems;i++) {
		RequestedQuestItem check = requestedItems[i];
		uint32_t len = (uint32_t)strlen(buffer);
		sprintf_s(&buffer[len], 0x350 - len, " | ItemType: %s, Id: %i, Amount: %i, Requested slot to check: %i, Operation: %s", check.getRequestedItem().getType().getTypeName(), check.getRequestedItem().getId(), check.getRequestedItem().getAmount(), check.getInventorySlot(), check.getOperationType().getName());
	}
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}