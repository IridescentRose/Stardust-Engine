#include <Graphics/2D/SideScrollerController.h>
#include <Utilities/Input.h>

namespace Stardust::Graphics::Render2D {
	SideScrollerController::SideScrollerController(CharacterSprite* ss, float s, float g, float j) : Controller2D(ss)
	{
		speed = s;
		gravity = g;
		jump = j;
		isOnGround = false;
		gravityVelocity = 0;
	}
	SideScrollerController* sp;


	void sswalkForward(bool, float)
	{
		if (sp->getCharacterSprite()->getFacing() != CHARACTER_FACING_UP)
			sp->getCharacterSprite()->setFacing(CHARACTER_FACING_UP);
		sp->getCharacterSprite()->triggerAnimEvent("walk");
		
		if(sp->isOnGround){
			sp->velocity = { 0.0f, 0.0f };
			sp->gravityVelocity = sp->jump;	
			sp->isOnGround = false;
		}
	}
	void sswalkBackward(bool, float)
	{
		if (sp->getCharacterSprite()->getFacing() != CHARACTER_FACING_DOWN)
			sp->getCharacterSprite()->setFacing(CHARACTER_FACING_DOWN);
		sp->getCharacterSprite()->triggerAnimEvent("walk");
		sp->velocity = { 0.0f, 0.0f };
	}
	void sswalkLeft(bool, float)
	{
		if (sp->getCharacterSprite()->getFacing() != CHARACTER_FACING_LEFT)
			sp->getCharacterSprite()->setFacing(CHARACTER_FACING_LEFT);

		sp->getCharacterSprite()->triggerAnimEvent("walk");
		sp->velocity = { -sp->speed, 0.0f };
	}
	void sswalkRight(bool, float)
	{
		if (sp->getCharacterSprite()->getFacing() != CHARACTER_FACING_RIGHT)
			sp->getCharacterSprite()->setFacing(CHARACTER_FACING_RIGHT);
		sp->getCharacterSprite()->triggerAnimEvent("walk");
		sp->velocity = { sp->speed, 0.0f };
	}

	void SideScrollerController::registerHandlers()
	{
		sp = this;
		Utilities::addActionHandler("jump", sswalkForward);
		Utilities::addActionHandler("crouch", sswalkBackward);
		Utilities::addActionHandler("walkLeft", sswalkLeft);
		Utilities::addActionHandler("walkRight", sswalkRight);
	}

	void SideScrollerController::setGravity(float g)
	{
		gravity = g;
	}
	void SideScrollerController::setSpeed(float s)
	{
		speed = s;
	}
	void SideScrollerController::setJump(float j)
	{
		jump = j;
	}

	void SideScrollerController::update(float dt)
	{
		glm::vec2 newPos = position + velocity * dt;
		doPhysics(newPos);

		gravityVelocity -= gravity * dt;

		glm::vec2 grav = { 0.0f, -gravityVelocity };
		newPos = position + grav * dt;
		doPhysics(newPos);

		if (position != newPos) {
			gravityVelocity = 0.0f;
			isOnGround = true;

			Utilities::app_Logger->log("HIT GROUND");
		}
		else {
			isOnGround = false;
		}

		setPosition(position);
	}
}