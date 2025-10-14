#include "json_reader.h"
#include <filesystem>
#include <thread>

void Indexfile(std::map<int, std::vector<std::string>>& docs,std::fstream& fstr,int i) {
        while (!fstr.eof()) {
			std::string tempstr;
			fstr >> tempstr;
			docs[i].push_back(tempstr);
		}
   fstr.close();
}

std::map<int, std::vector<std::string>>& ConverterJSON::GetTextDocuments() {
	std::map<std::string, json::Node> mapnode = rut_config_.AsDict();
	std::vector<std::filesystem::path> arr_path;
	if (mapnode["config"].IsDict()) {
		vernode_ = mapnode["config"].AsDict();
	}
	name_ = vernode_["name"].AsString();
	std::string ver = vernode_["version"].AsString();
	if (ver != ver_) {
		std::cout << "Версия не совпадает" << std::endl;
	}
	if (mapnode["files"].IsArray()) {
		for (const auto& arr : mapnode["files"].AsArray()) {
			arr_path.push_back(arr.AsString());
		}
	}
	std::vector<std::thread> vec_thr;
	std::vector<std::fstream> vec_fstr;
	for (int i = 0;i < arr_path.size();i++) {
		vec_fstr.emplace_back(arr_path[i]);
	}
	for (int i = 0;i < arr_path.size();i++) {
		vec_thr.emplace_back(Indexfile, std::ref(docs_), std::ref(vec_fstr[i]),i);
	}
	for (int i = 0;i < arr_path.size();i++) {
		vec_thr[i].join();
	}
	return docs_;
}

int ConverterJSON::GetResponsesLimit() {
	return vernode_["max_responses"].AsInt();
}


