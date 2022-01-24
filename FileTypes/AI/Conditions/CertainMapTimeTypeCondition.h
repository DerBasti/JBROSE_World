#ifndef __ROSE_CERTAIN_MAP_TIME_CONDITION__
#define __ROSE_CERTAIN_MAP_TIME_CONDITION__

#include "../../AIP.h"
#include "../../../Map/MapTime.h"

class CertainMapTimeTypeCondition : public AICondition {
private:
	MapTimeType timeType;
public:
	CertainMapTimeTypeCondition(std::shared_ptr<char>& rawData);
	~CertainMapTimeTypeCondition();
	virtual bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_CERTAIN_MAP_TIME_CONDITION__