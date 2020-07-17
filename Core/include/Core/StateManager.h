#pragma once
#include <vector>
#include "GameState.h"


namespace Stardust::Core {

	class GameState;

	class GameStateManager {
	public:
		GameStateManager();

		inline bool isRunning() {
			return running;
		}

		void changeState(GameState* state);
		void addState(GameState* state);
		void popState();

		void update();
		void draw();

	private:
		std::vector<GameState*> states;
		bool running;
	};

}