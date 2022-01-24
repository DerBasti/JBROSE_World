#ifndef __ROSE_WORLDSERVER__
#define __ROSE_WORLDSERVER__

#pragma once

#include "..\JBROSE_Common\ROSEServer.h"
#include "Entities/NPC.h"
#include "WorldClient.h"
#include "Map\Map.h"
#include "FileTypes/IFO.h"
#include "FileTypes/STB.h"
#include "FileTypes/AIP.h"
#include "FileTypes/ZMO.h"
#include "FileTypes/CHR.h"
#include "..\JBROSE_Common\DirectoryParser.h"
#include "Map/Telegate.h"

class WorldServer : public ROSEServer {
private:
	std::unordered_map<std::shared_ptr<ROSEClient>, Player*> worldClientList;
	std::unordered_map<uint16_t, Telegate*> telegates;
	std::unordered_map<uint32_t, ZMO*> attackAnimations;
	std::mutex globalNpcListMutex;
	std::unordered_map<uint16_t, NPC*> globalNpcList;
	std::map<uint32_t, std::shared_ptr<class QuestRecord>> questRecords;
	std::map<uint32_t, ZONFile*> zoneFiles;
	std::vector<RestorePoint*> restorePoints;
	CHRFile *npcAnimationFile;
	const static uint16_t MAP_AMOUNT = 120;
	Map* maps[MAP_AMOUNT];
	std::thread mapThreads[MAP_AMOUNT];

	std::shared_ptr<STBFile> aiSTB;
	std::shared_ptr<STBFile> npcSTB;
	std::shared_ptr<STBFile> warpSTB;
	std::shared_ptr<STBFile> dropSTB;
	std::shared_ptr<SkillSTBFile> skillSTB;
	std::shared_ptr<ZoneSTBFile> zoneSTB;
	std::shared_ptr<StatusSTBFile> statusSTB;
	std::shared_ptr<EquipmentSTB*> equipmentSTBs;
	std::shared_ptr<STBFile> motionTypesSTB;
	ConsumableItemList* consumableItemList;

	Player* findWorldClientByInterface(std::shared_ptr<ROSEClient>& client) const;

	void loadFileEntries();
	void loadAi();
	void loadNpcDefaultValues();
	void createMaps();
	void loadZONFiles();
	void loadQuests();
	void loadAttackAnimationTimings();
	void loadIFOData(Map* map, DirectoryParser& parser);
	void loadMonsters(Map* map, const class IFOFile& file);
	void loadNPCs(Map* map, const class IFOFile& file);
	void loadTelegates(Map* map, const class IFOFile& file);

	void extractTelegateFromIfo(std::vector<std::shared_ptr<IFOEntry>> &telegatesFromIfo, ZONFile* zonFile);

	bool saveCharacterDataForCharacter(Player* player);
	bool saveSkilledAttributesForCharacter(Player* player);
	bool saveInventoryForCharacter(Player* player);
	bool saveItemForPlayerInventory(Player* player, const uint8_t slotId);
	bool saveMoneyForCharacter(Player* player);

	ZMO* getAttackAnimationByWeaponId(const uint32_t id) const;
	ZMO* getAttackAnimationBySkillId(const uint32_t id) const;
	Item generateDrop(const uint16_t dropRowId, const uint16_t column) const;
protected:
	virtual void onServerStartup();
	virtual bool onPacketsReady(std::shared_ptr<ROSEClient>& client, std::queue<std::shared_ptr<Packet>>& packetQueue);
	virtual void onNewROSEClient(std::shared_ptr<ROSEClient>& roseClient);
	virtual void onROSEClientDisconnecting(std::shared_ptr<ROSEClient>& client);
public:
	WorldServer(uint16_t port);
	virtual ~WorldServer();

	void loadEncryption();
	bool loadCharacterDataForCharacter(Player* player);
	uint32_t getLastLoggedCharacterFromAccount(const uint32_t accountId);
	bool loadSkilledAttributesForCharacter(Player* player);
	bool loadInventoryForCharacter(Player* player);


	bool saveCharacter(Player* player);
	bool teleportPlayerFromTelegate(Player* player, const uint16_t telegateId);
	bool teleportPlayer(Player* player, Map* map, const Position& pos);

	bool addDropFromNPC(NPC* monster, int16_t levelDifferenceToKiller);

	__inline static WorldServer* getInstance() {
		return ROSEServer::getInstance<WorldServer>();
	}
	__inline const EquipmentSTB* getEquipmentSTB(uint8_t itemType) const {
		return equipmentSTBs.get()[itemType];
	}
	__inline const ConsumeSTBFile* getConsumeSTB() const {
		return dynamic_cast<const ConsumeSTBFile*>(equipmentSTBs.get()[ItemTypeList::CONSUMABLE.getTypeId()]);
	}
	__inline const SkillSTBFile* getSkillSTB() const {
		return skillSTB.get();
	}
	__inline std::shared_ptr<class QuestRecord> getQuestRecordWithQuestHash(uint32_t questHash) {
		return questRecords.find(questHash) != questRecords.cend() ? questRecords.at(questHash) : std::shared_ptr<class QuestRecord>();
	}
	__inline Map* getMapById(const uint16_t mapId) const {
		return mapId < MAP_AMOUNT ? maps[mapId] : nullptr;
	}
	__inline NPC* getGlobalNpcById(uint16_t npcId) const {
		return globalNpcList.at(npcId);
	}
	ZMO* getAttackAnimationForPlayer(Player* player) const;
	ZMO* getAttackAnimationForNpc(NPC* npc) const;

	__inline ConsumableItemList* getConsumableItemList() const {
		return consumableItemList;
	}
};

#endif //__ROSE_WORLDSERVER__