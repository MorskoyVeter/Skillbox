#include "json_reader.h"
#include "search_server.h"
#include "json_input.h"

using namespace std;


int main() {
    string  str1, str2;
    ifstream shows_file("C:/Prcommun/FinalProject/config.json");
    if (!shows_file) {
        cout << "error" << endl;
    }
    while (getline(shows_file, str1)) {
        str2 += str1;
    }
    shows_file.close();
    istringstream my_streamvv_1(str2);

    json::Document doc_1 = json::Load(my_streamvv_1);
    json::Node rut_1 = doc_1.GetRoot();
    shows_file.open("C:/Prcommun/FinalProject/requests.json");
    str1 = "";
    str2 = "";
    if (!shows_file) {
        cout << "error" << endl;
    }
    while (getline(shows_file, str1)) {
        str2 += str1;
    }
    shows_file.close();
    istringstream my_streamvv_2(str2);
    json::Document doc_2 = json::Load(my_streamvv_2);
    ConverterJSON convJson(rut_1);
    InvertedIndex ind(convJson.GetTextDocuments());
    SearchServer search(ind);
    search.AddRequests(doc_2.GetRoot());
    search.FindDocument();
    JsonInput inf( search.GetAnswer());
    inf.SetNodeInput(convJson.GetResponsesLimit());
}
