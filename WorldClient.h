#ifndef __ROSE_WORLDCLIENT__
#define __ROSE_WORLDCLIENT__

#pragma once

#include "..\JBROSE_Common\ROSEClient.h"
#include "BasicTypes\Entity.h"
#include "BasicTypes/RegenerationProcessor.h"
#include "BasicTypes\Item.h"
#include "BasicTypes/Inventory.h"
#include "BasicTypes/Job.h"
#include "BasicTypes/Skill.h"
#include "WorldPackets/Responses/PickupDropResponsePacket.h"


class PlayerTraits {
private:
	char* name;
	uint32_t characterId;
	uint8_t birthStone;
	uint8_t sex;
	uint16_t savedSpotId;
	uint32_t faceStyle;
	uint32_t hairStyle;
	Job job;
public:
	PlayerTraits() : job(Job::VISITOR) {
		name = new char[0x20];
		memset(name, 0x00, 0x20);
		birthStone = 0;
		characterId = 1;
		sex = 0;
		savedSpotId = 0;
		faceStyle = 1;
		hairStyle = 0;
	}
	virtual ~PlayerTraits() {
		delete[] name;
		name = nullptr;
	}

	__inline uint32_t getCharacterId() const {
		return characterId;
	}
	__inline void setCharacterId(const uint32_t id) {
		characterId = id;
	}
	__inline const char* getName() const {
		return name;
	}
	__inline void setName(const std::string& nameToCopy) {
		setName(nameToCopy.c_str());
	}
	__inline void setName(const char* name) {
		strncpy_s(this->name, 0x20, name, 0x1F);
	}
	__inline uint8_t getBirthstone() const {
		return birthStone;
	}
	__inline void setBirthstone(uint8_t birthstone) {
		this->birthStone = birthstone;
	}

	__inline uint8_t getSex() const {
		return sex;
	}
	__inline void setSex(uint8_t sex) {
		this->sex = sex;
	}

	__inline uint16_t getSavedSpotId() const {
		return savedSpotId;
	}
	__inline void setSavedSpotId(const uint16_t spotId) {
		savedSpotId = spotId;
	}

	__inline uint32_t getFaceStyle() const {
		return faceStyle;
	}
	__inline void setFaceStyle(const uint32_t faceStyle) {
		this->faceStyle = faceStyle;
	}

	__inline uint32_t getHairStyle() const {
		return hairStyle;
	}
	__inline void setHairStyle(const uint32_t hairStyle) {
		this->hairStyle = hairStyle;
	}
	__inline const Job& getJob() const {
		return job;
	}
	__inline uint16_t getJobNumeric() const {
		return static_cast<uint16_t>(job.getId());
	}
	__inline void setJobNumeric(uint16_t jobId) {
		setJob(Job::getJobFromId(jobId));
	}
	__inline void setJob(const Job& job) {
		this->job = job;
	}
};

class PlayerAttributes {
private:
	const char* attributeName;
	uint16_t pointsLearned;
	uint16_t pointsBuffed;
public:
	PlayerAttributes(const char* name, uint16_t pointsDistributed) {
		attributeName = name;
		pointsLearned = pointsDistributed;
		pointsBuffed = 0;
	}
	virtual ~PlayerAttributes() {
	}

	PlayerAttributes& operator=(const uint16_t value) {
		pointsLearned = value;
		return (*this);
	}
	__inline const char* getAttributeName() const {
		return attributeName;
	}
	operator uint16_t() const {
		return getPointsTotal();
	}

	__inline uint16_t getPointsLearned() const {
		return pointsLearned;
	}
	__inline void increasePointsLearned() {
		pointsLearned++;
	}
	__inline void setPointsLearned(const uint16_t points) {
		pointsLearned = points;
	}
	__inline uint16_t getPointsBuffed() const {
		return pointsBuffed;
	}
	__inline void setPointsBuffed(const uint16_t points) {
		pointsBuffed = points;
	}
	__inline uint16_t getPointsTotal() const {
		return getPointsBuffed() + getPointsLearned();
	}
};

class PlayerAttributeTypes {
private:
	PlayerAttributes* strength;
	PlayerAttributes* dexterity;
	PlayerAttributes* intelligence;
	PlayerAttributes* concentration;
	PlayerAttributes* charm;
	PlayerAttributes* sensibility;
	using PlayerAttributesCallback = PlayerAttributes* (PlayerAttributeTypes::*)() const;

	static PlayerAttributesCallback functions[];
public:
	const static uint8_t STRENGTH = 0;
	const static uint8_t DEXTERITY = 1;
	const static uint8_t INTELLIGENCE = 2;
	const static uint8_t CONCENTRATION = 3;
	const static uint8_t CHARM = 4;
	const static uint8_t SENSIBLITY = 5;
	PlayerAttributeTypes();
	virtual ~PlayerAttributeTypes();

	__inline PlayerAttributes* getStrength() const {
		return strength;
	}
	__inline PlayerAttributes* getDexterity() const {
		return dexterity;
	}
	__inline PlayerAttributes* getIntelligence() const {
		return intelligence;
	}
	__inline PlayerAttributes* getConcentration() const {
		return concentration;
	}
	__inline PlayerAttributes* getCharm() const {
		return charm;
	}
	__inline PlayerAttributes* getSensibility() const {
		return sensibility;
	}
	PlayerAttributes* getAttributeByType(uint8_t type) {
		auto func = functions[type];
		return (this->*func)();
	}
};

class Player;

class PlayerPacketHandler {
private:
	typedef typename bool (PlayerPacketHandler::*PacketHandlerMethod)(Player*, const Packet*) ;
	std::shared_ptr<ROSEClient> networkConnection;
	std::mutex queueMutex;
	std::queue<std::shared_ptr<Packet>> packetQueue;
	std::map<uint16_t, PacketHandlerMethod> handleMethods;
	ROSEThreadedLogger logger;

	__inline std::shared_ptr<ROSEClient> getConnectionWrapper() const {
		return networkConnection;
	}
protected:
	bool handleAssignmentOfLocalId(Player* player, const Packet* packet);
	bool handleCollision(Player* player, const Packet* packet);
	bool handleChangedEquipment(Player* player, const Packet* packet);
	bool handleChangedRespawnTown(Player* player, const Packet* packet);
	bool handleShopClose(Player* player, const Packet* packet);
	bool handleWeightChange(Player* player, const Packet* packet);
	bool handleDistributionOfStatPoint(Player* player, const Packet *packet);
	bool handleDropFromInventory(Player* player, const Packet* packet);
	bool handleExit(Player* player, const Packet* packet);
	bool handleIdentification(Player* player, const Packet* packet);
	bool handleInitBasicAttack(Player* player, const Packet* packet);
	bool handleNewDestination(Player* player, const Packet* packet);
	bool handlePickupDrop(Player* player, const Packet* packet);
	bool handleQuestJournalUpdate(Player* player, const Packet* packet);
	bool handleStanceChange(Player* player, const Packet* packet);
	bool handleShowMonsterHp(Player* player, const Packet* packet);
	bool handleTelegateEntered(Player* player, const Packet* packet);
	bool handleUseConsumableItem(Player* player, const Packet* packet);
	bool handleEmptyPacket(Player* player, const Packet* packet);

	bool sendEncryption();
public:
	PlayerPacketHandler(std::shared_ptr<ROSEClient>& networkConnection);
	virtual ~PlayerPacketHandler() {
	
	}

	bool handlePacket(Player* player, const Packet* packet);
	__inline bool sendDataToClient(const ResponsePacket& packet) const {
		return getConnectionWrapper()->sendData(packet);
	}
};

class Player : public Entity{
private:
	uint32_t accountId;
	PlayerInventory* inventory;
	RegenerationProcessor *regenerationProcessor;
	PlayerTraits* traits;
	PlayerAttributeTypes* attributes;
	PacketFactory* packetFactory;
	PlayerPacketHandler* packetHandler;
	SkillList<140> skillList;
	class PlayerQuestJournal* questJournal;
	
	class ZMO* attackAnimation;

	uint32_t getExperienceForLevelup() const;
	bool handleLevelup();
	bool sendLevelupToVisibleEntities();

	void updateAttackPower();
	void updateDefense();
	void updateMagicDefense();
	void updateCriticalRate();
	void updateAttackSpeed();
	void updateAttackRange();
	void updateMaximumHp();
	void updateMaximumMp();
	void updateMaximumWeight();
protected:
	__inline void setAccountId(const uint32_t accId) {
		accountId = accId;
	}

public:
	Player(std::shared_ptr<ROSEClient>& networkInterface);
	virtual ~Player();

	bool handlePacket(const Packet* packet);
	virtual bool spawnVisually(Entity* entity);
	virtual bool despawnVisually(Entity* entity);
	virtual bool despawnVisually(uint16_t entityId);
	void addExperience(uint32_t expGained);

	virtual const char* getName() const;
	bool loadPlayerDataFromAccount(uint32_t accountId);
	void updateMovementSpeed();
	void updateAttackAnimation();

	virtual void updateCombatValues() {
		Entity::updateCombatValues();
		updateMaximumWeight();
	}

	__inline PlayerInventory* getInventory() const {
		return inventory;
	}
	__inline PlayerTraits* getTraits() const {
		return traits;
	}
	__inline PlayerStats* getStats() const {
		return dynamic_cast<PlayerStats*>(Entity::getStats());
	}
	__inline PlayerAttributeTypes* getAttributes() const {
		return attributes;
	}
	__inline PlayerPacketHandler* getPacketHandler() const {
		return packetHandler;
	}
	__inline RegenerationProcessor* getRegenerationProcessor() const {
		return regenerationProcessor;
	}
	__inline PlayerQuestJournal* getQuestJournal() const {
		return questJournal;
	}

	virtual bool sendDataToSelf(const ResponsePacket& packet) {
		return this->getPacketHandler()->sendDataToClient(packet);
	}
	void onDamageReceived(Entity* attacker, uint32_t damageAmount);
	void onDisconnect();
	virtual void onUpdate();
	PickupDropResponsePacket onDropPickup(class Drop *drop);

	__inline virtual bool isPlayer() const {
		return true;
	}
};

#endif //__ROSE_WORLDCLIENT__