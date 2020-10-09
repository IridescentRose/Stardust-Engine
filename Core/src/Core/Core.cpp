#include <Core/Core.h>

namespace Stardust::Core {
	
	Component::~Component()
	{
	}
	void Component::Init()
	{
	}
	void Component::Update()
	{
	}
	void Component::Draw()
	{
	}

	Scene::Scene()
	{
		name = "Scene";
		objects.clear();
	}
	
	Scene::~Scene()
	{
		objects.clear();
		name = "";
	}

	void Scene::Update()
	{
		for (GameObject obj : objects) {
			obj.Update();
		}
	}
	void Scene::Draw()
	{
		for (GameObject obj : objects) {
			obj.Draw();
		}
	}

	void Scene::AddObject(GameObject& obj)
	{
		objects.push_back(obj);
	}
	void Scene::ClearObjects()
	{
		objects.clear();
	}
}
