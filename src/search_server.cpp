#include "search_server.h"
#include <algorithm>
#include <iostream>

std::vector<std::string> SplitIntoWord(const std::string& str) {
std::istringstream my_streamvv(str);
std::string strtemp;
std::vector<std::string> splitvec;
while (!my_streamvv.eof()) {
	my_streamvv >> strtemp;
	splitvec.push_back(strtemp);
}
return splitvec;
}

void SearchServer::AddRequests(json::Node rut) {
	std::map<std::string, json::Node> req=rut.AsDict();
	std::vector<json::Node> vec_str = req["requests"].AsArray();
	for (int i = 0;i < vec_str.size();i++) {
		std::vector<std::string> str;
		str = SplitIntoWord(vec_str[i].AsString());
		if (str.size() > 10) {
			std::cout << "string in "<<i+1<<" requests more 10 words" << std::endl;
		}
		requests_[i].insert(str.begin(), str.end());
		if (i > 1000) {
			std::cout << "exceeded limit 1000 requests" << std::endl;
		}
	}
	
}

void SearchServer::FindDocument() {
	for (const auto& [key, var] : requests_) {
		std::unordered_map<int, float> summrel{};
		for (const auto& str : var) {
			if (index_.word_to_document_freqs_.count(str) == 0) {
				continue;
			}
			for (int i = 0;i < index_.word_to_document_freqs_[str].size();i++) {
				if (i == size_) {
					break;
				}
				summrel[index_.word_to_document_freqs_[str][i].doc_id]
					+= index_.word_to_document_freqs_[str][i].relev;
			}
		}
		std::vector<std::pair<int, float>> ans;
		for (auto it = summrel.begin();it != summrel.end();it++) {
			ans.push_back(*it);
		}
		std::sort(ans.begin(), ans.end(), [](const auto& lhs, const auto& rhs) {
			if (lhs.second != rhs.second) {
				return lhs.second > rhs.second;
			}
			else {
				return lhs.first > rhs.first;
			}
			});
	answer_vec_.emplace_back(ans);
	}
}

InvertedIndex::InvertedIndex(std::map<int, std::vector<std::string>>& docs) : docs_(docs) {
	for (int i = 0;i < docs.size();i++) {
		const float inv_word_count = 1.0 / docs[i].size();
		entry entr;
		entr.doc_id = i;
		entr.relev = inv_word_count;
		for (const std::string& word : docs[i]) {
			if (word_to_document_freqs_[word].size() == 0) {
				word_to_document_freqs_[word].push_back(entr);
			}
			else {
				if (word_to_document_freqs_[word].back().doc_id != i) {
					entr.doc_id = i;
					entr.relev = inv_word_count;
					word_to_document_freqs_[word].push_back(entr);
				}
				else {
					word_to_document_freqs_[word].back().relev += inv_word_count;
				}
			}
		}
	}
}