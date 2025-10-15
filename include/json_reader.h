#pragma once
#include "json.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm> 
class ConverterJSON {
public:
	ConverterJSON() = default;
	ConverterJSON(json::Node& rut_1) : rut_config_(rut_1){}
	
	std::map<int, std::vector<std::string>>& GetTextDocuments();
	
	int GetResponsesLimit();

private:
	const std::string ver_{ "0.1" };
	std::string name_;
	json::Node& rut_config_;
	std::map<std::string, json::Node> vernode_;
	std::map<int, std::vector<std::string>> docs_;
};