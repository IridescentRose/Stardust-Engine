#pragma once
#include <Utilities/JSON.h>
#include <Graphics/2D/SpriteBase.h>
#include <string>
#include <map>

namespace Stardust::Graphics::UI {
	
	struct Anchor {
		int x, y;
	};

#define MIN_X 0
#define MAX_X 480
#define MIN_Y 0
#define MAX_Y 272
#define CENTER_X 240
#define CENTER_Y 136

#define TOP_LEFT_ANCHOR {MIN_X, MIN_Y}
#define TOP_RIGHT_ANCHOR {MAX_X, MIN_Y}
#define TOP_CENTER_ANCHOR {CENTER_X, MIN_Y}

#define CENTER_LEFT_ANCHOR {MIN_X, CENTER_Y}
#define CENTER_RIGHT_ANCHOR {MAX_X, CENTER_Y}
#define CENTER_CENTER_ANCHOR {CENTER_X, CENTER_Y}

#define BOT_LEFT_ANCHOR {MIN_X, MAX_Y}
#define BOT_RIGHT_ANCHOR {MAX_X, MAX_Y}
#define BOT_CENTER_ANCHOR {CENTER_X, MAX_Y}

	struct Offset {
		int x, y;
	};

	struct Extent {
		int x, y;
	};

	struct BaseColor {
		char r, g, b, a;
	};

	typedef Render2D::Sprite Image;

	typedef std::string ActionName;

	typedef void (*ActionListener)();
	
	class UIElement {
	public:
		UIElement() {
			anchor = CENTER_CENTER_ANCHOR;
			position = { 0, 0 };
			size = { 100, 100 };
			image = NULL;
			parent = NULL;
			children.clear();
			enabled = true;
		}
		UIElement(UIElement& p) {
			anchor = { p.anchor.x + p.position.x, p.anchor.y + p.position.y };
			position = { 0, 0 };
			size = { 100, 100 };
			image = NULL;
			parent = &p;
			children.clear();
			enabled = true;
		}
		~UIElement() {
			clearChildren();
		}

		Anchor anchor;
		Offset position;
		BaseColor color;
		Image* image;
		Extent size;
		bool enabled;

		void addChildren(UIElement* ui) {
			ui->parent = this;
			children.push_back(ui);
		}

		void clearChildren() {
			for (auto e : children) {
				delete e;
			}
			children.clear();
		}

		UIElement* parent;
		std::vector<UIElement*> children;

		virtual void Draw() = 0;
		virtual void Update() = 0;

	protected:
		virtual void loadFromJson(Json::Value v) = 0;
	};

	class Canvas {
	public:
		Canvas() {
			uiElements.clear();
		}
		~Canvas() {
			for (auto e : uiElements) {
				delete e;
			}
			uiElements.clear();
		}

		inline void Update() {
			auto f = [](auto&& self, std::vector<UIElement*> elements) -> void
			{ 
				for (auto e : elements) {
					e->Update();

					if (e->children.size() > 0) {
						self(self, e->children);
					}
				}
			};
			f(f, uiElements);
		}
		inline void Draw() {
			auto f = [](auto&& self, const std::vector<UIElement*>& elements) -> void
			{
				for (auto e : elements) {
					e->Draw();

					if (e->children.size() > 0) {
						self(self, e->children);
					}
				}
			};
			f(f, uiElements);
		}

		inline void AddUIElement(UIElement* element) {
			uiElements.push_back(element);
		}

		inline void ClearUIElements() {
			uiElements.clear();
		}

		inline void addListener(std::string name, ActionListener listener) {
			listeners.emplace(name, listener);
		}

		inline void clearListeners() {
			listeners.clear();
		}

		inline void handleAction(std::string name) {
			if (listeners.find(name) != listeners.end()) {
				listeners[name]();
			}
		}

	private:
		std::vector<UIElement*> uiElements;
		std::map<std::string, ActionListener> listeners;
	};

	class UIBase : public UIElement {
	public:
		UIBase() {
			type = "undefined";
		}
		~UIBase() {

		}

	protected:
		inline virtual void loadFromJson(Json::Value v) override {
			anchor = { v["anchor"]["x"].asInt(), v["anchor"]["y"].asInt() };
			position = { v["position"]["x"].asInt(), v["position"]["y"].asInt() };
			size = { v["size"]["x"].asInt(), v["size"]["y"].asInt() };
			type = v["type"].asString();
			color = { v["color"]["red"].asInt() ,v["color"]["green"].asInt() , v["color"]["blue"].asInt(), v["color"]["alpha"].asInt() };
		}

		std::string type;

	public:
		inline void load(Json::Value v) {
			loadFromJson(v);
		}
	};

	extern Canvas* g_Canvas;

}