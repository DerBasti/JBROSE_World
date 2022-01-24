#ifndef __QUESTDATA_RESPONSEPACKET__
#define __QUESTDATA_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class QuestDataResponsePacket : public ResponsePacket {
private:
	class PlayerQuestJournal* journal;
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x71B;
	const static uint16_t DEFAULT_LENGTH = 696;
	QuestDataResponsePacket(class PlayerQuestJournal* journal);
	virtual ~QuestDataResponsePacket();

	virtual std::string toPrintable() const {
		char buf[0x100] = { 0x00 };
		sprintf_s(buf, "[QuestDataResponsePacket - %i Bytes length]", DEFAULT_LENGTH);
		return std::string(buf);
	}
};

#endif //__QUESTDATA_RESPONSEPACKET__