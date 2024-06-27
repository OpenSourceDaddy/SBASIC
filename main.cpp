#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> readFile();
vector<string> Lex(const vector<string>& fileContents);
void parse(vector<string> tok);

int main() {
    vector<string> fileContents = readFile();
    vector<string> tokens = Lex(fileContents);
    parse(tokens);

    return 0;
}

vector<string> readFile(){
    std::ifstream in("test.klx", std::ios_base::in);
    if (!in) {
        cerr << "Error opening the file." << endl;
        return {};
    }
    std::string line;
    vector<string> lines;
    while (getline(in, line)) {
        lines.push_back(line);
    }
    in.close();
    return lines;
}

vector<string> Lex(const vector<string>& fileContents) {
    int state = 0;
    string tok = "";
    string String = "";
    vector<string> tokens;

    for (const auto& line : fileContents) {
        for (const auto& ch : line) {
            tok += ch;
            if (tok == " ") {
                if (state == 0) {
                    tok = "";
                } else {
                    tok = " ";
                }
            } else if (tok == "\n") {
                tok = "";
            } else if (tok == "PRINT" || tok == "print") {
                tokens.push_back("print");
                tok = "";
            } else if (tok == "\"") {
                if (state == 0) {
                    state = 1;
                } else if (state == 1) {
                    tokens.push_back("STRING:" + String + "\"");
                    String = "";
                    state = 0;
                    tok = "";
                }

            } else if (state == 1) {
                String += tok;
                tok = "";
            }
        }
    }

    return tokens;
}

void parse(vector<string> tok) {
    int i = 0;
    while (i < tok.size() - 1) {
        if ((tok[i] + " " + tok[i + 1].substr(0, 6)) == "PRINT STRING" || (tok[i]+" "+tok[i+1].substr(0,6))=="print STRING") {
            cout << "Found String" << endl;
            i += 2;
        } else {
            i+=2;
        }
    }
}
