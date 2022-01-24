#ifndef __ROSE_MAP_SECTOR__
#define __ROSE_MAP_SECTOR__
#pragma once

#include <inttypes.h>
#include <unordered_map> 
#include <queue>
#include <mutex>
#include "..\BasicTypes\Position.h"

class MapSector {
private:

	class DistributionPacket {
		private:
			std::shared_ptr<class ResponsePacket> responsePacket;
			const class Player* playerNotToSendPacketTo;
		public:
			DistributionPacket(const std::shared_ptr<class ResponsePacket>& responsePacket) : DistributionPacket(responsePacket, nullptr) {

			}
			DistributionPacket(const std::shared_ptr<class ResponsePacket>& responsePacket, const Player* playerNotToSendPacketTo) : playerNotToSendPacketTo(playerNotToSendPacketTo) {
				this->responsePacket = responsePacket;
				this->playerNotToSendPacketTo = playerNotToSendPacketTo;
			}
			virtual ~DistributionPacket() {
				playerNotToSendPacketTo = nullptr;
			}
			__inline std::shared_ptr<class ResponsePacket> getResponsePacket() const {
				return responsePacket;
			}
			__inline const Player* getPlayerNotToSendPacketTo() const {
				return playerNotToSendPacketTo;
			}
	};

	uint32_t id;
	std::unordered_map<uint16_t, class Entity*> entitiesInSector;
	std::unordered_map<uint16_t, class Player*> playerInSector;
	std::mutex packetQueueMutex;
	std::queue<DistributionPacket> visualityChangePacketQueue;
	Position center;

	bool despawnDisconnectingPlayerVisually(uint16_t localId);

	bool sendDataToAllPlayer(const std::shared_ptr<ResponsePacket>& packet);
	bool sendDataToAllPlayerExcept(const std::shared_ptr<ResponsePacket>& packet, const class Player* player);
public:
	MapSector(const uint32_t id, const Position& center);
	virtual ~MapSector();

	bool addEntity(Entity* entity);
	bool hasEntity(const uint16_t id);
	Entity* getEntity(const uint16_t id);
	void removeEntity(Entity* entity);
	void removeEntity(uint16_t localId);
	void removeDisconnectingPlayer(uint16_t entityLocalId);

	Entity* findFirstEntity(std::function<bool(Entity*)> searchFunction);

	bool spawnEntityVisually(class Entity* entity);
	bool despawnEntityVisually(class Entity* entity);

	void distributeVisualityChangePackets();
	void addPacketToDistributionQueue(const std::shared_ptr<ResponsePacket>& packet);
	void addPacketToDistributionQueue(const std::shared_ptr<ResponsePacket>& packet, const class Player* playerNotToSendPacketTo);
	
	__inline uint32_t getId() const {
		return id;
	}
	__inline const Position& getCenterPosition() const {
		return center;
	}
	__inline const std::unordered_map<uint16_t, class Entity*>& getAllEntitiesOfSector() const {
		return entitiesInSector;
	}
};

#endif //__ROSE_MAP_SECTOR__