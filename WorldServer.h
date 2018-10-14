#ifndef __ROSE_WORLDSERVER__
#define __ROSE_WORLDSERVER__

#pragma once

#include "..\JBROSE_Common\ROSEServer.h"
#include "Entities/NPC.h"
#include "WorldClient.h"
#include "Map\Map.h"
#include "FileTypes/IFO.h"
#include "FileTypes/STB.h"
#include "..\JBROSE_Common\DirectoryParser.h"
#include "Map/Telegate.h"

class WorldServer : public ROSEServer {
private:
	std::unordered_map<std::shared_ptr<ROSEClient>, Player*> worldClientList;
	std::unordered_map<uint32_t, NPCDefaultStatValues*> npcDefaultStatValues;
	std::unordered_map<uint16_t, Telegate*> telegates;
	std::map<uint32_t, ZONFile*> zoneFiles;
	std::thread mapThreads[120];
	Map* maps[120];
	
	std::shared_ptr<STBFile> npcSTB;
	std::shared_ptr<STBFile> warpSTB;
	std::shared_ptr<STBFile> zoneSTB;

	Player* findWorldClientByInterface(std::shared_ptr<ROSEClient>& client) const;

	void loadFileEntries();
	void loadNpcDefaultValues();
	void createMaps();
	void loadZONFiles();
	void loadIFOData(Map* map, DirectoryParser& parser);
	void loadMonsters(Map* map, const class IFOFile& file);
	void loadNPCs(Map* map, const class IFOFile& file);
	void loadTelegates(Map* map, const class IFOFile& file);

	void extractTelegateFromIfo(std::vector<std::shared_ptr<IFOEntry>> &telegatesFromIfo, ZONFile* zonFile);

	NPCDefaultStatValues* getNPCDefaultValue(const uint32_t id) const {
		return (npcDefaultStatValues.find(id) == npcDefaultStatValues.cend() ? nullptr : npcDefaultStatValues.at(id));
	}

	bool teleportPlayer(Player* player, Map* map, const Position& pos);
protected:
	virtual void onServerStartup();
	virtual bool onPacketsReady(std::shared_ptr<ROSEClient>& client, std::queue<std::shared_ptr<Packet>>& packetQueue);
	virtual void onNewROSEClient(std::shared_ptr<ROSEClient>& roseClient);
	virtual void onROSEClientDisconnecting(std::shared_ptr<ROSEClient>& client);
public:
	WorldServer(uint16_t port);
	virtual ~WorldServer();

	uint32_t getLastLoggedCharacterFromAccount(const uint32_t accountId);
	bool loadCharacterDataForCharacter(Player* player); 
	bool loadInventoryForCharacter(Player* player);

	void loadEncryption();

	bool teleportPlayerFromTelegate(Player* player, const uint16_t telegateId);

	__inline static WorldServer* getInstance() {
		return ROSEServer::getInstance<WorldServer>();
	}
};

#endif //__ROSE_WORLDSERVER__