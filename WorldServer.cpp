#include "WorldServer.h"
#include "WorldPackets\WorldServerPacketFactory.h"
#include "BasicTypes\Item.h"
#include "Entities/Monster.h"
#include "..\JBROSE_Common\DirectoryParser.h"
#include "FileTypes/IFO.h"
#include "FileTypes/ZON.h"
#include "Map/MapRemovalRequest.h"
#include "WorldPackets/Responses/TelegateResponsePacket.h"

WorldServer::WorldServer(uint16_t port) : ROSEServer(port) {
	setPacketFactoryCreatorFunction([]() {
		return std::shared_ptr<PacketFactory>(new WorldServerPacketFactory());
	});
}

WorldServer::~WorldServer() {
	std::for_each(telegates.begin(), telegates.end(), [](std::pair<uint16_t, Telegate*> gatePair) {
		delete gatePair.second;
		gatePair.second = nullptr;
	});
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
	std::cout << "Starting up server...\n";
	loadFileEntries();
	loadNpcDefaultValues();
	createMaps();
	std::cout << "Startup finished!\n";
	std::cout << "Startup took: " << timer.getPassedTimeInMillis() << "ms\n";
}

void WorldServer::loadFileEntries() {
	std::cout << "Loading STBs...\n";
	npcSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_NPC.STB"));
	warpSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\WARP.STB"));
	zoneSTB = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_ZONE.STB"));
	std::cout << "Finished loading STBs including translations!\n";
	std::cout << "Loading Zone-Files...\n";
	loadZONFiles();
	std::cout << "Finished loading Zone-Files...\n";
}

void WorldServer::loadZONFiles() {
	std::string basicPath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	for (uint32_t i = 1; i < zoneSTB->getEntryAmount(); i++) {
		auto row = zoneSTB->getEntry(i);
		auto path = row->getColumnData(1);
		std::string realPath = basicPath + std::string(path.get());
		if (realPath.length() == basicPath.length() || !DirectoryParser::isFileExistent(realPath.c_str())) {
			zoneFiles.insert(std::make_pair(i, nullptr));
			continue;
		}
		zoneFiles.insert(std::make_pair(i, new ZONFile(realPath.c_str())));
	}
}

void WorldServer::loadNpcDefaultValues() {
	auto allEntries = npcSTB->getAllEntries();
	for (auto it = allEntries.cbegin(); it != allEntries.cend(); it++) {
		auto entry = it->second;
		npcDefaultStatValues.insert(std::make_pair(it->first, new NPCDefaultStatValues(it->first, entry)));
	}
}

void WorldServer::createMaps() {
	const std::string basicPath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	std::cout << "Amount of zones: " << zoneSTB->getEntryAmount() << "\n";
	for (uint16_t i = 1; i < zoneSTB->getEntryAmount(); i++) {
		std::string path = std::string(zoneSTB->getEntry(i)->getColumnData(0x01).get());
		path = path.substr(0, path.find_last_of('\\'));
		DirectoryParser parser((basicPath + path).c_str());
		Map* currentMap = nullptr;
		if (!path.empty() && parser.isDirectoryExistent()) {
			currentMap = new Map(i);
			std::cout << "Starting up Map[" << zoneSTB->getEntry(i)->getColumnData(0) << "].\n";
			loadIFOData(currentMap, parser);
			mapThreads[i] = std::thread([this](Map* map) {
				while (true) {
					map->updateEntities();
					Sleep(20);
				}
			}, currentMap);
			std::cout << "Finished start-up of Map[" << zoneSTB->getEntry(i)->getColumnData(0) << "].\n";
		}
		maps[i] = currentMap;
	}
}

void WorldServer::loadIFOData(Map* map, DirectoryParser& parser) {
	parser.setFileExtension(".IFO");
	auto files = parser.getFiles();
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
		auto npcDefaultValues = getNPCDefaultValue(npcEntry->getObjectId());
		Entity* npc = new NPC(npcDefaultValues, pos);
		npc->getLocationData()->getPositionCollection()->setDirection(npcEntry->getDirection());
		map->addEntityToInsertionQueue(npc);
	}
}

void WorldServer::loadMonsters(Map* map, const IFOFile& file) {
	if (!file.isTypedEntryExistent(IFOFile::BlockType::MONSTERSPAWN)) {
		return;
	}
	auto spawns = file.getTypedEntry(IFOFile::BlockType::MONSTERSPAWN);
	for (auto& entry : spawns) {
		IFOMonsterSpawnEntry* spawn = dynamic_cast<IFOMonsterSpawnEntry*>(entry.get());
		uint32_t amount = 0;
		uint32_t idx = 0;
		auto basicRounds = spawn->getBasicRounds();
		while (amount < spawn->getMaximumAmountOfMonsters()) {
			auto round = basicRounds.at(idx);
			for (uint16_t x = 0; x < round->getMonsterAmount(); x++) {
				Position pos = spawn->getPosition();
				pos.setX(pos.getX() + (rand() / static_cast<float>(RAND_MAX)) * spawn->getMaximumRadius());
				pos.setY(pos.getY() + (rand() / static_cast<float>(RAND_MAX)) * spawn->getMaximumRadius());
				map->addEntityToInsertionQueue(new Monster(npcDefaultStatValues.at(round->getMonsterId()), pos));
				amount++;
			}
			idx++;
			if (idx >= basicRounds.size()) {
				idx = 0;
			}
		}
	}
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
		EventZoneData *telegateZoneData = zonFile->getEventByName(gateName.get());
		if (!telegateZoneData) {
			std::cout << "Gate [" << gateName << "] NOT found! Setting to default gate for map " << zoneSTB->getEntry(mapId)->getColumnData(0) << "\n";
			telegateZoneData = zonFile->getEventByName(EventZoneData::DEFAULT_EVENT);
		}
		telegate = new Telegate(telegateZoneData, mapId);
		this->telegates.insert(std::make_pair(ifoTelegate->getWarpSTBId(), telegate));
	}
}

void WorldServer::onNewROSEClient(std::shared_ptr<ROSEClient>& roseClient) {
	Player *newPlayer = new Player(roseClient);
	worldClientList.insert(std::make_pair(roseClient, newPlayer));
}

void WorldServer::onROSEClientDisconnecting(std::shared_ptr<ROSEClient>& roseClient) {
	auto client = findWorldClientByInterface(roseClient);
	if (!client) {
		std::cout << "Player konnte nicht gefunden werden!";
		return;
	}
	client->onDisconnect();
	worldClientList.erase(roseClient);
}

uint32_t WorldServer::getLastLoggedCharacterFromAccount(const uint32_t accountId) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT last_character_online FROM accounts WHERE id=%i", accountId);
	auto resultSet = database->selectQuery(buf);
	uint32_t resultId = -1;
	if (resultSet) {
		resultId = atoi(resultSet->getRow(0)->getColumnData(0).c_str());
	}
	return resultId;
}

bool WorldServer::loadCharacterDataForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT player_name, level, class_id, sex, face, hair FROM players WHERE id=%i", player->getTraits()->getCharacterId());
	auto result = database->selectQuery(buf);
	if (result) {
		ResultRow *row = result->getRow(0);
		auto playerTraits = player->getTraits();
		auto playerStats = player->getStats();
		playerTraits->setName(row->getColumnData(0));
		playerStats->setLevel(row->getColumnDataAsInt(1));
		playerStats->setJobId(row->getColumnDataAsInt(2));
		playerTraits->setSex(row->getColumnDataAsInt(3));
		playerTraits->setFaceStyle(row->getColumnDataAsInt(4));
		playerTraits->setHairStyle(row->getColumnDataAsInt(5));

		player->getLocationData()->setMap(maps[22]);
	}
	return result.operator bool();
}

bool WorldServer::loadInventoryForCharacter(Player* player) {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "SELECT * FROM items WHERE player_id=%i", player->getTraits()->getCharacterId());
	auto result = database->selectQuery(buf);
	if (result) {
		auto inventory = player->getInventory();
		for (uint32_t i = 0; i < result->getResultAmount(); i++) {
			auto currentRow = result->getRow(i);
			Item item(ItemTypeList::toItemType(currentRow->getColumnDataAsInt(2)), currentRow->getColumnDataAsInt(3), currentRow->getColumnDataAsInt(4));
			inventory->setItem(currentRow->getColumnDataAsInt(5), item);
		}
	}
	return result.operator bool();
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
		std::cout << "Couldn't find telegate with id: " << telegateId << ". Player position: [" << player->getLocationData() << "]\n";
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
	locationData->getPositionCollection()->setCurrentPosition(pos);
	locationData->getPositionCollection()->setDestinationPosition(pos);

	std::cout << "Spawning player to: " << locationData << "\n";

	return player->sendDataToSelf(response);
}