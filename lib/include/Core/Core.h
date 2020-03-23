#pragma once
#include <vector>
#include <Utilities/UUID.h>
#include <Utilities/JSON.h>

namespace Stardust::Core {

	class Component {
	public:
		Component();
		~Component();

		virtual void Init();
		virtual void Update();
		virtual void Draw();

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
			parent = NULL;
			child.clear();
		}

		GameObject(Utilities::UUID i) {
			id = id;
			name = "GameObject";
			tags.clear();
			layer = LAYER_DEFAULT;
			parent = NULL;
			child.clear();
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

			for (GameObject obj : child) {
				obj.Update();
			}
		}
		inline void Draw() {
			for (int i = 0; i < components.size(); i++) {
				components[i].Draw();
			}

			for (GameObject obj : child) {
				obj.Draw();
			}
		}

		inline void AddChild(GameObject c) {
			child.push_back(c);
			child[0].parent = this;
		}
		inline void ClearChild() {
			child.clear();
		}

		GameObject* parent;
		std::vector<GameObject> child;

		Utilities::UUID id;
	private:
		std::string name;
		std::vector<std::string> tags;
		std::vector<Component> components;
		int layer;
	};

	class Scene {
	public:
		Scene();
		Scene(std::string file);
		~Scene();

		void loadScene(std::string file);
		void saveScene(std::string file);

		void Update();
		void Draw();

		void AddObject(GameObject& obj);
		void ClearObjects();

		inline void setName(std::string n) {
			name = n;
		}

		inline std::string getName() {
			return name;
		}

	private:
		std::string name;
		std::vector<GameObject> objects;
	};

}