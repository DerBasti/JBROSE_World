#include "NPC.h"
#include "../FileTypes/AIP.h"
#include "../FileTypes/AI/AIPProcessor.h"
#include "../WorldClient.h"
#include "Drop.h"
#include "../Map/Map.h"
#include "..\WorldPackets\Responses\SpawnMonsterVisuallyResponsePacket.h"
#include "..\FileTypes\STB.h"
#include "../../JBROSE_Common/DirectoryParser.h"
#include "../Map/MonsterRecoveryPoint.h"
#include "../WorldServer.h"

NPC::NPC(NPCDefaultStatValues* defaultValues, AIP* aiProtocol, MonsterRecoveryPoint* spawn) : NPC(defaultValues, aiProtocol, spawn->getRandomPositionFromSpawn()) {
	spawnPoint = spawn;
}

NPC::NPC(NPCDefaultStatValues* defaultValues, AIP* ai, const Position& spawnPosition) {
	logger.setLoggerName("NPC");
	aiProtocol = ai;
	aiProcessor = new AIPProcessor(this);
	spawnPoint = nullptr;
	stats = new EntityStats();

	getCombat()->setTeamId(EntityDefaultTeamId::NPC);

	getLocationData()->getMapPosition()->setCurrentPosition(spawnPosition);
	getLocationData()->getMapPosition()->setDestinationPosition(spawnPosition);
	getLocationData()->getMapPosition()->setSourcePosition(spawnPosition);

	defaultStatValues = defaultValues;
	getStats()->setMaxHp(defaultValues->getMaxHP());
	getStats()->setCurrentHp(defaultValues->getMaxHP());
	getStats()->getCombatValues()->setAttackPower(defaultValues->getAttackPower());
	getStats()->getCombatValues()->setDefense(defaultValues->getDefense());

	visualityProcessor = new NPCVisualityProcessor(this);

	stance = new NPCStance([&]() {
		updateMovementSpeed();
		updateStanceVisually();
	});
	stance->setRunningStance();
}

NPC::~NPC() {
	delete visualityProcessor;
	visualityProcessor = nullptr;

	delete aiProcessor;
	aiProcessor = nullptr;

	delete stance;
	stance = nullptr;
}

const char* NPC::getName() const {
	return defaultStatValues->getName();
}

void NPC::onDamageReceived(Entity* attacker, uint32_t damageAmount) {
	uint16_t localId = attacker->getLocationData()->getLocalId();
	if (damageMap.find(localId) != damageMap.end()) {
		damageMap[localId] += damageAmount;
	}
	else {
		damageMap.emplace(std::make_pair(localId, damageAmount));
	}
	this->updateAiProcessor(AIEvent::DAMAGED, attacker);
	logger.logDebug(getName(), "(#", getLocationData()->getLocalId(), ") received ", damageAmount, " damage from ", attacker->getName(), ".");
}

void NPC::onDeath() {
	Entity::onDeath();
	uint32_t totalExperience = getDefaultStatValues()->getExperiencePoints() * getDefaultStatValues()->getLevel();
	Entity* lastAttacker = nullptr;
	Entity* highestDamageAttacker = nullptr;
	float highestDamagePercentage = 0.0f;
	for (auto it = damageMap.begin(); it != damageMap.end(); it++) {
		float percentage = (it->second+1) / static_cast<float>(this->getStats()->getMaxHp() + 1.0f);
		uint32_t expGained = static_cast<uint32_t>(totalExperience * percentage);
		Entity* currentEntity = getLocationData()->getMap()->findEntityByLocalId(it->first);
		if (!currentEntity) {
			continue;
		}
		logger.logDebug("Entity ", currentEntity->getName(), " dealt ", (percentage*100.0f), "% of the damage. Experience gained: ", expGained);
		if (currentEntity->isPlayer()) {
			Player* player = dynamic_cast<Player*>(currentEntity);
			player->addExperience(expGained);
			lastAttacker = player;
		}
		if (highestDamagePercentage < percentage && currentEntity->isPlayer()) {
			highestDamagePercentage = percentage;
			highestDamageAttacker = currentEntity;
		}
	}
	this->updateAiProcessor(AIEvent::DEATH, lastAttacker);
	logger.logDebug(getName(), "(#", getLocationData()->getLocalId(), ") was killed.");
	WorldServer::getInstance()->addDropFromNPC(this, getDefaultStatValues()->getLevel() - highestDamageAttacker->getStats()->getLevel());
	getLocationData()->getMap()->addEntityToRemovalQueue(this, RemovalReason::MONSTER_DEATH);
}

void NPC::updateAiProcessor(AIEvent eventType, Entity* designatedTarget)
{
	aiProcessor->triggerProcess(eventType, designatedTarget);
}

void NPC::updateAiTriggerTime() {
	aiProcessor->updateTriggerTimer();
}

void NPC::onIdle() {
	updateAiProcessor(AIEvent::IDLE);
}

void NPC::onMoving() {
	updateAiTriggerTime();
}

void NPC::updateAttackPower() {
	uint16_t attackPower = getDefaultStatValues()->getAttackPower();
	getStats()->getCombatValues()->setAttackPower(attackPower);
}

void NPC::updateDefense() {
	uint16_t defense = getDefaultStatValues()->getDefense();
	getStats()->getCombatValues()->setDefense(defense);
}

void NPC::updateMagicDefense() {
	uint16_t magicDefense = getDefaultStatValues()->getMagicDefense();
	getStats()->getCombatValues()->setMagicDefense(magicDefense);
}

void NPC::updateCriticalRate() {
	uint16_t criticalRate = 15;
}

void NPC::updateAttackSpeed() {
	uint16_t attackSpeed = getDefaultStatValues()->getAttackSpeed();
	getStats()->getCombatValues()->setAttackSpeed(attackSpeed);
}

void NPC::updateAttackRange() {
	uint16_t attackRange = getDefaultStatValues()->getAttackRange();
	getStats()->getCombatValues()->setAttackRange(attackRange);
}

void NPC::updateMovementSpeed() { 
	uint16_t movementSpeed = getDefaultStatValues()->getRunningSpeed();
	if (getStance()->isWalking()) {
		movementSpeed = getDefaultStatValues()->getWalkingSpeed();
	}
	getStats()->setMovementSpeed(movementSpeed);
}

void NPC::updateAttackAnimation() {
	ZMO* animation = WorldServer::getInstance()->getAttackAnimationForNpc(this);
	logger.logTrace("ZMO-Animation for NPC loaded: ", animation->getFilePath().c_str());
	getCombat()->setAttackAnimation(animation);
}



NPCDefaultStatValues::NPCDefaultStatValues(uint16_t id, STBEntry* entry) {
	this->id = id;
	name = entry->getColumnData(0);
	walkingSpeed = entry->getColumnDataAsInt(2);
	runningSpeed = entry->getColumnDataAsInt(3);
	size = static_cast<float>(entry->getColumnDataAsInt(4));
	level = entry->getColumnDataAsInt(7);
	hpPerLevel = entry->getColumnDataAsInt(8);
	attackPower = entry->getColumnDataAsInt(9);
	defense = entry->getColumnDataAsInt(11);
	magicDefense = entry->getColumnDataAsInt(12);
	attackSpeed = entry->getColumnDataAsInt(14);
	maxHp = level * hpPerLevel;
	aiId = entry->getColumnDataAsInt(16);
	dropItemChance = entry->getColumnDataAsInt(19);
	dropMoneyChance = entry->getColumnDataAsInt(20);
	attackRange = entry->getColumnDataAsInt(26);
	npcFlag = entry->getColumnDataAsInt(27) == 999;
	experiencePoints = entry->getColumnDataAsInt(17);
}

NPCDefaultStatValues::~NPCDefaultStatValues() {

}

void NPCCreationFactory::initializeFromSTB(STBFile* npcStbFile, STBFile* aiSTB) {
	ROSELogger logger;
	logger.logInfo("Loading AI...");
	for (uint32_t i = 0; i < aiSTB->getEntryAmount(); i++) {
		auto stbEntry = aiSTB->getEntry(i);
		std::string aiFileName = std::string("D:\\Games\\ROSE Server\\VFS_Extrator\\");
		aiFileName = aiFileName.append(std::string(stbEntry->getColumnData(0x00)));
		if (DirectoryParser::isFileExistent(aiFileName.c_str())) {
			AIP* aiProtocol = new AIP(aiFileName.c_str());
			aiProtocols.insert(std::make_pair(i, aiProtocol));
		}
	}
	logger.logInfo("Finished loading AI.");
	logger.logInfo("Loading NPC default values...");
	auto allEntries = npcStbFile->getAllEntries();
	for (auto it = allEntries.cbegin(); it != allEntries.cend(); it++) {
		auto entry = it->second;
		npcDefaultStatValues.insert(std::make_pair(it->first, new NPCDefaultStatValues(it->first, entry)));
	}
	logger.logInfo("Finished loading NPC default values.");
}

std::unordered_map<uint32_t, NPCDefaultStatValues*> NPCCreationFactory::npcDefaultStatValues;
std::unordered_map<uint32_t, AIP*> NPCCreationFactory::aiProtocols;

NPC* NPCCreationFactory::createNpc(const uint32_t id, const Position& spawnPosition) {
	return new NPC(npcDefaultStatValues[id], aiProtocols[npcDefaultStatValues[id]->getAiId()], spawnPosition);
}

NPC* NPCCreationFactory::createNpc(const uint32_t id, MonsterRecoveryPoint* spawnPoint) {
	return new NPC(npcDefaultStatValues[id], aiProtocols[npcDefaultStatValues[id]->getAiId()], spawnPoint);
}

void NPCCreationFactory::deleteAllEntries() {
	std::for_each(aiProtocols.begin(), aiProtocols.end(), [](std::pair<uint32_t, AIP*> aiPair) {
		delete aiPair.second;
		aiPair.second = nullptr;
	});
	std::for_each(npcDefaultStatValues.begin(), npcDefaultStatValues.end(), [](std::pair<uint16_t, NPCDefaultStatValues*> npcStatPair) {
		delete npcStatPair.second;
		npcStatPair.second = nullptr;
	});
}