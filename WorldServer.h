#ifndef __ROSE_WORLDSERVER__
#define __ROSE_WORLDSERVER__

#pragma once

#include "..\JBROSE_Common\ROSEServer.h"
#include "Entities/NPC.h"
#include "WorldClient.h"
#include "Map\Map.h"
#include "FileTypes/STB.h"
#include "..\JBROSE_Common\DirectoryParser.h"

class WorldServer : public ROSEServer {
private:
	std::unordered_map<std::shared_ptr<ROSEClient>, std::shared_ptr<Player>> worldClientList;
	std::unordered_map<uint32_t, std::shared_ptr<NPCDefaultStatValues>> npcDefaultStatValues;
	std::thread mapThreads[120];
	Map* maps[120];
	
	std::shared_ptr<STBFile> npcFile;
	std::shared_ptr<STBFile> zoneFile;

	std::shared_ptr<Player> findWorldClientByInterface(std::shared_ptr<ROSEClient>& client) const;

	void loadFileEntries();
	void loadNpcDefaultValues();
	void createMaps();
	void spawnMonsters(Map* map, DirectoryParser& parser);

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