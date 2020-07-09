#pragma once
#include "StateManager.h"

namespace Stardust::Core{

class GameStateManager;

class GameState {
public:
	virtual ~GameState();
	virtual void init() = 0;
	virtual void cleanup() = 0;
	
	virtual void enter() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void update(GameStateManager* st) = 0;
	virtual void draw(GameStateManager* st) = 0;
};

}