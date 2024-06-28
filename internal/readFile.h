#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

vector<string> readFile(){
    std::ifstream in("test.sbs", std::ios_base::in);
    if (!in) {
        cerr << "Error opening the file." << endl;
        return {};
    }
    std::string line;
    vector<string> lines;
    while (getline(in, line)) {
        lines.push_back(line);
    }
    lines.push_back("<EOF>");
    in.close();
    return lines;
}
