#pragma once
#include <Graphics/UI/UIText.h>
#include <Graphics/2D/SpriteAdvanced.h>

namespace Stardust::Graphics::UI{

class MarqueeText {
public:
	MarqueeText(std::string text);
	~MarqueeText();

	void setPosition(glm::vec2 position);
	void setSpeed(int s);

	void setText(std::string);
	void reset();

	bool isDone() {
		return index == txt.size();
	}

	void animateStep();
	void draw();

	void setOptions(FontStyle fs);

private:

	UIText* ui;
	
	std::string txt;
	int index;
	int speed;

};
}