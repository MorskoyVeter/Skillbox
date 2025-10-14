#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "json.h"
#include "json_builder.h"
class JsonInput
{
public:
	JsonInput(std::vector < std::vector<std::pair<int, float>>>& answer) : answer_(answer){}
	void SetNodeInput(int size);
private:
std::vector < std::vector<std::pair<int, float>>>& answer_;
};

