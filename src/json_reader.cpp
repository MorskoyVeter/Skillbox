#include "search_server.h" 
#include <thread>
#include <mutex>
#include <iostream>

std::vector<std::string> ConverterJSON::GetTextDocuments() {
	std::map<std::string, json::Node> mapnode = config_.AsDict();
	std::map<std::string, json::Node> vernode;
	std::vector<std::string> arr_path;
	if (mapnode["config"].IsDict()) {
		vernode = mapnode["config"].AsDict();
	}
	else {
		std::cout << "config file is empty" << std::endl;
		throw "config file is empty";
	}
	std::string name_ = vernode["name"].AsString();
	std::cout << "Starting " << "\"" << name_ << "\"" << std::endl;
	std::string ver = vernode["version"].AsString();
	if (ver != "0.1") {
		std::cout << name_ << " config.json has incorrect file version." << std::endl;
	}
	max_responses_ = vernode["max_responses"].AsInt();
	if (mapnode["files"].IsArray()) {
		for (const auto& arr : mapnode["files"].AsArray()) {
			arr_path.push_back(arr.AsString());
		}
	}
	return arr_path;
}

int ConverterJSON::GetResponsesLimit() {
	return max_responses_;
}

std::vector<std::string> ConverterJSON::GetRequests() {
	std::map<std::string, json::Node> reqest = requests_.AsDict();
	std::vector<json::Node> arr_req=reqest["requests"].AsArray();
	if (arr_req.size() < 1000) {
		std::vector<std::string> reque(arr_req.size());
		for (int i = 0;i < arr_req.size();i++) {
			reque[i] = arr_req[i].AsString();
		}

		return reque;
	}
	else {
		std::vector<std::string> reque(1000);
		for (int i = 0;i < 1000;i++) {
			reque[i] = arr_req[i].AsString();
		}
		return reque;
	}
}


void Indexfile( std::vector<std::string>& docs, std::fstream& fstr, int i, std::mutex& mutex_object) {

	if (!fstr.is_open()) {
		std::cout << "file " << i << " not open" << std::endl;
		return;
	}
	std::string pushstr;
	int j = 1000;
	while (!fstr.eof() && j!=0) {
		std::string tempstr;
		fstr >> tempstr;
		if (tempstr.length() > 99) {
			tempstr.resize(100);
		}
		if (!std::all_of(tempstr.begin(), tempstr.end(), islower)) {
			std::cout << tempstr << " not a-z";
		}
		mutex_object.lock();
			pushstr += tempstr + " ";
			j--;
		mutex_object.unlock();
	}
	docs[i] = pushstr;
	fstr.close();
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
	std::vector<std::thread> vec_thr;
	std::vector<std::fstream> vec_fstr;
	std::mutex mutex_object;
	for (int i = 0;i < input_docs.size();i++) {
		vec_fstr.emplace_back(input_docs[i]);
	}
	docs_.resize(vec_fstr.size());
	for (int i = 0;i < input_docs.size();i++) {
		vec_thr.emplace_back(Indexfile, std::ref(docs_), std::ref(vec_fstr[i]), i, std::ref(mutex_object));
	}
	for (int i = 0;i < input_docs.size();i++) {
		vec_thr[i].join();
	}
	for (int i = 0;i < docs_.size();i++) {
		std::vector<std::string> dtr_temp;
		std::istringstream shows(docs_[i]);
		std::string str;
		while (shows >> str) {
			dtr_temp.push_back(str);
		}
		dtr_size_.push_back(dtr_temp.size());
		std::map<std::string, int> tmap;
		for (int j = 0;j < dtr_temp.size();j++) {
			tmap[dtr_temp[j]]++;
		}
		for (const auto& [key,value]:tmap) {
			Entry entry;
			entry.count = value;
			entry.doc_id = i;
			freq_dictionary_[key].push_back(entry);
		}
	}
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
	if (freq_dictionary_.count(word)) {
		return freq_dictionary_[word];
	}
	return {};
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
	json::Builder builder;
	std::string id_key = "docid";
	std::string rank_value = "rank";
	std::string requests = "requests000";
	int rank = 0;
	builder.StartDict().Key("answers").StartDict();
	if (answers.size() > 0) {
		for (int i = 0;i < answers.size();i++) {
			if (i < 9) {
				requests.erase(requests.length() - 1);
				requests += std::to_string(i + 1);
			}
			else if (i > 8 && i < 99) {
				requests.erase(requests.length() - 2);
				requests += std::to_string(i + 1);
			}
			else {
				requests.erase(requests.length() - 3);
				requests += std::to_string(i + 1);
			}
			builder.Key(requests).StartDict();
			if (answers[i].size() > 0) {
				builder.Key("result").Value("true").Key("relevance").StartArray();
				for (int j = 0;j < answers[i].size()&&j < max_responses_;j++) {
					builder.StartDict().Key(id_key).Value(answers[i][j].first)
						.Key(rank_value).Value(answers[i][j].second).EndDict();

				}
				builder.EndArray().EndDict();
			}
			else {
				builder.Key("result").Value("false").EndDict();
			}
		}
	}

	builder.EndDict().EndDict();
	json::Document doc(builder.Build());
	std::ofstream answ("./answer.json");
	json::Print(doc, answ);
}