#ifndef __QUESTDATA_RESPONSEPACKET__
#define __QUESTDATA_RESPONSEPACKET__
#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class QuestJournalEntry {
public:
	const static uint8_t AMOUNT_OF_QUESTVARS = 10;
	const static uint8_t AMOUNT_OF_ITEMS = 5;
private:
	uint16_t id;
	uint32_t timePassed;

	uint16_t questVar[AMOUNT_OF_QUESTVARS];

	uint32_t leverBits;

	uint16_t itemHeader[AMOUNT_OF_ITEMS];
	uint32_t itemData[AMOUNT_OF_ITEMS];

public:
	QuestJournalEntry() {
		id = 0;
		timePassed = leverBits = 0;
		for (uint8_t i = 0; i < AMOUNT_OF_ITEMS; i++) {
			itemHeader[i] = itemData[i] = 0;
		}
		for (uint16_t i = 0; i < AMOUNT_OF_QUESTVARS; i++) {
			questVar[i] = 0;
		}
	}
	virtual ~QuestJournalEntry() {

	}
	__inline uint16_t getId() const {
		return id;
	}
	__inline uint32_t getTimePassed() const {
		return timePassed;
	}
	__inline uint16_t getQuestVar(uint8_t slot) const {
		return questVar[slot];
	}
	__inline uint32_t getLeverBits() const {
		return leverBits;
	}
	__inline uint16_t getItemHeader(uint8_t slot) const {
		return itemHeader[slot];
	}
	__inline uint32_t getItemData(uint8_t slot) const {
		return itemData[slot];
	}
};

class QuestDataResponsePacket : public ResponsePacket {
private:
	uint16_t episodeVars[5];
	uint16_t jobVars[3];
	uint16_t planetVars[7];
	uint16_t unionVars[10];

	QuestJournalEntry entry[10];

	uint32_t globalQuestFlag[10];
protected:
	void appendContentToSendable(SendablePacket& packet) const;
public:
	const static uint16_t ID = 0x71B;
	const static uint16_t DEFAULT_LENGTH = 696;
	QuestDataResponsePacket();
	virtual ~QuestDataResponsePacket();

	virtual std::string toPrintable() const {
		char buf[0x100] = { 0x00 };
		sprintf_s(buf, "[QuestDataResponsePacket - %i Bytes length]", DEFAULT_LENGTH);
		return std::string(buf);
	}
};

#endif //__QUESTDATA_RESPONSEPACKET__