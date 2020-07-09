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
	
	Scene::Scene(std::string file)
	{
		objects.clear();
		loadScene(file);
	}

	Scene::~Scene()
	{
		objects.clear();
		name = "";
	}

	Json::Value makeGameObject(GameObject o) {
		Json::Value res;

		res["UUID"] = o.getUUID();
		res["name"] = o.getName();
		res["layer"] = o.getLayer();

		Json::Value tags(Json::arrayValue);

		for (std::string str : o.getTags()) {
			tags.append(str);
		}

		res["tags"] = tags;

		Json::Value components(Json::arrayValue);

		for (Component comp : o.getComponents()) {
			//TODO: ADD COMPONENTS
		}

		res["components"] = components;

		Json::Value children(Json::arrayValue);

		for (GameObject c : o.child) {
			children.append(makeGameObject(c));
		}

		res["children"] = children;

		return res;
	}

	void makeGameObjects(std::vector<GameObject> objs, Json::Value& vec) {
		for (const GameObject& obj : objs) {
			vec.append(makeGameObject(obj));
		}
	}


	GameObject jsonToGameObject(Json::Value v) {
		GameObject res;

		res.id = v["UUID"].asString();
		res.setName(v["name"].asString());
		res.setLayer(v["layer"].asInt());

		for (int i = 0; i < v["tags"].size(); i++) {
			res.addTag(v["tags"][i].asString());
		}

		//TODO: ADD COMPONENTS

		for (int i = 0; i < v["children"].size(); i++) {
			res.AddChild(jsonToGameObject(v["children"][i]));
		}

		return res;
	}

	void Scene::loadScene(std::string file)
	{
		Json::Value v = Utilities::JSON::openJSON(file);

		name = v["scene"].asString();

		int sz = v["objects"].size();
		for (int i = 0; i < sz; i++) {
			objects.push_back(jsonToGameObject(v["objects"][i]));
		}
	}


	void Scene::saveScene(std::string file)
	{
		std::ofstream f(file);
		Json::Value endResult;

		endResult["scene"] = name;
		Json::Value vec(Json::arrayValue);
		makeGameObjects(objects, vec);
		endResult["objects"] = vec;

		f << endResult << std::endl;
		f.close();
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
