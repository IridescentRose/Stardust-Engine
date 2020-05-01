#include <Graphics/2D/TopDownController.h>
#include <Utilities/Input.h>
#include <Utilities/Logger.h>

namespace Stardust::Graphics::Render2D {

	TopDownController* tp;



	void walkForward(bool, float)
	{
		tp->getCharacterSprite()->triggerAnimEvent("walkUp");
		tp->velocity = { 0.0f, -tp->speed };
	}
	void walkBackward(bool, float)
	{
		tp->getCharacterSprite()->triggerAnimEvent("walkDown");
		tp->velocity = { 0.0f, tp->speed };
	}
	void walkLeft(bool, float)
	{
		tp->getCharacterSprite()->triggerAnimEvent("walkLeft");
		tp->velocity = { -tp->speed, 0.0f };
	}
	void walkRight(bool, float)
	{
		tp->getCharacterSprite()->triggerAnimEvent("walkRight");
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