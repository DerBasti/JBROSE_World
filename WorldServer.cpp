#include "WorldServer.h"
#include "WorldPackets\WorldServerPacketFactory.h"
#include "BasicTypes\Item.h"
#include "Entities/Monster.h"
#include "..\JBROSE_Common\DirectoryParser.h"
#include "FileTypes/IFO.h"
#include "WorldPackets/Responses/TelegateResponsePacket.h"

WorldServer::WorldServer(uint16_t port) : ROSEServer(port) {
	setPacketFactoryCreatorFunction([]() {
		return std::shared_ptr<PacketFactory>(new WorldServerPacketFactory());
	});
}

WorldServer::~WorldServer() {

}

void WorldServer::loadEncryption() {
	ENCRYPTION_TABLE = std::shared_ptr<CryptTable>(new CryptTable());
	ENCRYPTION_TABLE->generateCryptTables();
}


std::shared_ptr<Player> WorldServer::findWorldClientByInterface(std::shared_ptr<ROSEClient>& client) const {
	auto it = worldClientList.find(client);
	std::shared_ptr<Player> player;
	if (it != worldClientList.end()) {
		player = (*it).second;
	}
	return player;
}

bool WorldServer::onPacketsReady(std::shared_ptr<ROSEClient>& roseClient, std::queue<std::shared_ptr<Packet>>& packetQueue) {
	std::shared_ptr<Player> client = findWorldClientByInterface(roseClient);
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
	std::cout << "Starting up server...\n";
	loadFileEntries();
	loadNpcDefaultValues();
	createMaps();
	std::cout << "Startup finished!\n";
}

void WorldServer::loadFileEntries() {
	npcFile = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_NPC.STB"));
	zoneFile = std::shared_ptr<STBFile>(new STBFile("D:\\Games\\ROSE Server\\VFS_Extrator\\3DDATA\\STB\\LIST_ZONE.STB"));
}

void WorldServer::loadNpcDefaultValues() {
	auto allEntries = npcFile->getAllEntries();
	for (auto it = allEntries.cbegin(); it != allEntries.cend(); it++) {
		auto entry = it->second;
		npcDefaultStatValues.insert(std::make_pair(it->first, std::shared_ptr<NPCDefaultStatValues>(new NPCDefaultStatValues(it->first, entry))));
	}
}

void WorldServer::createMaps() {
	const std::string basicPath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	for (uint16_t i = 1; i < zoneFile->getEntryAmount(); i++) {
		maps[i] = new Map(i);
		std::string path = std::string(zoneFile->getEntry(i)->getColumnData(0x01).get());
		path = path.substr(0, path.find_last_of('\\'));
		DirectoryParser parser((basicPath + path).c_str());
		if (!path.empty() && parser.isDirectoryExistent()) {
			std::cout << "Starting up Map[" << i << "].\n";
			spawnMonsters(maps[i], parser);
			mapThreads[i] = std::thread([this](Map* map) {
				while (true) {
					map->updateEntities();
					Sleep(20);
				}
			}, maps[i]);
		}
	}
}

void WorldServer::spawnMonsters(Map* map, DirectoryParser& parser) {
	parser.setFileExtension(".IFO");
	auto files = parser.getFiles();
	for (auto& file : files) {
		IFOFile ifo(file);
		if (!ifo.isTypedEntryExistent(IFOFile::BlockType::MONSTERSPAWN)) {
			continue;
		}
		auto spawns = ifo.getTypedEntry(IFOFile::BlockType::MONSTERSPAWN);
		for (auto& entry : spawns) {
			IFOMonsterSpawnEntry* spawn = dynamic_cast<IFOMonsterSpawnEntry*>(entry.get());
			uint32_t amount = 0;
			uint32_t idx = 0;
			auto basicRounds = spawn->getBasicRounds();
			while(amount < spawn->getMaximumAmountOfMonsters()) {
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
}

void WorldServer::onNewROSEClient(std::shared_ptr<ROSEClient>& roseClient) {
	std::shared_ptr<Player> newPlayer = std::shared_ptr<Player>(new Player(roseClient));
	worldClientList.insert(std::make_pair(roseClient, newPlayer));
}

void WorldServer::onROSEClientDisconnecting(std::shared_ptr<ROSEClient>& roseClient) {
	auto client = findWorldClientByInterface(roseClient);
	if (!client) {
		std::cout << "Player konnte nicht gefunden werden!";
		return;
	}
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
	return true;
}

bool WorldServer::teleportPlayerFromTelegate(Player* player, const uint16_t telegateId) {
	//FIND TELEGATE. IF FOUND:

	Position pos(520000.0f, 520000.0f);
	uint16_t mapId = 21;

	return teleportPlayer(player, maps[mapId], pos);
}

bool WorldServer::teleportPlayer(Player* player, Map* map, const Position& pos) {
	auto locationData = player->getLocationData();

	TelegateResponsePacket response;
	response.setLocalEntityId(locationData->getLocalId());
	response.setMapId(map->getId());
	response.setPosition(pos);

	locationData->getMap()->addEntityToRemovalQueue(player);
	locationData->setMap(map);
	locationData->setCurrentMapSector(nullptr);
	locationData->getPositionCollection()->setCurrentPosition(pos);
	locationData->getPositionCollection()->setDestinationPosition(pos);

	return player->sendDataToSelf(response);
}