#ifndef __ROSE_RESPAWN_AFTER_DEATH_REQUEST_PACKET__
#define __ROSE_RESPAWN_AFTER_DEATH_REQUEST_PACKET__

class RespawnAfterDeathRequestPacket : public Packet {
private:

public:
	RespawnAfterDeathRequestPacket(const Packet* packet);
	virtual ~RespawnAfterDeathRequestPacket();
};

#endif //__ROSE_RESPAWN_AFTER_DEATH_REQUEST_PACKET__