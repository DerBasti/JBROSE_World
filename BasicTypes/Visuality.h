#ifndef __ROSE_VISUALITY__
#define __ROSE_VISUALITY__
#pragma once

#include <memory>
#include <unordered_map>

class Entity;

class VisualityProcessor {
private:
	bool updateRequired;
	std::unordered_map<uint32_t, class MapSector*> visibleSectors;
	Entity* entity;
protected:
	__inline Entity* getEntity() const {
		return entity;
	}
public:
	VisualityProcessor(Entity* entity);
	virtual ~VisualityProcessor();
	virtual bool updateVisuality();

	virtual std::shared_ptr<class SpawnEntityVisuallyResponsePacket> createSpawnVisuallyPacket();
	Entity* findEntity(const uint16_t localId);

	__inline bool isVisualityUpdateRequired() const {
		return updateRequired;
	}
	__inline void setVisualityUpdateRequired(bool flag) {
		updateRequired = flag;
	}
	__inline std::unordered_map<uint32_t, class MapSector*> getVisibleSectors() const {
		return visibleSectors;
	}
};

class PlayerVisualityProcessor : public VisualityProcessor {
public:
	PlayerVisualityProcessor(Entity* entity);
	virtual ~PlayerVisualityProcessor();

	virtual std::shared_ptr<class SpawnEntityVisuallyResponsePacket> createSpawnVisuallyPacket();
};

class NPCVisualityProcessor : public VisualityProcessor {
public:
	NPCVisualityProcessor(Entity* entity);
	virtual ~NPCVisualityProcessor();

	virtual std::shared_ptr<class SpawnEntityVisuallyResponsePacket> createSpawnVisuallyPacket();
};

class MonsterVisualityProcessor : public VisualityProcessor {
public:
	MonsterVisualityProcessor(Entity* entity);
	virtual ~MonsterVisualityProcessor();

	virtual std::shared_ptr<class SpawnEntityVisuallyResponsePacket> createSpawnVisuallyPacket();
};

class DropVisualityProcessor : public VisualityProcessor {
public:
	DropVisualityProcessor(Entity* entity);
	virtual ~DropVisualityProcessor();

	virtual std::shared_ptr<class SpawnEntityVisuallyResponsePacket> createSpawnVisuallyPacket();
};

#endif //__ROSE_VISUALITY__