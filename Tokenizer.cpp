#include "Tokenizer.h"
using namespace std;
vector<string> tokenize(string command){
    vector<string> tokens;
    string current = "";
    bool insideString = false;
    for (size_t i = 0; i < command.length(); i++) {
        char c = command[i];
        if(c == '\'') {
            if(!insideString && current != ""){
                tokens.push_back(current);
                current = "";
            }
            current += c;
            if(insideString){
                tokens.push_back(current);
                current = "";
                insideString = false;
            }
            else{
                insideString = true;
            }
            continue;
        }
        if(insideString){
            current += c;
            continue;
        }
        if(c == ' ' || c == '\t'){
            if (current != "") {
                tokens.push_back(current);
                current = "";
            }
            continue;
        }
        if (c == ',' || c == ';' || c == '(' ||
            c == ')' || c == '*') {
            if (current != "") {
                tokens.push_back(current);
                current = "";
            }
            tokens.push_back(string(1, c));
            continue;
        }
        if(c == '=' || c == '<' || c == '>' || c == '!'){
            if (current != "") {
                tokens.push_back(current);
                current = "";
            }
            string op = "";
            op += c;
            if (i + 1 < command.length()) {
                char next = command[i + 1];
                if (next == '=' || (c == '<' && next == '>')) {
                    op += next;
                    i++;
                }
            }
            tokens.push_back(op);
            continue;
        }
        current += c;
    }
    if(current != ""){
        tokens.push_back(current);
    }
    return tokens;
}
