#include <Graphics/2D/TopDownController.h>
#include <Utilities/Input.h>
#include <Utilities/Logger.h>

namespace Stardust::Graphics::Render2D {

	TopDownController* tp;



	void walkForward(bool, float)
	{
		if(tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_UP)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_UP);
		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { 0.0f, -tp->speed };
	}
	void walkBackward(bool, float)
	{
		if (tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_DOWN)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_DOWN);
		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { 0.0f, tp->speed };
	}
	void walkLeft(bool, float)
	{
		if (tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_LEFT)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_LEFT);

		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { -tp->speed, 0.0f };
	}
	void walkRight(bool, float)
	{
		if (tp->getCharacterSprite()->getFacing() != CHARACTER_FACING_RIGHT)
			tp->getCharacterSprite()->setFacing(CHARACTER_FACING_RIGHT);
		tp->getCharacterSprite()->triggerAnimEvent("walk");
		tp->velocity = { tp->speed, 0.0f };
	}

	void TopDownController::registerHandlers()
	{
		tp = this;
		Utilities::addActionHandler("walkUp", walkForward);
		Utilities::addActionHandler("walkDown", walkBackward);
		Utilities::addActionHandler("walkLeft", walkLeft);
		Utilities::addActionHandler("walkRight", walkRight);
	}

	TopDownController::TopDownController(CharacterSprite* s, float ss) : Controller2D(s)
	{
		speed = ss;
	}
}