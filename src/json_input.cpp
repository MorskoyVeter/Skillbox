#include "json_input.h"

void JsonInput::SetNodeInput() {
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
				builder.Key("result").Value("true").Key("relevance").StartArray();
				for (int j = 0;j < answer_[i].size();j++) {
					builder.StartDict().Key(id_key).Value(answer_[i][j].first)
						   .Key(rank_value).Value(answer_[i][j].second).EndDict();
					
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
	json::Print(doc,answ);
}
