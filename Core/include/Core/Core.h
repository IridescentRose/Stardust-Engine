#pragma once
#include <Utilities/UUID.h>
#include <Utilities/JSON.h>
#include <vector>
#include <glm/glm.hpp>
#include <Math/Matrix.h>

/**
 * UNSTABLE
**/
namespace Stardust::Core {

	struct Transform {
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;

		glm::mat4 toModelMatrix() {
			return Math::makeTransformMatrix(pos, rot, scale);
		}
	};

	class GameObject;

	class Component {
	public:
		virtual ~Component();

		virtual void Init();
		virtual void Update();
		virtual void Draw();

		GameObject* parent;
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
			transform = {
				{0, 0, 0},
				{0, 0, 0},
				{1, 1, 1},
			};
		}

		GameObject(Utilities::UUID i) {
			id = i;
			name = "GameObject";
			tags.clear();
			layer = LAYER_DEFAULT;
			parent = NULL;
			child.clear();
			transform = {
				{0, 0, 0},
				{0, 0, 0},
				{1, 1, 1},
			};
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
			cmp.parent = this;
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

		inline void setTransform(Transform trans) {
			transform = trans;
		}

		inline Transform getTransform() {
			return transform;
		}

		inline GameObject* getParent() {
			return parent;
		}
		inline void setParent(GameObject* obj) {
			parent = obj;
		}

		std::vector<GameObject> child;

	private:
		GameObject* parent;
		Utilities::UUID id;
		Transform transform;
		std::string name;
		std::vector<std::string> tags;
		std::vector<Component> components;
		int layer;
	};

	class Scene {
	public:
		Scene();
		~Scene();

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