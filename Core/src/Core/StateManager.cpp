#include <Core/StateManager.h>

namespace Stardust::Core{
GameStateManager::GameStateManager()
{
	running = true;
	states.clear();
}

void GameStateManager::changeState(GameState* state)
{
	if (states.size() > 0) {
		for (auto s : states) {
			s->cleanup();
			delete s;
		}
	}

	states.push_back(state);
	states.back()->enter();
}

void GameStateManager::addState(GameState* state)
{
	if (states.size() > 0) {
		states.back()->pause();
	}

	states.push_back(state);
	states.back()->enter();
}

void GameStateManager::popState()
{
	if (states.size() > 0) {
		states.back()->cleanup();
		delete states.back();
		states.pop_back();
		
		if (states.size() > 0) {
			states.back()->resume();
		}
	}
}

void GameStateManager::update()
{
	if(states.size() > 0)
		states.back()->update(this);
}

void GameStateManager::draw()
{
	if (states.size() > 0)
		states.back()->draw(this);
}
}