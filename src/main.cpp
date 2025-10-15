#include "json_reader.h"
#include "search_server.h"
#include "json_input.h"
#include <string_view>

using namespace std;

int main() {
    string  str1="", str2="";
    string name_config = "config.json";
    string name_requests = "requests.json";
    ifstream shows_file("./config.json");
    if (!shows_file) {
        throw "File config.json is missing";
    }
    while (getline(shows_file, str1)) {
        str2 += str1;
    }
    if (str2 == "") {
        cout << name_config << " empty" << std::endl;
    }
    shows_file.close();
    istringstream my_streamvv_1(str2);

    json::Document doc_1 = json::Load(my_streamvv_1);
    json::Node rut_1 = doc_1.GetRoot();
    shows_file.open("./requests.json");
    str1 = "";
    str2 = "";
    if (!shows_file) {
        throw "File requests.json is missing";
    }
    while (getline(shows_file, str1)) {
        str2 += str1;
    }
    if (str2 == "") {
        cout << name_requests << " empty" << std::endl;
    }
    shows_file.close();
    istringstream my_streamvv_2(str2);
    json::Document doc_2 = json::Load(my_streamvv_2);
    ConverterJSON convJson(rut_1);
    InvertedIndex ind(convJson.GetTextDocuments());
    SearchServer search(ind, convJson.GetResponsesLimit());
    search.AddRequests(doc_2.GetRoot());
    search.FindDocument();
    JsonInput inf(search.GetAnswer());
    inf.SetNodeInput();
}
