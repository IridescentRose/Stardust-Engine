#pragma once
#include <string>
#include <fstream>
#include <json/json.h>

namespace Stardust::Utilities::JSON {

	inline Json::Value openJSON(std::string path) {
		Json::Value val;
		std::ifstream file(path);
		file >> val;
		file.close();

		return val;
	}

	inline int getInt(Json::Value v) {
		return v.asInt();
	}
	inline bool getBool(Json::Value v) {
		return v.asBool();
	}
	inline float getFloat(Json::Value v) {
		return v.asFloat();
	}
	inline double getDouble(Json::Value v) {
		return v.asDouble();
	}
	inline std::string getString(Json::Value v) {
		return v.asString();
	}

	/*
		Must check getChildValue() for existence
	*/
	inline Json::Value getChildValue(Json::Value map, std::string key) {
		return map[key.c_str()];
	}

}