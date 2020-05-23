#include <Graphics/UI/MarqueeText.h>


namespace Stardust::Graphics::UI{
MarqueeText::MarqueeText(std::string text)
{
	ui = new UI::UIText({ 0, 0 }, "");
	setText(text);
}

MarqueeText::~MarqueeText()
{
	delete ui;
}

void MarqueeText::setPosition(glm::vec2 position)
{
	ui->setPosition(position);
}

void MarqueeText::setSpeed(int s)
{
	speed = s;
}

void MarqueeText::setText(std::string t)
{
	ui->setContent(t);
	txt = t;
	index = t.length();
}

void MarqueeText::reset()
{
	index = 0;
}

int count = 0;

void MarqueeText::animateStep()
{
	count++;
	if (count % speed == 0) {
		index++;
	}

	if (index > txt.size()) {
		index = txt.size();
	}
}

void MarqueeText::draw()
{
	ui->setContent(txt.substr(0, index));
	ui->draw();
}

void MarqueeText::setOptions(UI::FontStyle fs)
{
	ui->setOptions(fs);
}

}