#include "json_reader.h"
#include <filesystem>
#include <thread>
#include <mutex>
#include <iostream>


void Indexfile(std::map<int, std::vector<std::string>>& docs,std::fstream& fstr,int i, std::mutex& mutex_object) {
	    
	if (fstr.eof()) {
		    std::cout << "File " << i << " empty" << std::endl;
	    }
	    while (!fstr.eof()) {
			std::string tempstr;
			fstr >> tempstr;
			if (tempstr.length() > 100) {
				std::cout << tempstr << " lenght more 100 charasters" << std::endl;
			}
			else if (!std::all_of(tempstr.begin(), tempstr.end(), islower)) {
				std::cout << tempstr << " contains is not lower character" << std::endl;
			}
			mutex_object.lock();
			docs[i].push_back(tempstr);
			mutex_object.unlock();
		}
		if (docs[i].size() > 1000) {
			std::cout << "In Document "<<i<<" exceeded limit 1000 words" << std::endl;
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
	std::cout << "Starting " <<"\""<<name_ <<"\""<< std::endl;
	std::string ver = vernode_["version"].AsString();
	if (ver != ver_) {
		std::cout <<name_<<" has incorrect file version" << std::endl;
	}
	if (mapnode["files"].IsArray()) {
		for (const auto& arr : mapnode["files"].AsArray()) {
			arr_path.push_back(arr.AsString());
		}
	}
	std::vector<std::thread> vec_thr;
	std::vector<std::fstream> vec_fstr;
	std::mutex mutex_object;
	for (int i = 0;i < arr_path.size();i++) {
		vec_fstr.emplace_back(arr_path[i]);
	}
	for (int i = 0;i < arr_path.size();i++) {
		vec_thr.emplace_back(Indexfile, std::ref(docs_), std::ref(vec_fstr[i]),i, std::ref(mutex_object));
	}
	for (int i = 0;i < arr_path.size();i++) {
		vec_thr[i].join();
	}
	return docs_;
}

int ConverterJSON::GetResponsesLimit() {
	return vernode_["max_responses"].AsInt();
}


