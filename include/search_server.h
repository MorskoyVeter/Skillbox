#include "json_reader.h"
#include <set>
struct RelativeIndex {
	size_t doc_id;
	float rank;
	bool operator ==(const RelativeIndex& other) const {
		return (doc_id == other.doc_id && rank == other.rank);
	}
};
class SearchServer {
public:

	SearchServer(InvertedIndex& idx) : index_(idx) {};
	
	std::vector<std::vector<RelativeIndex>> search(const
		std::vector<std::string>& queries_input);
private:
	InvertedIndex& index_;
};