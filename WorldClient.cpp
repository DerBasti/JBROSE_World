#include "WorldClient.h"
#include "WorldPackets\RequestPackets.h"
#include "WorldPackets\ResponsePackets.h"
#include "WorldServer.h"
#include "Map/MapSector.h"
#include "..\JBROSE_Common\EncryptionHandler.h"
#include "Map/MapRemovalRequest.h"
#include "FileTypes/Quests/PlayerQuestJournal.h"
#include "FileTypes/QSD.h"
#include "Entities/Drop.h"
#include "Entities/Monster.h"
#include <iostream>

PlayerAttributeTypes::PlayerAttributesCallback PlayerAttributeTypes::functions[] = {
	&PlayerAttributeTypes::getStrength,
	&PlayerAttributeTypes::getDexterity,
	&PlayerAttributeTypes::getIntelligence,
	&PlayerAttributeTypes::getCharm,
	&PlayerAttributeTypes::getConcentration,
	&PlayerAttributeTypes::getSensibility
};

Player::Player(std::shared_ptr<ROSEClient>& networkInterface) {
	packetHandler = new PlayerPacketHandler(networkInterface);
	logger.setLoggerName("Player");

	questJournal = new PlayerQuestJournal();
	regenerationProcessor = new RegenerationProcessor(this);
	stats = new PlayerStats();
	traits = new PlayerTraits();
	inventory = new PlayerInventory();
	attributes = new PlayerAttributeTypes();
	visualityProcessor = new PlayerVisualityProcessor(this);
	skillList = SkillList<140>(WorldServer::getInstance()->getSkillSTB());

	getCombat()->setTeamId(EntityDefaultTeamId::PLAYER);

	stance = new PlayerStance([&]() {
		updateMovementSpeed();
		updateStanceVisually();
	});
	stance->setRunningStance();

}

Player::~Player() {
	logger.logDebug("~Player()");

	delete packetHandler;
	packetHandler = nullptr;

	delete stats;
	stats = nullptr;

	delete traits;
	traits = nullptr;

	delete inventory;
	inventory = nullptr;

	delete attributes;
	attributes = nullptr;

	delete visualityProcessor;
	visualityProcessor = nullptr;

	delete stance;
	stance = nullptr;

	delete questJournal;
	questJournal = nullptr;
}

void Player::onDisconnect() {
	auto map = getLocationData()->getMap();
	if (map) {
		map->addEntityToRemovalQueue(this, RemovalReason::PLAYER_DISCONNECT);
	}
}

void Player::onUpdate() {
	regenerationProcessor->checkRegeneration();
}

const char* Player::getName() const {
	return getTraits()->getName();
}

bool Player::handlePacket(const Packet* packet) {
	return getPacketHandler()->handlePacket(this, packet);
}

void Player::onDamageReceived(Entity* attacker, uint32_t damageAmount) {
	logger.logDebug(getName(), "(#", getLocationData()->getLocalId(), ") received ", damageAmount, " damage from ", attacker->getName(), ".");
}

PlayerPacketHandler::PlayerPacketHandler(std::shared_ptr<ROSEClient>& networkConnection) {
	this->networkConnection = networkConnection;

	this->handleMethods.insert(std::make_pair(AssignLocalPlayerIdRequestPacket::ID, &PlayerPacketHandler::handleAssignmentOfLocalId));
	this->handleMethods.insert(std::make_pair(ChangeEquipmentRequestPacket::ID, &PlayerPacketHandler::handleChangedEquipment));
	this->handleMethods.insert(std::make_pair(ChangeRespawnTownRequestPacket::ID, &PlayerPacketHandler::handleChangedRespawnTown));
	this->handleMethods.insert(std::make_pair(CollisionRequestPacket::ID, &PlayerPacketHandler::handleCollision));
	this->handleMethods.insert(std::make_pair(CloseShopRequestPacket::ID, &PlayerPacketHandler::handleShopClose));
	this->handleMethods.insert(std::make_pair(CurrentWeightRequestPacket::ID, &PlayerPacketHandler::handleWeightChange));
	this->handleMethods.insert(std::make_pair(DistributeStatPointRequestPacket::ID, &PlayerPacketHandler::handleDistributionOfStatPoint));
	this->handleMethods.insert(std::make_pair(DropItemFromInventoryRequestPacket::ID, &PlayerPacketHandler::handleDropFromInventory));
	this->handleMethods.insert(std::make_pair(ExitRequestPacket::ID, &PlayerPacketHandler::handleExit));
	this->handleMethods.insert(std::make_pair(IdentifyAccountRequestPacket::ID, &PlayerPacketHandler::handleIdentification));
	this->handleMethods.insert(std::make_pair(InitBasicAttackRequestPacket::ID, &PlayerPacketHandler::handleInitBasicAttack));
	this->handleMethods.insert(std::make_pair(NewDestinationRequestPacket::ID, &PlayerPacketHandler::handleNewDestination));
	this->handleMethods.insert(std::make_pair(PickupDropRequestPacket::ID, &PlayerPacketHandler::handlePickupDrop));
	this->handleMethods.insert(std::make_pair(PingRequestPacket::ID, &PlayerPacketHandler::handleEmptyPacket));
	this->handleMethods.insert(std::make_pair(QuestJournalUpdateRequestPacket::ID, &PlayerPacketHandler::handleQuestJournalUpdate));
	this->handleMethods.insert(std::make_pair(ShowMonsterHpRequestPacket::ID, &PlayerPacketHandler::handleShowMonsterHp));
	this->handleMethods.insert(std::make_pair(StanceRequestPacket::ID, &PlayerPacketHandler::handleStanceChange));
	this->handleMethods.insert(std::make_pair(TelegateRequestPacket::ID, &PlayerPacketHandler::handleTelegateEntered));
	this->handleMethods.insert(std::make_pair(UseConsumableRequestPacket::ID, &PlayerPacketHandler::handleUseConsumableItem));
}

bool PlayerPacketHandler::handlePacket(Player* player, const Packet* packet) {
	//Add new Packets to WorldServerPacketFactory
	bool success = false;
	auto it = handleMethods.find(packet->getCommandId());
	if (it != handleMethods.end()) {
		auto method = it->second;
		success = (this->*method)(player, packet);
	}
	else {
		logger.logWarn("Unknown Packet: ", packet->toPrintable().c_str());
	}
	return success;
}


void Player::addExperience(uint32_t expGained) {
	auto playerStats = getStats();
	playerStats->addExperience(expGained);
	logger.logTrace("Added ", expGained, " experience to player '", getName(), "'.");
	bool hasLevelup = playerStats->getExperiencePoints() >= getExperienceForLevelup();
	if (hasLevelup) {
		logger.logTrace("Levelup detected.");
		do {
			uint32_t levelupExp = getExperienceForLevelup();
			playerStats->setExperiencePoints(playerStats->getExperiencePoints() - levelupExp);
			handleLevelup();
			hasLevelup = playerStats->getExperiencePoints() >= getExperienceForLevelup();
		} while (hasLevelup);
	}
	else {
		UpdateExperienceResponsePacket packet(this);
		getPacketHandler()->sendDataToClient(packet);
	}
}

bool Player::handleLevelup() {
	PlayerStats* stats = getStats();
	stats->addLevel();

	updateCombatValues();
	stats->setCurrentHp(stats->getMaxHp());
	stats->setCurrentMp(stats->getMaxMp());

	stats->addAvailableStatPoints((getStats()->getLevel() * 10 / 8) + 10);
	uint16_t currentLevel = getStats()->getLevel();
	if(currentLevel == 10 || currentLevel == 14) {
		stats->addAvailableSkillPoints(2);
	}
	else if (currentLevel == 18) {
		stats->addAvailableSkillPoints(3);
	}
	else if (currentLevel == 22) {
		stats->addAvailableSkillPoints(4);
	}
	else if ((currentLevel > 22 && ((currentLevel - 22) % 4 == 0)) || (currentLevel >= 100 && (currentLevel % 2) == 0)) {
		stats->addAvailableSkillPoints(5);
	}
	return sendLevelupToVisibleEntities();
}

bool Player::sendLevelupToVisibleEntities() {
	LevelupForSelfResponsePacket packetForSelf(this);
	auto packetForOthers = std::shared_ptr<LevelupForOthersResponsePacket>(new LevelupForOthersResponsePacket(getLocationData()->getLocalId()));

	return this->getPacketHandler()->sendDataToClient(packetForSelf) && sendDataToVisibleExceptSelf(packetForOthers);
}

uint32_t Player::getExperienceForLevelup() const {
	uint32_t result = 0x00;
	uint16_t level = this->getStats()->getLevel();
	if (level <= 15) {
		result = static_cast<uint32_t>((level + 10) * (level + 5) * (level + 3) * 0.7);
	}
	else if (level <= 50) {
		result = static_cast<uint32_t>((level - 5) * (level + 2) * (level + 2) * 2.2);
	}
	else if (level <= 100) {
		result = static_cast<uint32_t>((level - 38) * (level - 5) * (level + 2) * 9);
	}
	else if (level <= 139) {
		result = static_cast<uint32_t>((level + 220) * (level + 34) * (level + 22));
	}
	else {
		result = static_cast<uint32_t>((level - 126) * (level - 15) * (level + 7) * 41);
	}
	logger.logDebug("Experience for levelup necessary for '", getName(), "': ", result, " (Current Level: ", level, ").");
	return result;
}


bool PlayerPacketHandler::handleIdentification(Player* player, const Packet* packet) {
	const IdentifyAccountRequestPacket* identPacket = dynamic_cast<const IdentifyAccountRequestPacket*>(packet);

	if (!sendEncryption() || !player->loadPlayerDataFromAccount(identPacket->getAccountId())) {
		return false;
	}

	bool success = sendDataToClient(CharacterDataResponsePacket(player));
	success &= sendDataToClient(InventoryResponsePacket(player->getInventory()));
	success &= sendDataToClient(QuestDataResponsePacket(player->getQuestJournal()));
	success &= sendDataToClient(GamingPlanResponsePacket());

	return success;
}

bool PlayerPacketHandler::sendEncryption() {
	WorldEncryptionPacket encryptionPacket;
	encryptionPacket.setEncryptionKey(CryptTable::DEFAULT_CRYPTTABLE_START_VALUE);

	return getConnectionWrapper()->sendData(encryptionPacket);
}

bool Player::loadPlayerDataFromAccount(uint32_t accountId) {
	setAccountId(accountId);

	WorldServer *server = WorldServer::getInstance();
	uint32_t charId = server->getLastLoggedCharacterFromAccount(accountId);
	getTraits()->setCharacterId(charId);
	bool success = server->loadCharacterDataForCharacter(this);
	success &= server->loadSkilledAttributesForCharacter(this);
	success &= server->loadInventoryForCharacter(this);

	updateCombatValues();
	getStats()->setCurrentHp(getStats()->getMaxHp());
	getStats()->setCurrentMp(getStats()->getMaxMp());
	return success;
}


bool PlayerPacketHandler::handleAssignmentOfLocalId(Player* player, const Packet* packet) {
	bool success = player->getLocationData()->getMap()->addEntityToInsertionQueue(player);
	if (success) {
		player->updateCombatValues();
		AssignLocalPlayerIdResponsePacket response(player);

		StanceResponsePacket stanceResponse;
		stanceResponse.setLocalEntityId(player->getLocationData()->getLocalId());
		stanceResponse.setMovementSpeed(player->getStats()->getMovementSpeed());
		stanceResponse.setStanceType(player->getStance()->getStanceId());

		success &= sendDataToClient(response);
		success &= handleWeightChange(player, nullptr);
		success &= sendDataToClient(stanceResponse);
	}

	return success;
}

bool PlayerPacketHandler::handleChangedRespawnTown(Player* player, const Packet* packet) {
	const ChangeRespawnTownRequestPacket* respawnTownPacket = dynamic_cast<const ChangeRespawnTownRequestPacket*>(packet);
	logger.logDebug("[HandleChangedRespawnTown-DEBUG]: ", respawnTownPacket->toPrintable().c_str());
	return true;
}

bool PlayerPacketHandler::handleWeightChange(Player* player, const Packet* packet) {
	CurrentWeightResponsePacket response;
	response.setLocalEntityId(player->getLocationData()->getLocalId());
	response.setWeightPercentage(static_cast<uint8_t>(player->getInventory()->getTotalWeight() * 100.0f / player->getStats()->getMaximumPossibleWeight()));

	return sendDataToClient(response);
}

bool PlayerPacketHandler::handleDistributionOfStatPoint(Player* player, const Packet *packet) {
	const DistributeStatPointRequestPacket* statPacket = dynamic_cast<const DistributeStatPointRequestPacket*>(packet);
	auto wantedStatType = player->getAttributes()->getAttributeByType(statPacket->getStatTypeId());
	uint16_t statPointsRequired = (wantedStatType->getPointsLearned() / 5);
	if (player->getStats()->getAvailableStatPoints() < statPointsRequired) {
		return true;
	}
	wantedStatType->increasePointsLearned();
	player->getStats()->setAvailableStatPoints(player->getStats()->getAvailableStatPoints() - statPointsRequired);
	player->updateCombatValues();

	DistributeStatPointResponsePacket response(wantedStatType, statPacket->getStatTypeId());
	return sendDataToClient(response);
}

bool PlayerPacketHandler::handleExit(Player* player, const Packet* packet) {
	ExitResponsePacket response;
	sendDataToClient(response);
	return false;
}

bool PlayerPacketHandler::handleNewDestination(Player* player, const Packet* packet) {
	const NewDestinationRequestPacket* destinationPacket = dynamic_cast<const NewDestinationRequestPacket*>(packet);

	if (destinationPacket->getTargetLocalId() != 0x00) {
		auto target = player->getVisualityProcessor()->findEntity(destinationPacket->getTargetLocalId());
		if (target == nullptr) {
			logger.logWarn("Invalid target (#: ", destinationPacket->getTargetLocalId(), ") by player: ", player->getName());
			return false;
		}
		logger.logDebug("New target: ", target->getName(), " (#: ", destinationPacket->getTargetLocalId(), ")");
		player->getCombat()->setTarget(target, CombatType::NONE);
	}
	else {
		player->getCombat()->clear();
	}

	Position destination = Position(destinationPacket->getDestinationX(), destinationPacket->getDestinationY());
	player->getLocationData()->getMapPosition()->setDestinationPositionVisually(std::move(destination));

	return true;
}

bool Player::spawnVisually(Entity* entity) {
	auto packet = entity->getVisualityProcessor()->createSpawnVisuallyPacket();
	return getPacketHandler()->sendDataToClient(*packet);
}

bool Player::despawnVisually(Entity* entity) {
	return getPacketHandler()->sendDataToClient(DespawnEntityVisuallyResponsePacket(entity));
}

bool Player::despawnVisually(uint16_t localId) {
	return getPacketHandler()->sendDataToClient(DespawnEntityVisuallyResponsePacket(localId));
}

PickupDropResponsePacket Player::onDropPickup(Drop* drop) {
	PickupDropResponsePacket response(drop->getLocationData()->getLocalId());
	if (drop->getDropOwner() != nullptr && drop->getDropOwner() != this) {
		response.setDropMessageType(PickupDropMessageType::NOT_OWNER);
		return response;
	}
	const Item& item = drop->getItem();
	uint8_t slot = getInventory()->getSlotForItem(item);
	if (slot == InventorySlot::UNKNOWN_SLOT) {
		response.setDropMessageType(PickupDropMessageType::INVENTORY_FULL);
		return response;
	}
	if (item.getType() == ItemTypeList::MONEY) {
		response.setPreviousMoneyAmount(getInventory()->getMoneyAmount());
	}
	uint8_t slotId = getInventory()->addItemToInventory(item);
	response.setDropMessageType(PickupDropMessageType::OKAY);
	response.setItemToAdd(slotId, getInventory()->getItem(slotId));
	return response;
}

bool PlayerPacketHandler::handleTelegateEntered(Player* player, const Packet* packet) {
	const TelegateRequestPacket* telegatePacket = dynamic_cast<const TelegateRequestPacket*>(packet);
	const uint16_t telegateId = telegatePacket->getTelegateId();

	WorldServer *server = WorldServer::getInstance();
	return server->teleportPlayerFromTelegate(player, telegateId);
}

bool PlayerPacketHandler::handleUseConsumableItem(Player* player, const Packet* packet) {
	const UseConsumableRequestPacket* usePacket = dynamic_cast<const UseConsumableRequestPacket*>(packet);
	if (!player->getInventory()->isValidSlot(usePacket->getInventorySlot())) {
		logger.logWarn("Invalid UseItem-Slot: ", usePacket->getInventorySlot());
		return false;
	}
	const Item& item = player->getInventory()->getItem(usePacket->getInventorySlot());
	if (item.getType() != ItemTypeList::CONSUMABLE) {
		logger.logWarn("Invalid ItemType for UseItem is '", item.getType().getTypeName(), "'! [Player: ", player->getName(), ", Slot: ", usePacket->getInventorySlot(), "].");
		return false;
	}
	auto consumable = WorldServer::getInstance()->getConsumableItemList()->createConsumable(item.getId());
	bool consumeSuccessful = true;
	switch (consumable->getExecutionType()) {
		case ConsumableExecutionType::FOOD:
			consumeSuccessful = player->getRegenerationProcessor()->addConsumedItem(consumable);
		break;
		case ConsumableExecutionType::IMMEDIATE_APPLICATION:
			consumeSuccessful = player->getRegenerationProcessor()->addImmediateUseItem(consumable);
		break;
		case ConsumableExecutionType::REPAIR_HAMMER:

		break;
		case ConsumableExecutionType::LEARN_SKILL:

		break;
	}
	if (!consumeSuccessful) {
		return true;
	}
	auto response = std::shared_ptr<UseConsumableResponsePacket>(new UseConsumableResponsePacket(player->getLocationData()->getLocalId(), item.getId()));
	player->sendDataToVisibleEntities(response);

	UseConsumableResponsePacket userResponse(player->getLocationData()->getLocalId(), item.getId());
	userResponse.setInventorySlotId(usePacket->getInventorySlot());
	player->sendDataToSelf(userResponse);
	return true;
}

bool PlayerPacketHandler::handleEmptyPacket(Player* player, const Packet* packet) {
	return true;
}

bool PlayerPacketHandler::handleShopClose(Player* player, const Packet* packet) {
	return true;
}

bool PlayerPacketHandler::handleCollision(Player* player, const Packet* packet) {
	const CollisionRequestPacket* collisionPacket = dynamic_cast<const CollisionRequestPacket*>(packet);
	const Position& current = player->getLocationData()->getMapPosition()->getCurrentPosition();
	float distance = PositionProcessor::getDistanceBetweenPoints(current, collisionPacket->getCollisionPosition());
	if (distance >= 200.0f) { //1m
		logger.logWarn("[Collision]: Distance >= 1m (", distance, "). Current: (", current, "), Collision: (", collisionPacket->getCollisionPosition(), ")");
		return false;
	}
	player->getLocationData()->getMapPosition()->setCurrentPosition(collisionPacket->getCollisionPosition());
	player->getLocationData()->getMapPosition()->setDestinationPosition(collisionPacket->getCollisionPosition());

	auto response = std::shared_ptr<CollisionResponsePacket>(new CollisionResponsePacket());
	response->setEntityLocalId(player->getLocationData()->getLocalId());
	response->setCollisionPosition(player->getLocationData()->getMapPosition()->getCurrentPosition());
	response->setZ(collisionPacket->getZ());

	return player->sendDataToVisibleEntities(response);
}

bool PlayerPacketHandler::handleStanceChange(Player* player, const Packet* packet) {
	const StanceRequestPacket* stancePacket = dynamic_cast<const StanceRequestPacket*>(packet);
	uint8_t stanceCase = stancePacket->getStanceId();
	switch (stanceCase) {
		case 0x00:
			if (player->getStance()->isWalking()) {
				player->getStance()->setRunningStance();
			}
			else if (player->getStance()->isRunning()) {
				player->getStance()->setWalkingStance();
			}
		break;
		case 0x01:
			if (player->getStance()->isSitting()) {
				player->getStance()->setRunningStance();
			}
			else if (!player->getStance()->isDriving() && !player->getStance()->isSitting()) {
				player->getStance()->setSittingStance();
			}
		break;
		case 0x02:
			//Driving
		break;
	}

	auto response = std::shared_ptr<StanceResponsePacket>(new StanceResponsePacket());
	response->setLocalEntityId(player->getLocationData()->getLocalId());
	response->setMovementSpeed(player->getStats()->getMovementSpeed());
	response->setStanceType(player->getStance()->getStanceId());

	return player->sendDataToVisibleEntities(response);
}


bool PlayerPacketHandler::handleShowMonsterHp(Player* player, const Packet* packet) {
	const ShowMonsterHpRequestPacket* showHpRequestPacket = dynamic_cast<const ShowMonsterHpRequestPacket*>(packet);
	Entity* monsterEntity = player->getVisualityProcessor()->findEntity(showHpRequestPacket->getMonsterLocalId());
	bool success = monsterEntity != nullptr && monsterEntity->isMonster();
	if (success) {
		ShowMonsterHpResponsePacket response(dynamic_cast<Monster*>(monsterEntity));
		success = sendDataToClient(response);
	}
	return success;
}

bool PlayerPacketHandler::handleQuestJournalUpdate(Player* player, const Packet* packet) {
	const QuestJournalUpdateRequestPacket* requestPacket = dynamic_cast<const QuestJournalUpdateRequestPacket*>(packet);
	
	QuestJournalUpdateResult updateResult;
	switch (requestPacket->getUpdateActionType()) {
		case QuestJournalUpdateRequestAction::ADD_QUEST:
		{
			logger.logWarn("Adding quest requested without running a trigger...");
			PlayerQuest* quest = new PlayerQuest(requestPacket->getQuestHash());
			if(player->getQuestJournal()->addQuestToJournal(quest)) {
				updateResult = QuestJournalUpdateResult::ADD_SUCCESSFUL;
			}
			else {
				delete quest;
				quest = nullptr;
				updateResult = QuestJournalUpdateResult::ADD_FAILED;
			}
		}
		break;
		case QuestJournalUpdateRequestAction::DELETE_QUEST:
			if (player->getQuestJournal()->deleteQuestWithQuestId(requestPacket->getQuestHash())) {
				updateResult = QuestJournalUpdateResult::DELETE_SUCCESSFUL;
			}
			else {
				updateResult = QuestJournalUpdateResult::DELETE_FAILED;
			}
		break;
		case QuestJournalUpdateRequestAction::RUN_TRIGGER:
			auto record = WorldServer::getInstance()->getQuestRecordWithQuestHash(requestPacket->getQuestHash());
			if (!record) {
				logger.logWarn("Invalid quest hash requested for QuestTrigger run by '", player->getName(), "'.");
				return false;
			}
			if (player->handleQuestTriggerRun(record)) {
				updateResult = QuestJournalUpdateResult::TRIGGER_RUN_SUCCESSFUL;
			}
			else {
				updateResult = QuestJournalUpdateResult::TRIGGER_RUN_FAILED;
			}
		break;
	}

	QuestJournalUpdateResponsePacket response(updateResult);
	response.setQuestHash(requestPacket->getQuestHash());
	response.setQuestJournalSlot(requestPacket->getQuestJournalSlot());

	return sendDataToClient(response);
}

bool PlayerPacketHandler::handleInitBasicAttack(Player* player, const Packet* packet) {
	const InitBasicAttackRequestPacket* attackRequestPacket = dynamic_cast<const InitBasicAttackRequestPacket*>(packet);
	Entity* newTarget = player->getVisualityProcessor()->findEntity(attackRequestPacket->getTargetLocalId());
	if (newTarget == nullptr || newTarget->isAlliedTo(player)) {
		return false;
	}
	player->getCombat()->clear();
	player->getCombat()->setTarget(newTarget, newTarget->isDrop() ? CombatType::NONE : CombatType::BASIC_ATTACK);
	return true;
}

bool PlayerPacketHandler::handleChangedEquipment(Player* player, const Packet* packet) {
	const ChangeEquipmentRequestPacket* changePacket = dynamic_cast<const ChangeEquipmentRequestPacket*>(packet);
	uint8_t destinationSlot = changePacket->getDestinationSlotId();
	PlayerInventory* inventory = player->getInventory();
	if (changePacket->getDestinationSlotId() == 0x00) {
		destinationSlot = inventory->getSlotForItem(inventory->getItem(changePacket->getSourceSlotId()));
	}
	if (destinationSlot <= 0 || destinationSlot >= inventory->getMaxInventorySlots()) {
		return true;
	}

	Item tempItem = inventory->getItem(changePacket->getSourceSlotId());
	inventory->setItem(changePacket->getSourceSlotId(), inventory->getItem(destinationSlot));
	inventory->setItem(destinationSlot, tempItem);

	player->updateCombatValues();

	UpdateInventorySlotsResponsePacket slotPacket;
	slotPacket.addItemToUpdate(changePacket->getSourceSlotId(), inventory->getItem(changePacket->getSourceSlotId()));
	slotPacket.addItemToUpdate(destinationSlot, inventory->getItem(destinationSlot));
	bool success = player->getPacketHandler()->sendDataToClient(slotPacket);

	auto sourceSlotUpdatePacket = std::shared_ptr<ChangeEquipmentResponsePacket>(new ChangeEquipmentResponsePacket(player, changePacket->getSourceSlotId()));
	auto destinationSlotUpdatePacket = std::shared_ptr<ChangeEquipmentResponsePacket>(new ChangeEquipmentResponsePacket(player, destinationSlot));
	success &= player->sendDataToVisibleEntities(sourceSlotUpdatePacket);
	success &= player->sendDataToVisibleEntities(destinationSlotUpdatePacket);

	return success;
}

bool PlayerPacketHandler::handleDropFromInventory(Player* player, const Packet* packet) {
	const DropItemFromInventoryRequestPacket* dropPacket = dynamic_cast<const DropItemFromInventoryRequestPacket*>(packet);
	const Item& item = player->getInventory()->getItem(dropPacket->getSlotId());
	if (!item.isValid()) {
		logger.logWarn("Invalid request for item: [Player: ", player->getName(), "][Slot: ", dropPacket->getSlotId(), "].");
		return false;
	}
	if (item.getAmount() < dropPacket->getAmount()) {
		logger.logDebug("Requested drop amount (", dropPacket->getAmount(), ") bigger than owned amount (", item.getAmount(), ").");
		return true;
	}

	Item itemToDrop(item);
	itemToDrop.setAmount(dropPacket->getAmount());

	bool success = player->getLocationData()->getMap()->addEntityToInsertionQueue(new Drop(itemToDrop, player->getLocationData()->getMapPosition()->getCurrentPosition()));
	success &= player->getInventory()->reduceAmountOfItemInSlot(dropPacket->getSlotId(), dropPacket->getAmount());

	if (item.getType() == ItemTypeList::MONEY) {
		UpdateMoneyResponsePacket moneyPacket(player->getInventory()->getMoneyAmount());
		success &= sendDataToClient(moneyPacket);
	}
	else {
		UpdateInventorySlotsResponsePacket slotPacket;
		slotPacket.addItemToUpdate(dropPacket->getSlotId(), item);
		success &= this->sendDataToClient(slotPacket);
	}
	return success;
}

bool PlayerPacketHandler::handlePickupDrop(Player* player, const Packet* packet) {
	const PickupDropRequestPacket* dropPacket = dynamic_cast<const PickupDropRequestPacket*>(packet);
	auto dropEntity = player->getVisualityProcessor()->findEntity(dropPacket->getLocalId());
	if (dropEntity == nullptr || !dropEntity->isDrop()) {
		logger.logError("Player '", player->getName(), "' tried to pickup invalid drop (#", dropPacket->getLocalId(), ").");
		return false;
	}
	float distance = PositionProcessor::getDistanceBetweenPoints(player->getLocationData()->getMapPosition()->getCurrentPosition(), 
		dropEntity->getLocationData()->getMapPosition()->getCurrentPosition());
	const float maxAllowedDistanceFromDrop = 500.0f;
	if (distance >= maxAllowedDistanceFromDrop) {
		logger.logError("Player '", player->getName(), "' tried to pickup drop (#", dropPacket->getLocalId(), ") from invalid distance: ", distance, ". Allowed is: ", maxAllowedDistanceFromDrop);
		return false;
	}
	player->getCombat()->clear();
	player->getLocationData()->getMapPosition()->setDestinationPositionToCurrentPosition();
	PickupDropResponsePacket response = player->onDropPickup(dynamic_cast<Drop*>(dropEntity));
	bool success = sendDataToClient(response);
	if (response.getDropMessageType() == PickupDropMessageType::OKAY) {
		player->getLocationData()->getMap()->addEntityToRemovalQueue(dropEntity, RemovalReason::DROP_PICKUP);
	}
	return success;
}

PlayerAttributeTypes::PlayerAttributeTypes() {
	strength = new PlayerAttributes("Strength", 15);
	dexterity = new PlayerAttributes("Dexterity", 15);
	intelligence = new PlayerAttributes("Intelligence", 15);
	concentration = new PlayerAttributes("Concentration", 15);
	charm = new PlayerAttributes("Charm", 10);
	sensibility = new PlayerAttributes("Sensibility", 10);
}

PlayerAttributeTypes::~PlayerAttributeTypes() {
	delete strength;
	strength = nullptr;

	delete dexterity;
	dexterity = nullptr;

	delete intelligence;
	intelligence = nullptr;

	delete concentration;
	concentration = nullptr;

	delete charm;
	charm = nullptr;

	delete sensibility;
	sensibility = nullptr;
}