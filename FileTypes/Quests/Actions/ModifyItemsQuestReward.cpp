#include "ModifyItemsQuestReward.h"
#include "../../../WorldClient.h"
#include "../../../BasicTypes/OperationHandler.h"

ModifyItemsQuestReward::ModifyItemsQuestReward(std::shared_ptr<char>& rawData) : QuestReward(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	uint32_t itemTypeAndId = reader.readUInt();
	ItemType itemType = ItemTypeList::toItemType(itemTypeAndId / 1000);
	uint16_t itemId = itemTypeAndId % 1000;

	operation = OperationHandler::fromResultOperationTypeId(static_cast<uint8_t>(ResultOperationTypeId::SUBTRACTION) - (reader.readUShort() & 0xFF));

	this->item = Item(itemType, itemId, reader.readUShort());
	affectsParty = reader.readByte() > 0;
}

ModifyItemsQuestReward::~ModifyItemsQuestReward() {

}

void ModifyItemsQuestReward::performAction(QuestTriggerContext& context) {
	if (!context.getTriggerEntity()->isPlayer() || context.getSelectedQuest() == nullptr) {
		return;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	if (item.getType() == ItemTypeList::QUEST) {
		QuestItemInventory* inventory = context.getSelectedQuest()->getQuestInventory();
		for (uint8_t i = 0; i < inventory->getMaxInventorySlots(); i++) {
			const Item& inventoryItem = inventory->getItem(i);
			if (inventoryItem.getId() == item.getId()) {
				if(operation == ResultOperationTypeId::ADDITION) {
					Item newItemState = Item(inventoryItem.getType(), inventoryItem.getId(), inventoryItem.getAmount() + item.getAmount());
					inventory->setItem(i, newItemState);
					break;
				}
				else if (operation == ResultOperationTypeId::SUBTRACTION) {
					if (inventoryItem.getAmount() >= item.getAmount()) {
						Item newItemState = Item(inventoryItem.getType(), inventoryItem.getId(), inventoryItem.getAmount() - item.getAmount());
						inventory->setItem(i, newItemState);
						break;
					}
				}
			}
			if (!inventoryItem.isValid() && operation == ResultOperationTypeId::ADDITION) {
				inventory->setItem(i, item);
				break;
			}
		}
	}
	else {
		PlayerInventory* inventory = player->getInventory();
		auto list = inventory->getSameItemsInInventory(item);
		if (!list.empty()) {
			bool operationSuccessful = false;
			for (auto it = list.begin(); it != list.end(); it++) {
				Item inventoryItem = it->second;
				if (operation == ResultOperationTypeId::ADDITION && it->second.getAmount() + item.getAmount() <= PlayerInventory::MAXIMUM_AMOUNT_PER_STACK) {
					inventoryItem.addAmount(item.getAmount());
					inventory->setItem(it->first, inventoryItem);
					return;
				}
				else if (operation == ResultOperationTypeId::SUBTRACTION && it->second.getAmount() >= item.getAmount()) {
					inventoryItem.setAmount(inventoryItem.getAmount() - item.getAmount());
					if (inventoryItem.getAmount() <= 0) {
						inventoryItem.clear();
					}
					inventory->setItem(it->first, inventoryItem);
					return;
				}
			}
		}
		if (operation == ResultOperationType::ADDITION) {
			inventory->addItemToInventory(item);
		}
	}
}
std::shared_ptr<char> ModifyItemsQuestReward::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[ModifyItemsQuestReward] ItemType: %s, Id: %i, Amount: %i, Operation: %s", item.getType().getTypeName(), item.getId(), item.getAmount(), operation.getName());
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}