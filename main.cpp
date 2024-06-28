#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "internal/readFile.h"
using namespace std;

vector<string> Lex(const vector<string>& fileContents);
void parse(vector<string> tok);
bool isNumber(const std::string& str);

int main() {
    vector<string> fileContents = readFile();
    vector<string> tokens = Lex(fileContents);

    for (const auto& token : tokens) {
        cout << token << endl;
    }
    //parse(tokens);

    return 0;
}

bool isNumber(const std::string& str) {
    std::regex pattern("^[0-9]+$");
    return std::regex_match(str, pattern);
}

vector<string> Lex(const vector<string>& fileContents) {
    int state = 0;
    string tok = "";
    string String = "";
    string expr = "";
    bool isexpr = false;
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
            } else if (tok == "\n" || tok == "<EOF>") {
                if (expr != "" && isexpr == false) {
                    tokens.push_back("NUM:" + expr);
                    expr = "";
                }
                else if (expr != "" && isexpr == true) {
                    tokens.push_back("EXPR:" + expr);
                    expr = "";
                    isexpr = false;
                }
                tokens.push_back("<EOF>");
                tok = "";
            } else if (tok == "PRINT" || tok == "print") {
                tokens.push_back("print");
                tok = "";
            } else if (isNumber(tok)) {
                expr += tok;
                tok = "";
            } else if (tok == "+" || tok=="-" || tok=="/" || tok=="*") {
                isexpr = true;
                expr += tok;
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
    for (size_t i = 0; i < tok.size(); ++i) {
        if (tok[i] == "print") {
            if (tok[i + 1].substr(0, 6) == "STRING") {
                cout << "STRING:" + tok[i + 1].substr(7, tok[i + 1].size() - 1) << endl;
            } else if (tok[i + 1].substr(0, 3) == "NUM") {
                cout << "NUM:" + tok[i + 1].substr(4, tok[i + 1].size() - 4) << endl;
            } else if (tok[i + 1].substr(0, 4) == "EXPR") {
                cout << "EXPR:" + tok[i + 1].substr(5, tok[i + 1].size() - 5) << endl;
            }
            i+=2;
        }
    }
}
