#ifndef SOURCES_GAME_GAMECONTROLLER_H
#define SOURCES_GAME_GAMECONTROLLER_H

#include <QPixmap>

#include "sources/game/objects/creatures/player/player.h"
#include "sources/game/objects/creatures/enemies/enemy.h"
#include "sources/game/field.h"
#include "sources/game/gamestate.h"
#include "sources/logging/logginglistener.h"

using sharedGameController = std::shared_ptr<class GameController>;
using sharedQPixmap = std::shared_ptr<QPixmap>;
using Enemies = std::vector<sharedCreature>;
using sharedGameState = std::shared_ptr<class GameState>;


class GameController final {
private:
    sharedPlayer player_;
    Enemies enemies_;
    sharedLoggingListener logger_;
    sharedGameState state_;
    size_t level_ = 0;
    bool level_complete_ = false;

public:
    GameController(const sharedLoggingListener& logger);
    ~GameController();

    void startTurn();
    void endTurn();
    void createLevel();
    void movePlayer(Direction direction);
    void executePlayerInteraction();
    void executePlayerAttack();
    void changeState(const sharedGameState& state);
    void checkLevelFinish();
    bool isLevelComplete();
    size_t getLevelNumber() const;
    bool isPlayerReachedExit() const;
    bool isPassablePosition(const Position2D& position) const;
    void getLevelPixmap(sharedQPixmap& level_pixmap) const;
    sharedPlayer getPlayer();
    Enemies& getEnemies();
}; // class GameController


#endif // SOURCES_GAME_GAMECONTROLLER_H