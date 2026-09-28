#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>
#include <vector>
using namespace std;
enum class TokenType{
    Identifier,
    Number,
    String,
    Operator,
    Symbol,
    EndOfFile
};
struct Token{
    TokenType type;
    std::string value;
};
class Tokenizer{
public:
    vector<Token> tokenize(const string& input);
private:
    bool isIdentifierStart(char c);
    bool isIdentifierChar(char c);
    bool isNumberStart(char c);
    Token readIdentifier(const string& input, size_t& position);
    Token readNumber(const string& input, size_t& position);
    Token readString(const string& input, size_t& position);
    Token readOperator(const string& input, size_t& position);
};
#endif