#ifndef __ROSE_WORLDCLIENT__
#define __ROSE_WORLDCLIENT__

#pragma once

#include "..\JBROSE_Common\ROSEClient.h"
#include "BasicTypes\Entity.h"
#include "BasicTypes\Item.h"

class PlayerTraits {
private:
	std::string name;
	uint32_t characterId;
	uint8_t sex;
	uint16_t savedSpotId;
	uint32_t faceStyle;
	uint32_t hairStyle;
public:
	PlayerTraits() {
		setName("ADMIN");
		characterId = 1;
		sex = 0;
		savedSpotId = 0;
		faceStyle = 1;
		hairStyle = 0;
	}
	virtual ~PlayerTraits() {}

	__inline uint32_t getCharacterId() const {
		return characterId;
	}
	__inline void setCharacterId(const uint32_t id) {
		characterId = id;
	}
	__inline std::string getName() const {
		return name;
	}
	__inline void setName(const std::string& name) {
		this->name = name;
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
};

class PlayerStats {
private:
	uint16_t level;
	uint16_t jobId;
	uint16_t currentHp;
	uint16_t maxHp;
	uint16_t currentMp;
	uint16_t maxMp;
	uint32_t experiencePoints;
	uint16_t movementSpeed;
	uint16_t stamina;
	uint16_t availableStatPoints;
	uint16_t availableSkillPoints;
public:
	PlayerStats() {
		level = 1;
		jobId = availableStatPoints = availableSkillPoints = 0;
		currentHp = currentMp = 100;
		maxHp = maxMp = 100;
		movementSpeed = 425;
		experiencePoints = 0;
		stamina = 5000;
	}
	__inline uint16_t getLevel() const {
		return level;
	}
	__inline void setLevel(const uint16_t level) {
		this->level = level;
	}
	__inline uint16_t getJobId() const {
		return jobId;
	}
	__inline void setJobId(const uint16_t jobId) {
		this->jobId = jobId;
	}
	__inline uint16_t getCurrentHp() const {
		return currentHp;
	}
	__inline void setCurrentHp(const uint16_t currentHp) {
		this->currentHp = currentHp;
	}
	__inline uint16_t getCurrentMp() const {
		return currentMp;
	}
	__inline void setCurrentMp(const uint16_t currentMp) {
		this->currentMp = currentMp;
	}
	__inline uint32_t getExperiencePoints() const {
		return experiencePoints;
	}
	__inline void setExperiencePoints(const uint16_t experiencePoints) {
		this->experiencePoints = experiencePoints;
	}
	__inline uint16_t getMovementSpeed() const {
		return movementSpeed;
	}
	__inline void setMovementSpeed(const uint16_t speed) {
		movementSpeed = speed;
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

	__inline uint16_t getAvailableSkillPoints() const {
		return availableSkillPoints;
	}
	__inline void setAvailableSkillPoints(const uint16_t availableSkillPoints) {
		this->availableSkillPoints = availableSkillPoints;
	}
};

class Inventory {
private:
	uint64_t moneyAmount;
	Item inventorySlots[140];
public:
	Inventory() {}
	virtual ~Inventory() {}

	__inline const Item& getItem(const uint8_t slot) const {
		return inventorySlots[slot];
	}

	__inline void setItem(const uint8_t slot, const Item& item) {
		inventorySlots[slot] = item;
	}

	__inline void clearItem(const uint8_t slot) {
		inventorySlots[slot] = Item();
	}
};

class Player : public Entity{
private:
	uint32_t accountId;
	std::shared_ptr<ROSEClient> networkConnection;
	Inventory* inventory;
	PlayerStats* stats;
	PlayerTraits* traits;
	PacketFactory* packetFactory;

	bool sendEncryption();
	bool loadEntirePlayer();
protected:

	bool handleAssignmentOfLocalId(const Packet* packet);
	bool handleCollision(const Packet* packet);
	bool handleChangedRespawnTown(const Packet* packet);
	bool handleIdentification(const Packet* packet);
	bool handleNewDestination(const Packet* packet);
	bool handleTelegateEntered(const Packet* packet);

	__inline void setAccountId(const uint32_t accId) {
		accountId = accId;
	}

	__inline std::shared_ptr<ROSEClient> getConnectionWrapper() const {
		return networkConnection;
	}
public:
	Player(std::shared_ptr<ROSEClient>& networkInterface);
	virtual ~Player();

	bool handlePacket(const Packet* packet);
	virtual bool spawnVisually(Entity* entity);
	virtual bool despawnVisually(Entity* entity);
	virtual bool despawnVisually(uint16_t entityId);

	__inline bool sendDataToSelf(const ResponsePacket& packet) const {
		return getConnectionWrapper()->sendData(packet);
	}
	bool sendDataToVisible(const ResponsePacket& packet) const;
	bool sendDataToVisibleExceptSelf(const ResponsePacket& packet) const;

	__inline Inventory* getInventory() const {
		return inventory;
	}
	__inline PlayerTraits* getTraits() const {
		return traits;
	}
	__inline PlayerStats* getStats() const {
		return stats;
	}

	void onDisconnect();

	__inline virtual bool isPlayer() const {
		return true;
	}
};

#endif //__ROSE_WORLDCLIENT__