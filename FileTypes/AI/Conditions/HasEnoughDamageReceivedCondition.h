#ifndef __ROSE_HAS_ENOUGH_DAMAGE_RECEIVED_CONDITION__
#define __ROSE_HAS_ENOUGH_DAMAGE_RECEIVED_CONDITION__

#include "..\..\AIP.h"

class HasEnoughDamageReceivedCondition : public AICondition {
private:
	uint32_t damageAmountNecessary;
	bool damageDealtOnEnemyFlag;
public:
	HasEnoughDamageReceivedCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
		LoadedDataReader interpreter(rawData.get());
		interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);
		damageAmountNecessary = interpreter.readUInt();
		damageDealtOnEnemyFlag = interpreter.readUInt() > 0;
	}
	virtual ~HasEnoughDamageReceivedCondition() { }
	virtual bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_HAS_ENOUGH_DAMAGE_RECEIVED_CONDITION__