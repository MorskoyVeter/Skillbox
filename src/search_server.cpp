#include "search_server.h"
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
	std::vector<std::set<std::string>> unig_requests;
	for (auto& str : queries_input) {
		std::istringstream my_streamvv(str);
		std::string strtr;
		unig_requests.push_back({});
		int x = 0;
		while(my_streamvv>>strtr && x<10){
			unig_requests.back().insert(strtr);
			if (!std::all_of(strtr.begin(), strtr.end(), islower)) {
				std::cout << strtr << " not a-z"<<std::endl;
			}
			++x;
		}
	}
	std::vector<std::vector<std::pair<int, std::string>>> req_sort_sum(unig_requests.size());
	std::vector<std::map<size_t, size_t>> m_coun(unig_requests.size());
	std::vector<Entry> ent;
	for (int i = 0;i < unig_requests.size();i++) {
		auto it = unig_requests[i].begin();
		size_t siz = 0;
		for (;it != unig_requests[i].end();it++) {
			ent = index_.GetWordCount(*it);
			if (ent.empty()) {
				continue;
			}
			size_t sum = 0;
			for (const auto& cn : ent) {
				sum += cn.count;
			}
			req_sort_sum[i].push_back({ sum,*it });
		}
		std::sort(req_sort_sum[i].begin(), req_sort_sum[i].end(), [](const auto& lhs,const auto& rhs) {
			return lhs.first < rhs.first;
		});
		for (int j = 0;j < req_sort_sum[i].size();j++) {
			ent = index_.GetWordCount(req_sort_sum[i][j].second);
			for (auto v : ent) {
				m_coun[i][v.doc_id] += v.count;
			}
        }
	}
	RelativeIndex r_index;
	std::vector<std::vector<RelativeIndex>> rel_ind(unig_requests.size());
	for (size_t j = 0;j < rel_ind.size();j++) {
		for (const auto& [key,value]:m_coun[j]) {
			r_index.doc_id = key;
			r_index.rank = value/ (float)index_.GetSize()[key];
				rel_ind[j].push_back(r_index);
		}
		std::sort(rel_ind[j].begin(), rel_ind[j].end(), [](const auto& lhs, const auto& rhs) {
			if (lhs.rank == rhs.rank) {
				return lhs.doc_id < rhs.doc_id;
			}
			return lhs.rank > rhs.rank;
			});
	}
	return rel_ind;
}