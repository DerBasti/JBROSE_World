#include "Job.h"

Job::Job() : Job(JobId::VISITOR_ID, "") {

}
Job::Job(const Job& job) : Job(job.getId(), job.getName()) {

}

Job::Job(JobId newId, const char* nameOfJob) {
	id = newId;
	//name = std::shared_ptr<char>(new char[0x20], std::default_delete<char[]>());
	name = new char[0x20];
	strncpy_s(name, 0x20, nameOfJob, strlen(nameOfJob));
}

Job::~Job() {

}

Job& Job::operator=(const Job& job) {
	this->id = job.id;
	strncpy_s(name, 0x20, job.getName(), 0x1F);
	return (*this);
}
Job& Job::operator=(const JobId id) {
	this->id = id;
	return (*this);
}

const Job Job::VISITOR = Job(JobId::VISITOR_ID, "Visitor");

const Job Job::SOLDIER = Job(JobId::SOLDIER_ID, "Soldier");
const Job Job::KNIGHT = Job(JobId::KNIGHT_ID, "Knight");
const Job Job::CHAMPION = Job(JobId::CHAMPION_ID, "Champion");

const Job Job::MUSE = Job(JobId::MUSE_ID, "Muse");
const Job Job::MAGE = Job(JobId::MAGE_ID, "Mage");
const Job Job::CLERIC = Job(JobId::CLERIC_ID, "Cleric");

const Job Job::HAWKER = Job(JobId::HAWKER_ID, "Hawker");
const Job Job::RAIDER = Job(JobId::RAIDER_ID, "Raider");
const Job Job::SCOUT = Job(JobId::SCOUT_ID, "Scout");

const Job Job::DEALER = Job(JobId::DEALER_ID, "Dealer");
const Job Job::BOURGEOIS = Job(JobId::BOURGEOIS_ID, "Bourgeois");
const Job Job::ARTISAN = Job(JobId::ARTISAN_ID, "Artisan");

const std::unordered_map<uint16_t, const Job> Job::jobList{

	std::make_pair(static_cast<uint16_t>(Job::VISITOR.getId()), Job::VISITOR),
	std::make_pair(static_cast<uint16_t>(Job::SOLDIER.getId()), Job::SOLDIER),
	std::make_pair(static_cast<uint16_t>(Job::KNIGHT.getId()), Job::KNIGHT),
	std::make_pair(static_cast<uint16_t>(Job::CHAMPION.getId()), Job::CHAMPION),
	std::make_pair(static_cast<uint16_t>(Job::MUSE.getId()), Job::MUSE),
	std::make_pair(static_cast<uint16_t>(Job::MAGE.getId()), Job::MAGE),
	std::make_pair(static_cast<uint16_t>(Job::CLERIC.getId()), Job::CLERIC),
	std::make_pair(static_cast<uint16_t>(Job::HAWKER.getId()), Job::HAWKER),
	std::make_pair(static_cast<uint16_t>(Job::RAIDER.getId()), Job::RAIDER),
	std::make_pair(static_cast<uint16_t>(Job::SCOUT.getId()), Job::SCOUT),
	std::make_pair(static_cast<uint16_t>(Job::DEALER.getId()), Job::DEALER),
	std::make_pair(static_cast<uint16_t>(Job::BOURGEOIS.getId()), Job::BOURGEOIS),
	std::make_pair(static_cast<uint16_t>(Job::ARTISAN.getId()), Job::ARTISAN)
};
