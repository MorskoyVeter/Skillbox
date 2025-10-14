#pragma once
#include <sstream>
#include <string>
#include <vector>
#include "json.h"
#include <map>
#include <set>
#include <unordered_map>
struct entry {
	size_t doc_id{};
	float relev{};
};

class InvertedIndex {
public:
	InvertedIndex(std::map<int, std::vector<std::string>>& docs);
	std::map<int, std::vector<std::string>>& docs_;
	std::map<std::string, std::vector<entry>> word_to_document_freqs_;
};

class SearchServer
{
public:
	SearchServer() = default;
	SearchServer(InvertedIndex& index) : index_(index){}
	void AddRequests(json::Node rut);
	void FindDocument();
	std::vector < std::vector<std::pair<int, float>>>& GetAnswer() {
		return answer_vec_;
	}
private:
	InvertedIndex index_;
	std::map<int,std::set<std::string>> requests_;
	std::vector < std::vector<std::pair<int, float>>> answer_vec_;
};

