#include "MapRemovalRequest.h"
#include "..\BasicTypes\Entity.h"

std::shared_ptr<RemovalRequest> RemovalRequestFactory::createRemovalRequest(Entity* entity, RemovalReason reason) {
	std::shared_ptr<RemovalRequest> removalRequest;
	switch (reason) {
		case RemovalReason::MONSTER_DEATH:
		break;
		case RemovalReason::PLAYER_DISCONNECT:
			removalRequest = std::shared_ptr<RemovalRequest>(
				new RemovalRequest(entity->getLocationData()->getLocalId(), entity->getLocationData()->getCurrentMapSector())
				, [entity](RemovalRequest* req) -> void {
				delete req;
				delete entity;
			});
		break;
		case RemovalReason::TELEPORT:
			removalRequest = std::shared_ptr<RemovalRequest>(
				new RemovalRequest(entity->getLocationData()->getLocalId(), entity->getLocationData()->getCurrentMapSector())
				);
		break;
	}
	return removalRequest;
}

RemovalRequest::RemovalRequest() : RemovalRequest(0, nullptr) {}

RemovalRequest::RemovalRequest(const uint16_t localId, MapSector* sector) {
	this->localId = localId;
	registeredSector = sector;
}

RemovalRequest::~RemovalRequest() {
	registeredSector = nullptr;
	localId = 0;
}