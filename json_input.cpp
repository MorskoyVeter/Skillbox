#include "json_input.h"

void JsonInput::SetNodeInput(int size) {
	json::Builder builder;
	std::string id_key = "docid";
	std::string rank_value = "rank";
	std::string requests = "requests000";
	int rank = 0;
	builder.StartDict().Key("answers").StartDict();
	if (answer_.size() > 0) {
		for (int i = 0;i < answer_.size();i++) {
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
			if (answer_[i].size() > 0) {
				builder.Key("result").Value("true").Key("relevance").StartDict();
				for (int j = 0;j < answer_[i].size();j++) {
					if (j == size) {
						break;
					}
					id_key += std::to_string(j);
					rank_value += std::to_string(j);
					builder.Key(id_key).Value(answer_[i][j].first)
						   .Key(rank_value).Value(answer_[i][j].second);
					if (j < 9) {
						id_key.erase(id_key.length() - 1);
						rank_value.erase(rank_value.length() - 1);
					}
					else if (j > 8 && j < 99) {
						id_key.erase(id_key.length() - 2);
						rank_value.erase(rank_value.length() - 1);
					}
					else {
						id_key.erase(id_key.length() - 3);
						rank_value.erase(rank_value.length() - 1);
					}
				}
				builder.EndDict().EndDict();
			}
			else {
				builder.Key("result").Value("false").EndDict();
			}
		}
	}
	
	builder.EndDict().EndDict();
	json::Document doc(builder.Build());
	json::Print(doc, std::cout);
}
