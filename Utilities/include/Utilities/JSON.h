#pragma once
#include <string>
#include <fstream>
#include <json/json.h>

/**
 * A small collection of JSON utilities.
**/
namespace Stardust::Utilities::JSON {

	/**
	 * Returns a jsoncpp value from the file.
	 * Said value is null if the file doesn't exist.
	 * 
	 * @param path - The path to your JSON.
	**/
	inline Json::Value openJSON(std::string path) {
		Json::Value val;
		std::ifstream file(path);
		file >> val;
		file.close();

		return val;
	}
}