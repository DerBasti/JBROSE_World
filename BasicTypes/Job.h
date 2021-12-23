#ifndef __ROSE_JOB__
#define __ROSE_JOB__

#include <cstdint>
#include <unordered_map>

enum class JobId : uint16_t {
	VISITOR_ID = 0,
	SOLDIER_ID = 111,
	KNIGHT_ID = 121,
	CHAMPION_ID = 122,

	MUSE_ID = 211,
	MAGE_ID = 221,
	CLERIC_ID = 222,

	HAWKER_ID = 311,
	RAIDER_ID = 321,
	SCOUT_ID = 322,

	DEALER_ID = 411,
	BOURGEOIS_ID = 421,
	ARTISAN_ID = 422
};

class Job {
private:
	JobId id;
	char* name;
	const static std::unordered_map<uint16_t, const Job> jobList;

	Job(JobId newId, const char* nameOfJob);
public:
	const static Job VISITOR;

	const static Job SOLDIER;
	const static Job KNIGHT;
	const static Job CHAMPION;

	const static Job MUSE;
	const static Job MAGE;
	const static Job CLERIC;

	const static Job HAWKER;
	const static Job RAIDER;
	const static Job SCOUT;

	const static Job DEALER;
	const static Job BOURGEOIS;
	const static Job ARTISAN;

	Job();
	Job(const Job& job);
	virtual ~Job();

	Job& operator=(const Job& job);
	Job& operator=(const JobId id);

	operator JobId() const {
		return id;
	}
	__inline JobId getId() const {
		return id;
	}
	__inline const char* getName() const {
		return name;
	}
	__inline const static Job& getJobFromId(const uint16_t id) {
		return jobList.at(id);
	}
	__inline bool isSecondJob() const {
		return static_cast<uint16_t>(getId()) & 0x10;
	}
};


#endif //__ROSE_JOB__