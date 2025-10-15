#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "json.h"
#include "json_builder.h"

class JsonInput
{
public:
	JsonInput(std::vector < std::vector<std::pair<int, float>>>& answer) : answer_(answer){}
	void SetNodeInput();
private:
std::vector < std::vector<std::pair<int, float>>>& answer_;
};

