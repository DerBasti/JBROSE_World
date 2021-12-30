#include "WorldServer.h"
#include "WorldPackets\WorldServerPacketFactory.h"
#include "BasicTypes\Item.h"
#include "Entities/Drop.h"
#include "Entities/Monster.h"
#include "..\JBROSE_Common\DirectoryParser.h"
#include "FileTypes/AIP.h"
#include "FileTypes/IFO.h"
#include "FileTypes/ZON.h"
#include "Map/MapRemovalRequest.h"
#include "WorldPackets/Responses/TelegateResponsePacket.h"
#include "BasicTypes/RegenerationProcessor.h"
#include <future>
#include <thread>
#include <chrono>
#include <iostream>

WorldServer::WorldServer(uint16_t port) : ROSEServer(port) {
	setPacketFactoryCreatorFunction([]() {
		return std::shared_ptr<PacketFactory>(new WorldServerPacketFactory());
	});
}

WorldServer::~WorldServer() {
	NPCCreationFactory::deleteAllEntries();

	std::for_each(telegates.begin(), telegates.end(), [](std::pair<uint16_t, Telegate*> gatePair) {
		delete gatePair.second;
		gatePair.second = nullptr;
	});
	std::for_each(zoneFiles.begin(), zoneFiles.end(), [](std::pair<uint32_t, ZONFile*> zonPair) {
		delete zonPair.second;
		zonPair.second = nullptr;
	});
	std::for_each(attackAnimations.begin(), attackAnimations.end(), [](std::pair<uint32_t, ZMO*> zmoPair) {
		delete zmoPair.second;
		zmoPair.second = nullptr;
	});

	if (npcAnimationFile != nullptr) {
		delete npcAnimationFile;
		npcAnimationFile = nullptr;
	}

}

void WorldServer::loadEncryption() {
	ENCRYPTION_TABLE = std::shared_ptr<CryptTable>(new CryptTable());
	ENCRYPTION_TABLE->generateCryptTables();
}


Player* WorldServer::findWorldClientByInterface(std::shared_ptr<ROSEClient>& client) const {
	auto it = worldClientList.find(client);
	Player* player = nullptr;
	if (it != worldClientList.end()) {
		player = (*it).second;
	}
	return player;
}

bool WorldServer::onPacketsReady(std::shared_ptr<ROSEClient>& roseClient, std::queue<std::shared_ptr<Packet>>& packetQueue) {
	Player* client = findWorldClientByInterface(roseClient);
	bool success = client != nullptr;
	if (success) {
		while (!packetQueue.empty()) {
			std::shared_ptr<Packet> p = packetQueue.front();
			success &= client->handlePacket(p.get());
			packetQueue.pop();
		}
	}
	return success;
}

void WorldServer::onServerStartup() {
	Timer timer;
	logger.logInfo("Starting up server...");
	loadFileEntries();
	loadAttackAnimationTimings();
	loadAi();
	loadNpcDefaultValues();
	createMaps();
	logger.logInfo("Startup finished!");		
	logger.logInfo("Startup took: ", timer.getPassedTimeInMillis(), "ms");
}

void WorldServer::loadFileEntries() {
	clock_t timeStart = clock();
	logger.logInfo("Loading STBs...");
	
	npcSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_NPC.STB"));
	aiSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\FILE_AI.STB"));
	warpSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\WARP.STB"));
	zoneSTB = std::shared_ptr<ZoneSTBFile>(new ZoneSTBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_ZONE.STB"));
		
	equipmentSTBs = std::shared_ptr<STBFile*>(new STBFile*[15], [](STBFile** ptr) {
		for (uint16_t i = 0; i < 15; i++) {
			delete ptr[i];
			ptr[i] = nullptr;
		}
		delete[] ptr;
		ptr = nullptr;
	});
	
	const uint16_t stbThreadSize = 15;
	std::thread stbThreads[stbThreadSize];

	stbThreads[0] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::ARMOR.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_BODY.STB");
	});
	stbThreads[1] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::BACK.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_BACK.STB");
	});
	stbThreads[2] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::CONSUMABLE.getTypeId()] = new ConsumeSTBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_USEITEM.STB");
	});
	stbThreads[3] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::FACE.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_FACEITEM.STB");
	});
	stbThreads[4] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::GLOVES.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_ARMS.STB");
	});
	stbThreads[5] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::HEADGEAR.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_CAP.STB");
	});
	stbThreads[6] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::JEWELS.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_JEMITEM.STB");
	});
	stbThreads[7] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::JEWELRY.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_JEWEL.STB");
	});
	stbThreads[8] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::OTHER.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_NATURAL.STB");
	});
	stbThreads[9] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::PAT.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_PAT.STB");
	});
	stbThreads[10] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::QUEST.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_QUESTITEM.STB");
	});
	stbThreads[11] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::SHIELD.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_SUBWPN.STB");
	});
	stbThreads[12] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::SHOES.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_FOOT.STB");
	});
	stbThreads[13] = std::thread([this]() {
		equipmentSTBs.get()[ItemTypeList::WEAPON.getTypeId()] = new EquipmentSTB("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_WEAPON.STB");
	});
	stbThreads[14] = std::thread([this]() {
		dropSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\ITEM_DROP.STB"));
	});
	for (uint16_t i = 0; i < stbThreadSize; i++) {
		stbThreads[i].join();
	}
	ConsumableItemList::loadListFromStb(getConsumeSTB());
	
	clock_t duration = clock() - timeStart;
	logger.logInfo("Finished loading STBs including translations in ",duration,"ms!");
	logger.logInfo("Loading Zone-Files...");
	loadZONFiles();
	logger.logInfo("Finished loading Zone-Files...");
}

void WorldServer::loadAi() {
	/*logger.logInfo("Loading AI...");
	for (uint32_t i = 0; i < aiSTB->getEntryAmount(); i++) {
		auto stbEntry = aiSTB->getEntry(i);
		std::string aiFileName = std::string("D:\\Games\\ROSE Server\\VFS_Extrator\\");
		aiFileName = aiFileName.append(std::string(stbEntry->getColumnData(0x00)));
		if (DirectoryParser::isFileExistent(aiFileName.c_str())) {
			AIP* aiProtocol = new AIP(aiFileName.c_str());
			aiProtocols.insert(std::make_pair(i, aiProtocol));
		}
	}
	logger.logInfo("Finished loading AI.");*/
}

void WorldServer::loadZONFiles() {
	std::string basicPath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	const uint32_t pathColumnId = 1;
	for (uint32_t i = 1; i < zoneSTB->getEntryAmount(); i++) {
		auto row = zoneSTB->getEntry(i);
		auto path = row->getColumnData(pathColumnId);
		std::string realPath = basicPath + std::string(path);
		std::pair<uint32_t,ZONFile*> pair = std::make_pair(i, nullptr);
		if (realPath.length() == basicPath.length() || !DirectoryParser::isFileExistent(realPath.c_str())) {
			zoneFiles.insert(std::move(pair));
			continue;
		}
		pair.second = new ZONFile(realPath.c_str());
		auto rawRestorePoints = pair.second->getAllRestorePoints();
		for (auto rawRestorePoint : rawRestorePoints) {
			restorePoints.push_back(new RestorePoint(static_cast<uint32_t>(restorePoints.size()), i, rawRestorePoint));
		}
		zoneFiles.insert(std::move(pair));
	}
}

void WorldServer::loadAttackAnimationTimings() {
	auto motionSTB = new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\FILE_MOTION.STB");
	const char* basePath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	for (uint32_t i = 0; i < motionSTB->getEntryAmount(); i++) {
		std::string zmoPath = std::string(basePath) + motionSTB->getEntry(i)->getColumnData(0);
		ZMO* zmo = new ZMO(zmoPath.c_str());
		attackAnimations.insert(std::make_pair(i, zmo));
	}
	delete motionSTB;

	motionTypesSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\TYPE_MOTION.STB"));
	npcAnimationFile = new CHRFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\NPC\\LIST_NPC.CHR");
}

void WorldServer::loadNpcDefaultValues() {
	NPCCreationFactory::initializeFromSTB(npcSTB.get(), aiSTB.get());
}

void WorldServer::createMaps() {
	logger.logInfo("Amount of zones: ", zoneSTB->getEntryAmount());
	const std::string basicPath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	for (uint16_t i = 1; i < zoneSTB->getEntryAmount(); i++) {
		mapThreads[i] = std::thread([this, basicPath](int mapId) {
			std::string path = std::string(zoneSTB->getEntry(mapId)->getColumnData(0x01));
			path = path.substr(0, path.find_last_of('\\'));
			WindowsDirectoryParser parser((basicPath + path).c_str());
			Map* currentMap = nullptr;
			if (!path.empty() && parser.isPathExistent()) {
				currentMap = new Map(mapId, zoneSTB->getEntry(mapId)->getColumnData(0), this->zoneFiles.at(mapId), zoneSTB.get());
				maps[mapId] = currentMap;
				logger.logInfo("Starting up Map[", currentMap->getName(), "].");
				currentMap->addRestorePoints(restorePoints);
				loadIFOData(currentMap, parser);
				while (true) {
					currentMap->updateEntities();
					Sleep(16);
				}
				logger.logInfo("Finished start-up of Map[",zoneSTB->getEntry(mapId)->getColumnData(0), "].");
			}
		}, i);
	}
}

void WorldServer::loadIFOData(Map* map, DirectoryParser& parser) {
	parser.setFileExtensionToSearchFor(".IFO");
	parser.loadFileList();
	auto files = parser.getFileList();
	for (auto& file : files) {
		IFOFile ifo(file);
		loadNPCs(map, ifo);
		loadMonsters(map, ifo);
		loadTelegates(map, ifo);
	}
}

void WorldServer::loadNPCs(Map* map, const IFOFile& file) {
	if (!file.isTypedEntryExistent(IFOFile::BlockType::NPCLOCATION)) {
		return;
	}
	auto npcs = file.getTypedEntry(IFOFile::BlockType::NPCLOCATION);
	for (auto& npcEntry : npcs) {
		const Position& pos = npcEntry->getPosition();
		Entity* npc = NPCCreationFactory::createNpc(npcEntry->getObjectId(), pos);
		npc->getLocationData()->getMapPosition()->setDirection(npcEntry->getDirection());
		map->addEntityToInsertionQueue(npc);
	}
}

void WorldServer::loadMonsters(Map* map, const IFOFile& file) {
	if (!file.isTypedEntryExistent(IFOFile::BlockType::MONSTERSPAWN)) {
		return;
	}
	auto spawns = file.getTypedEntry(IFOFile::BlockType::MONSTERSPAWN);
	for (auto& entry : spawns) {
		std::shared_ptr<IFOMonsterSpawnEntry>& spawn = (std::shared_ptr<IFOMonsterSpawnEntry>&)(entry);
		map->addMonsterSpawn(spawn);
	}
	map->checkForMonsterRespawns();
}

void WorldServer::loadTelegates(Map* map, const IFOFile& file) {
	Timer timer;
	ZONFile* zoneFile = zoneFiles.at(map->getId());
	if (!zoneFile) {
		return;
	}
	if (file.isTypedEntryExistent(IFOFile::BlockType::TELEGATE)) {
		auto telegatesFromIfo = file.getTypedEntry(IFOFile::BlockType::TELEGATE);
		extractTelegateFromIfo(telegatesFromIfo, zoneFile);
	}
}

void WorldServer::extractTelegateFromIfo(std::vector<std::shared_ptr<IFOEntry>> &telegatesFromIfo, ZONFile* zonFile)
{
	for (auto& ifoTelegate : telegatesFromIfo) {
		const STBEntry* warpDestinationEntry = warpSTB->getEntry(ifoTelegate->getWarpSTBId());
		uint32_t mapId = warpDestinationEntry->getColumnDataAsInt(1);
		auto gateName = warpDestinationEntry->getColumnData(2);
		auto zonFile = zoneFiles.at(mapId);
		Telegate* telegate = nullptr;
		EventZoneData *telegateZoneData = zonFile->getEventByName(gateName);
		if (!telegateZoneData) {
			logger.logError("Gate [", gateName, "] NOT found! Setting to default gate for map ", zoneSTB->getEntry(mapId)->getColumnData(0));
			telegateZoneData = zonFile->getEventByName(EventZoneData::DEFAULT_EVENT);
		}
		telegate = new Telegate(telegateZoneData, mapId);
		this->telegates.insert(std::move(std::make_pair(ifoTelegate->getWarpSTBId(), telegate)));
	}
}

void WorldServer::onNewROSEClient(std::shared_ptr<ROSEClient>& roseClient) {
	Player *newPlayer = new Player(roseClient);
	worldClientList.insert(std::move(std::make_pair(roseClient, newPlayer)));
}

void WorldServer::onROSEClientDisconnecting(std::shared_ptr<ROSEClient>& roseClient) {
	auto client = findWorldClientByInterface(roseClient);
	if (!client) {
		logger.logWarn("Player konnte nicht gefunden werden!");
		return;
	}
	saveCharacter(client);
	client->onDisconnect();
	worldClientList.erase(roseClient);
}

bool WorldServer::saveCharacter(Player* player) {
	bool success = saveCharacterDataForCharacter(player);
	success &= saveSkilledAttributesForCharacter(player);
	success &= saveInventoryForCharacter(player);
	return success;
}

uint32_t WorldServer::getLastLoggedCharacterFromAccount(const uint32_t accountId) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT last_character_online FROM accounts WHERE id=%i", accountId);
	auto resultSet = database->selectQuery(buf);
	uint32_t resultId = -1;
	if (resultSet->hasResult()) {
		resultId = resultSet->getRow(0)->getColumnDataAsInt(0);
	}
	else {
		logger.logError("[DB-ERROR]: ", resultSet->getError());
	}
	return resultId;
}

bool WorldServer::loadCharacterDataForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT player_name, level, class_id, sex, face, hair FROM players WHERE id=%i", player->getTraits()->getCharacterId());
	auto result = database->selectQuery(buf);
	if (result->hasResult()) {
		ResultRow *row = result->getRow(0);
		auto playerTraits = player->getTraits();
		auto playerStats = player->getStats();
		playerTraits->setName(row->getColumnData(0));
		playerStats->setLevel(row->getColumnDataAsInt(1));
		playerTraits->setJob(Job::getJobFromId(row->getColumnDataAsInt(2)));
		playerTraits->setSex(row->getColumnDataAsInt(3));
		playerTraits->setFaceStyle(row->getColumnDataAsInt(4));
		playerTraits->setHairStyle(row->getColumnDataAsInt(5));

		player->getLocationData()->setMap(maps[22]);
		Position position = player->getLocationData()->getMap()->getDefaultRespawnPoint();
		player->getLocationData()->getMapPosition()->setCurrentPosition(position);
		player->getLocationData()->getMapPosition()->setDestinationPosition(position);

	}
	else {
		logger.logError("[DB-ERROR]: ", result->getError());
	}
	return result->hasResult();
}

bool WorldServer::loadSkilledAttributesForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT strength, dexterity, intelligence, concentration, charm, sensibility FROM player_attributes WHERE player_id=%i", player->getTraits()->getCharacterId());
	auto result = database->selectQuery(buf);
	if (result->hasResult()) {
		ResultRow *row = result->getRow(0);
		PlayerAttributeTypes* attributes = player->getAttributes();
		attributes->getStrength()->setPointsLearned(static_cast<uint16_t>(row->getColumnDataAsInt(0)));
		attributes->getDexterity()->setPointsLearned(static_cast<uint16_t>(row->getColumnDataAsInt(1)));
		attributes->getIntelligence()->setPointsLearned(static_cast<uint16_t>(row->getColumnDataAsInt(2)));
		attributes->getConcentration()->setPointsLearned(static_cast<uint16_t>(row->getColumnDataAsInt(3)));
		attributes->getCharm()->setPointsLearned(static_cast<uint16_t>(row->getColumnDataAsInt(4)));
		attributes->getSensibility()->setPointsLearned(static_cast<uint16_t>(row->getColumnDataAsInt(5)));
	}
	else {
		logger.logError("[DB-ERROR]: ", result->getError());
	}
	return result->hasResult();
}

bool WorldServer::loadInventoryForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT item_type, item_id, amount, slot FROM items WHERE player_id=%i", player->getTraits()->getCharacterId());
	auto result = database->selectQuery(buf);
	if (result->hasResult()) {
		auto inventory = player->getInventory();
		for (uint32_t i = 0; i < result->getResultAmount(); i++) {
			auto currentRow = result->getRow(i);
			const ItemType& itemType = ItemTypeList::toItemType(currentRow->getColumnDataAsInt(0));
			const uint32_t itemId = currentRow->getColumnDataAsInt(1);
			const uint32_t itemAmount = currentRow->getColumnDataAsInt(2);
			Item itemToAssign;
			if (itemType != ItemTypeList::MONEY) {
				itemToAssign = Item(itemType, itemId, itemAmount);
				inventory->setItem(currentRow->getColumnDataAsInt(3), itemToAssign);
			}
			else {
				inventory->setMoney(itemAmount);
			}
		}
	}
	else {
		logger.logError("[DB-ERROR]: ", result->getError());
	}
	return true;
}

bool WorldServer::saveCharacterDataForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	auto playerStats = player->getStats();
	sprintf_s(buf, "UPDATE players SET level=%i, class_id=%i, sex=%i, face=%i, hair=%i, save_spot_id=%i WHERE id=%i", playerStats->getLevel(), player->getTraits()->getJob().getId(),
		player->getTraits()->getSex(), player->getTraits()->getFaceStyle(), player->getTraits()->getHairStyle(), player->getTraits()->getSavedSpotId(), player->getTraits()->getCharacterId());
	logger.logDebug("Saving player's basic data...");;
	auto result = database->updateQuery(buf);
	if (!result) {
		logger.logWarn("Saving player '", player->getTraits()->getName(), "'[", player->getTraits()->getCharacterId(), "] could not be saved.\nDB-Error: ", database->getLastError());
	}
	return result;
}

bool WorldServer::saveSkilledAttributesForCharacter(Player* player) {
	auto playerAttributes = player->getAttributes();
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "UPDATE player_attributes SET strength=%i, dexterity=%i, intelligence=%i, concentration=%i, charm=%i, sensibility=%i WHERE player_id=%i",
		playerAttributes->getStrength()->getPointsLearned(), playerAttributes->getDexterity()->getPointsLearned(),
		playerAttributes->getIntelligence()->getPointsLearned(), playerAttributes->getConcentration()->getPointsLearned(),
		playerAttributes->getCharm()->getPointsLearned(), playerAttributes->getSensibility()->getPointsLearned(),
		player->getTraits()->getCharacterId());
	logger.logDebug("Saving player's attribute data...");
	auto result = database->updateQuery(buf);
	if (!result) {
		logger.logWarn("Saving player '", player->getTraits()->getName(), "'[", player->getTraits()->getCharacterId(), "] could not be saved.\nDB-Error: ", database->getLastError());
	}
	return true;
}

bool WorldServer::saveInventoryForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "DELETE FROM items WHERE player_id=%i", player->getTraits()->getCharacterId());
	bool success = database->deleteQuery(buf);
	logger.logDebug("Pruning character inventory...");
	if (success) {
		logger.logDebug("Saving player's inventory...");
		for (uint8_t i = 0; i < Inventory::MAX_SLOTS;i++) {
			success &= saveItemForPlayerInventory(player, i);
		}
		success &= saveMoneyForCharacter(player);
	}
	else {
		logger.logError("[DB-ERROR]: ", database->getLastError());
	}
	return success;
}

bool WorldServer::saveItemForPlayerInventory(Player* player, const uint8_t slotId) {
	char buf[0x200] = { 0x00 };
	bool currentInsertSuccess = false;
	const Item& item = player->getInventory()->getItem(slotId);
	if (item.isValid()) {
		sprintf_s(buf, "INSERT INTO items (player_id, item_type, item_id, amount, slot, durability, stat_id, refinement, appraised, socketed, lifespan) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
			player->getTraits()->getCharacterId(), item.getType().getTypeId(), item.getId(), item.getAmount(), slotId, item.getDurability(),
			item.getStatId(), item.getRefineLevel(), item.isAppraised(), item.isSocketed(), item.getLifespan());
		bool currentInsertSuccess = database->insertQuery(buf);
		if (!currentInsertSuccess) {
			logger.logWarn("[DB-ERROR]: Couldn't save item ", item, " in slot ", slotId, " for player '", player->getTraits()->getName(), "'.");
		}
	}
	return currentInsertSuccess;
}

bool WorldServer::saveMoneyForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "INSERT INTO items (player_id, item_type, item_id, amount, slot, durability, stat_id, refinement, appraised, socketed, lifespan) VALUES(%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i)",
		player->getTraits()->getCharacterId(), ItemTypeList::MONEY.getTypeId(), 0, player->getInventory()->getMoneyAmount(), 0, 35,
		0, 0, true, false, 1000);
	return database->insertQuery(buf);
}

bool WorldServer::teleportPlayerFromTelegate(Player* player, const uint16_t telegateId) {
	Position pos(520000.0f, 520000.0f);
	uint16_t mapId = 22;
	if (telegates.find(telegateId) != telegates.cend()) {
		auto telegate = telegates.at(telegateId);
		pos = telegate->getPosition();
		mapId = telegate->getMapId();
	}
	else {
		logger.logWarn("Couldn't find telegate with id: ", telegateId, ". Player position: [", player->getLocationData(), "].");
	}
	return teleportPlayer(player, maps[mapId], pos);
}

bool WorldServer::teleportPlayer(Player* player, Map* map, const Position& pos) {
	auto locationData = player->getLocationData();

	TelegateResponsePacket response;
	response.setLocalEntityId(locationData->getLocalId());
	response.setMapId(map->getId());
	response.setPosition(pos);

	locationData->getMap()->addEntityToRemovalQueue(player, RemovalReason::TELEPORT);
	locationData->setMap(map);
	locationData->setCurrentMapSector(nullptr);
	locationData->getMapPosition()->setCurrentPosition(pos);
	locationData->getMapPosition()->setDestinationPosition(pos);

	logger.logDebug("Spawning player to: ", locationData);

	return player->getPacketHandler()->sendDataToClient(response);
}

ZMO* WorldServer::getAttackAnimationForPlayer(Player* player) const {
	ZMO* animation = nullptr;
	switch (player->getCombat()->getCombatType()) {
		case CombatTypeId::NONE:
		case CombatTypeId::BASIC_ATTACK:
		{
			const Item& weapon = player->getInventory()->getItem(ItemTypeList::WEAPON.getInventorySlotId());
			animation = getAttackAnimationByWeaponId(weapon.getId());
		}
		break;
		case CombatTypeId::SKILL_ATTACK:

		break;
	}
	return animation;
}

ZMO* WorldServer::getAttackAnimationByWeaponId(const uint32_t weaponId) const {
	uint32_t motionTypeColumn = getEquipmentSTB(ItemTypeList::WEAPON.getTypeId())->getMotionIdOfEntry(weaponId);
	uint32_t animationId = motionTypesSTB->getEntry(ItemTypeList::WEAPON.getTypeId())->getColumnDataAsInt(motionTypeColumn);
	logger.logDebug("Found animation of id #", animationId, " for weapon id #", weaponId);

	auto iterator = attackAnimations.find(animationId);
	if (iterator != attackAnimations.end()) {
		return iterator->second;
	}
	logger.logWarn("No corrosponding animation for weapon id #", weaponId, " found!");
	return nullptr;
}
ZMO* WorldServer::getAttackAnimationBySkillId(const uint32_t id) const {
	return nullptr;
}

ZMO* WorldServer::getAttackAnimationForNpc(NPC* npc) const {
	ZMO* animation = nullptr;
	switch (npc->getCombat()->getCombatType()) {
		case CombatTypeId::NONE:
		case CombatTypeId::BASIC_ATTACK:
			animation = npcAnimationFile->getAttackAnimationForNpcId(npc->getDefaultStatValues()->getId());
		break;
	}
	return animation;
}

bool WorldServer::addDropFromNPC(NPC* monster, int16_t levelDifferenceToKiller) {
	Drop* drop = nullptr;
	uint16_t levelDiffbalanced = (std::min)((std::max)(static_cast<int16_t>(0), levelDifferenceToKiller), static_cast<int16_t>(10));
	NumericRandomizer<uint16_t> randomizer(0, 100);
	Position pos = PositionProcessor::generateRandomPointAroundPosition(monster->getLocationData()->getMapPosition()->getCurrentPosition(), 200.0f);
	uint16_t drawnValue = randomizer.generateRandomValue();
	logger.logDebug("Chance to drop money: ", monster->getDefaultStatValues()->getDefaultMoneyDropChance(), "%. Drawn random value: ", drawnValue);
	if (drawnValue <= monster->getDefaultStatValues()->getDefaultMoneyDropChance()) {
		randomizer.setNewBoundries(monster->getDefaultStatValues()->getLevel() + 20, (monster->getDefaultStatValues()->getLevel() + 30) * 2);
		Item item = MoneyItem(randomizer.generateRandomValue());
		drop = new Drop(item, pos);
	}
	else {
		drawnValue = randomizer.generateRandomValue();
		uint16_t dropRowId = monster->getLocationData()->getMap()->getId();
		if (drawnValue <= monster->getDefaultStatValues()->getDefaultOwnDropChance()) {
			dropRowId = monster->getDefaultStatValues()->getDropTableRowId();
		}
		randomizer.setNewBoundries(0, 30);
		uint16_t columnId = randomizer.generateRandomValue();
		Item dropItem = generateDrop(dropRowId, columnId);
		if (dropItem.isValid()) {
			drop = new Drop(dropItem, pos);
		}
	}

	bool success = drop != nullptr;
	if (success) {
		logger.logDebug("Dropping: ", drop->getName());
		success &= monster->getLocationData()->getMap()->addEntityToInsertionQueue(drop);
	}
	return success;
}

Item WorldServer::generateDrop(const uint16_t dropRowId, const uint16_t column) const {
	auto dropRowEntry = dropSTB->getEntry(dropRowId);
	uint32_t itemId = dropRowEntry->getColumnDataAsInt(column);
	NumericRandomizer<uint8_t> randomizer(0, 5);
	if (itemId <= 1000) {
		if (itemId > 0 && itemId < 5) {
			randomizer.setNewBoundries(5, itemId * 10);
			uint16_t offset = 26 + randomizer.generateRandomValue();
			if (offset >= dropRowEntry->getColumnAmount()) {
				return Item();
			}
			itemId = dropRowEntry->getColumnDataAsInt(offset);
		} else {
			return Item();
		}
	}
	if (itemId <= 1000) {
		return Item();
	}
	Item dropItem(ItemTypeList::toItemType(itemId / 1000), itemId % 1000);
	if (dropItem.isStackable()) {
		dropItem.setAmount(randomizer.generateRandomValue() + 1);
	}
	else {
		EquipmentSTB *stbFile = (EquipmentSTB*)equipmentSTBs.get()[dropItem.getType().getTypeId()];
		randomizer.setNewBoundries(35, stbFile->getQualityOfEntry(dropItem.getId()) + 30);
		dropItem.setDurability(randomizer.generateRandomValue());
	}
	return dropItem;
}