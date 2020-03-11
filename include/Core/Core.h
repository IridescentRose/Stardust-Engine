#pragma once
#include <vector>
#include <Utilities/UUID.h>

namespace Stardust::Core {

	class Component {
	public:
		Component();
		~Component();

		void Init();
		void Update();
		void Draw();

	};

	enum DefaultLayers {
		LAYER_DEFAULT = 0,
	};

	class GameObject {
	public:
		GameObject() {
			id = Utilities::generateUUID();
			name = "GameObject";
			tags.clear();
			layer = LAYER_DEFAULT;
		}

		inline Utilities::UUID getUUID() {
			return id;
		}
		inline void setLayer(int l) {
			layer = l;
		}
		inline int getLayer() {
			return layer;
		}
		inline void setName(std::string n) {
			name = n;
		}
		inline std::string getName() {
			return name;
		}

		inline void addTag(std::string tag) {
			tags.push_back(tag);
		}
		inline void clearTag() {
			tags.clear();
		}
		inline bool hasTag(std::string name) {
			for (int i = 0; i < tags.size(); i++) {
				if (tags[i] == name) {
					return true;
				}
			}
			return false;
		}
		inline std::vector<std::string> getTags() {
			return tags;
		}

		inline void addComponents(Component cmp) {
			components.push_back(cmp);
		}
		inline void clearComponents() {
			components.clear();
		}
		inline std::vector<Component> getComponents() {
			return components;
		}

		inline void Update() {
			for (int i = 0; i < components.size(); i++) {
				components[i].Update();
			}
		}
		inline void Draw() {
			for (int i = 0; i < components.size(); i++) {
				components[i].Draw();
			}
		}

	private:
		std::string name;
		std::vector<std::string> tags;
		std::vector<Component> components;
		int layer;
		Utilities::UUID id;
	};
}