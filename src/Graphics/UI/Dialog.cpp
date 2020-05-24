#include <Graphics/UI/Dialog.h>
#include <Utilities/Input.h>
#include <Utilities/Logger.h>


namespace Stardust::Graphics::UI {

	Dialogue::Dialogue(Render2D::Sprite* sprt)
	{
		dialogueBox = sprt;

		main = new MarqueeText("example");
		main->reset();

		g_RenderCore.SetClearColor(0, 0, 0, 255);

		helper = new UIText({ 240, 136 }, "yes");
		helper->setOptions({ 0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_CENTER });

		selPos = 0;
		display = false;
		sel = NULL;
	}

	void Dialogue::reset()
	{
		main->reset();
		display = false;
	}

	void Dialogue::setDialogue(Dialog* d)
	{
		info = d;
		main->setText(d->text);
		selIndex = 0;
		selPos = 0;

		reset();
	}

	void Dialogue::show()
	{
		display = true;
	}

	void Dialogue::hide()
	{
		display = false;
	}
	void Dialogue::update()
	{
		if (display) {
			main->animateStep();

			if (main->isDone()) {
				switch (info->interactionType) {
				case INTERACTION_TYPE_NONE: {
					if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {

						if(sel != NULL)
							sel->Play(5);

						hide();
					}
					break;
				}

				case INTERACTION_TYPE_SELECTION: {
					if (Utilities::KeyPressed(PSP_CTRL_UP)) {
						selPos--;
					}

					if (selPos < 0) {
						selPos = 0;
					}

					if (Utilities::KeyPressed(PSP_CTRL_DOWN)) {
						selPos++;
					}

					if (selPos > (info->choice.size() - 1)) {
						selPos = info->choice.size() - 1;
					}

					if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
						Events::Event* e = new Events::Event();
						e->event_type = selPos;
						info->e(e);
						hide();
					}

				}

				}

			}
		}
	}

	void Dialogue::draw()
	{
		if (display) {

			sceGuEnable(GU_BLEND);
			dialogueBox->Draw();
			main->draw();

			if (info->interactionType == INTERACTION_TYPE_SELECTION) {
				for (int i = 0; i < info->choice.size(); i++) {
					if(i != selPos){
						helper->setOptions({ 0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_CENTER });
					}
					else {
						helper->setOptions({ 0.5f, 0xFF0000FF, INTRAFONT_ALIGN_CENTER });
					}
					helper->setContent(info->choice[i]);
					helper->setPosition(glm::vec2(240, (136 - info->choice.size() * 10) + i *20));
					helper->draw();
				}
			}
		}
	}

	bool Dialogue::isEngaged()
	{
		return display;
	}

	DialogStack::DialogStack(Dialogue* d, int textWrap, std::string usr)
	{
		dial = d;
		username = usr;
		wrap = textWrap;
	}

	std::string textWrap(std::string str, int location) {
		// your other code
		int n = str.rfind(' ', location);
		if (n != std::string::npos) {
			str.at(n) = '\n';
		}
		// your other code
		return str;
	}

	void DialogStack::addDialog(Dialog* d)
	{
		size_t start_pos = d->text.find("[player]");
		if (start_pos != std::string::npos) {
			d->text.replace(start_pos, std::string("[player]").length(), username);
		}

		//Wrap text function
		int minWraps = d->text.size() / wrap;

		for (int i = 0; i < minWraps; i++) {
			d->text = textWrap(d->text, wrap * (i + 1));
		}

		dialogs.push(d);
	}

	void DialogStack::clearDialog()
	{
		for (int i = 0; i < dialogs.size(); i++) {
			dialogs.pop();
		}
	}

	void DialogStack::update()
	{
		if (!dial->isEngaged()) {
			if (dialogs.size() > 0) {
				dial->setDialogue(dialogs.front());
				dial->show();
				dialogs.pop();
			}
		}
	}

}