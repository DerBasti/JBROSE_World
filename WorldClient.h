#ifndef __ROSE_WORLDCLIENT__
#define __ROSE_WORLDCLIENT__

#pragma once

#include "..\JBROSE_Common\ROSEClient.h"
#include "BasicTypes\Entity.h"
#include "BasicTypes/RegenerationProcessor.h"
#include "BasicTypes\Item.h"
#include "BasicTypes/Inventory.h"
#include "BasicTypes/Job.h"
#include "WorldPackets/Responses/PickupDropResponsePacket.h"


class PlayerTraits {
private:
	char* name;
	uint32_t characterId;
	uint8_t sex;
	uint16_t savedSpotId;
	uint32_t faceStyle;
	uint32_t hairStyle;
	Job job;
public:
	PlayerTraits() : job(Job::VISITOR) {
		name = new char[0x20];
		memset(name, 0x00, 0x20);
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
	__inline void setJob(const Job job) {
		this->job = job;
	}
};

class PlayerStats : public EntityStats {
private:
	uint32_t experiencePoints;
	uint16_t stamina;
	uint16_t availableStatPoints;
	uint16_t availableSkillPoints;
	uint16_t maximumWeight;
public:
	PlayerStats() {
		availableStatPoints = availableSkillPoints = 0;
		experiencePoints = 0;
		stamina = 5000;
		maximumWeight = 1100;
	}
	__inline void addExperience(const uint32_t expGained) {
		setExperiencePoints(getExperiencePoints() + expGained);
	}
	__inline uint32_t getExperiencePoints() const {
		return experiencePoints;
	}
	__inline void setExperiencePoints(const uint32_t experiencePoints) {
		this->experiencePoints = experiencePoints;
	}
	__inline uint16_t getStamina() const {
		return stamina;
	}
	__inline void setStamina(const uint16_t stamina) {
		this->stamina = stamina;
	}

	__inline uint16_t getAvailableStatPoints() const {
		return availableStatPoints;
	}
	__inline void setAvailableStatPoints(const uint16_t availableStatPoints) {
		this->availableStatPoints = availableStatPoints;
	}
	__inline void addAvailableStatPoints(uint16_t additionalPoints) {
		setAvailableStatPoints(getAvailableStatPoints() + additionalPoints);
	}

	__inline uint16_t getAvailableSkillPoints() const {
		return availableSkillPoints;
	}
	__inline void setAvailableSkillPoints(const uint16_t availableSkillPoints) {
		this->availableSkillPoints = availableSkillPoints;
	}
	__inline void addAvailableSkillPoints(uint16_t additionalPoints) {
		setAvailableSkillPoints(getAvailableSkillPoints() + additionalPoints);
	}
	__inline uint16_t getMaximumPossibleWeight() const {
		return maximumWeight;
	}
	__inline void setMaximumPossibleWeight(uint16_t newMaximum) {
		maximumWeight = newMaximum;
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
	std::shared_ptr<ROSEClient> networkConnection;
	ROSELogger logger;

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
	bool handleStanceChange(Player* player, const Packet* packet);
	bool handleShowMonsterHp(Player* player, const Packet* packet);
	bool handleTelegateEntered(Player* player, const Packet* packet);

	bool sendEncryption();
public:
	PlayerPacketHandler(std::shared_ptr<ROSEClient>& networkConnection) {
		this->networkConnection = networkConnection;
	}
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
	Inventory* inventory;
	RegenerationProcessor *regenerationProcessor;
	PlayerTraits* traits;
	PlayerAttributeTypes* attributes;
	PacketFactory* packetFactory;
	PlayerPacketHandler* packetHandler;
	
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

	__inline Inventory* getInventory() const {
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