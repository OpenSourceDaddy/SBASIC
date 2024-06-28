#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

vector<string> readFile();
vector<string> Lex(const vector<string>& fileContents);
void parse(vector<string> tok);
bool isNumber(const std::string& str);

int main() {
    vector<string> fileContents = readFile();
    vector<string> tokens = Lex(fileContents);
    parse(tokens);

    return 0;
}

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

bool isNumber(const std::string& str) {
    // Define the regex pattern to match exactly one digit from 0 to 9
    std::regex pattern("^[0-9]+$");

    // Check if the string matches the pattern
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
                if(expr != "" && isexpr==true){
                    tokens.push_back("EXPR:"+expr);
                    expr="";
                    //isexpr=false;
                }else if(expr !="" && isexpr==false){
                    tokens.push_back("NUM:"+expr);
                    expr="";
                }
                tok = "";
            } else if (tok == "PRINT" || tok == "print") {
                tokens.push_back("print");
                tok = "";
            }else if(isNumber(tok)){
                //cout << "NUMBER" << endl;
                expr += tok;
                tok="";
            }else if (tok=="+"){
                isexpr = true;
                expr += "+";
                tok="";
            }else if (tok == "\"") {
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
    //cout << expr << endl;
    return tokens;
}

void parse(vector<string> tok) {
    int i = 0;
    while (i < tok.size() - 1) {
        cout << tok[i] + "" + tok[i+1] << endl;
        if((tok[i]+" "+ tok[i+1].substr(0,3))=="PRINT NUM" || (tok[i]+" "+tok[i+1].substr(0,3))=="print NUM"){
            cout << tok[i+1].substr(4,tok[i+1].size()) << endl;
            i+=2;
        }else if((tok[i]+" " +tok[i+1].substr(0,4))=="PRINT EXPR" || (tok[i]+" " +tok[i+1].substr(0,4))=="print EXPR"){
            cout << tok[i+1].substr(5,tok[i+1].size()) << endl;
            i +=2;
        }
        else if ((tok[i] + " " + tok[i + 1].substr(0, 6)) == "PRINT STRING" || (tok[i]+" "+tok[i+1].substr(0,6))=="print STRING") {
            cout << tok[i+1].substr(7,tok[i+1].size()) << endl;
            i += 2;
        }
    }
}