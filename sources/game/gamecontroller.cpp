#include <QMap>

#include "sources/game/gamecontroller.h"
#include "sources/game/levelgenerator.h"
#include "sources/game/objects/creatures/enemies/enemy.h"
#include "sources/gui/levelpainter.h"
#include "sources/game/playerturnstate.h"


GameController::GameController(const sharedLoggingListener& logger): logger_(logger) {
    changeState(std::make_shared<PlayerTurnState>());

    player_ = std::make_shared<Player>(Position2D(0, 0));
    player_->setMaxHealth(100);
    player_->setHealth(73);
    player_->setAttackDamage(25);
    player_->setProtection(0);
    player_->getEventManager().subscribe(logger);
}


void GameController::createLevel() {
    LevelGenerator levelGenerator(logger_);
    levelGenerator.generate(Size2D(2 + level_, 2 + level_), level_);
    levelGenerator.spawnEnemies(enemies_, level_);
    level_complete_ = false;
    player_->setPosition(levelGenerator.getEntryPosition());
    player_->setPassFounded(false);
}


void GameController::changeState(const sharedGameState& state) {
    state_ = state;
}


void GameController::startTurn() {
    if (state_ != nullptr) {
        state_->startTurn(*this);
    }
}


void GameController::endTurn() {
    if (state_ != nullptr) {
        state_->endTurn(*this);
    }
}


void GameController::movePlayer(Direction direction) {
    if (state_ != nullptr) {
        state_->movePlayer(*this, direction);
    }
}


void GameController::executePlayerInteraction() {
    if (state_ != nullptr) {
        state_->executePlayerInteraction(*this);
    }
}


void GameController::executePlayerAttack() {
    if (state_ != nullptr) {
        state_->executePlayerAttack(*this);
    }
}


void GameController::checkLevelFinish() {
    if (isPlayerReachedExit() && player_->getPassFounded() && !level_complete_) {
        level_complete_ = true;
        level_++;
    }
}


bool GameController::isLevelComplete() {
    return level_complete_;
}


bool GameController::isPlayerReachedExit() const {
    const Field& field = Field::getInstance();
    return field.getCell(player_->getPosition()).getType() == CellType::Exit;
}


bool GameController::isPassablePosition(const Position2D& position) const {
    const Field& field = Field::getInstance();
    if (field.getCell(position).isPassable()) {
        for (auto& enemy : enemies_) {
            if (enemy->getPosition() == position) {
                return false;
            }
        }
        return position != player_->getPosition();
    }
    return false;
}


void GameController::getLevelPixmap(sharedQPixmap& levelPixmap) const {
    gui::LevelPainter::paint(levelPixmap, player_, enemies_);
}


sharedPlayer GameController::getPlayer() {
    return player_;
}


Enemies& GameController::getEnemies() {
    return enemies_;
}


size_t GameController::getLevelNumber() const {
    return level_;
}


GameController::~GameController() {
    logger_->update("Quitting the game...");
}