#pragma once
#include "json.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm> 
#include "json_builder.h"
class ConverterJSON {
public:
	ConverterJSON() = default;
	ConverterJSON(const json::Node& rut_1, const json::Node& rut_2) : config_(rut_1), requests_(rut_2){}
	
	std::vector<std::string> GetTextDocuments();
	int GetResponsesLimit();
	std::vector<std::string> GetRequests();
	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
private:
	const json::Node& config_;
	const json::Node& requests_;
	int max_responses_{};
};

struct Entry {
	size_t doc_id, count;
	// Данный оператор необходим для проведения тестовых сценариев
	bool operator ==(const Entry& other) const {
		return (doc_id == other.doc_id &&
			count == other.count);
	}
};

class InvertedIndex {
public:
	InvertedIndex() = default;
	void UpdateDocumentBase(std::vector<std::string> input_docs);
	std::vector<Entry> GetWordCount(const std::string& word);
	std::vector<size_t>& GetSize() {
		return dtr_size_;
	}
private:
	std::vector<std::string> docs_; // список содержимого документов
	std::vector<size_t> dtr_size_;
	std::map<std::string, std::vector<Entry>> freq_dictionary_;
};