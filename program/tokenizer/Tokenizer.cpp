#include "Tokenizer.h"
#include <cctype>
#include <stdexcept>
using namespace std;
bool Tokenizer::isIdentifierStart(char c) {
    return  isalpha(static_cast<unsigned char>(c)) || c == '_';
}
bool Tokenizer::isIdentifierChar(char c) {
    return  isalnum(static_cast<unsigned char>(c)) || c == '_';
}
bool Tokenizer::isNumberStart(char c) {
    return  isdigit(static_cast<unsigned char>(c));
}
Token Tokenizer::readIdentifier(
    const  string& input,
    size_t& position
){
    size_t start = position;
    while (
        position < input.length() &&
        isIdentifierChar(input[position])
    ) {
        position++;
    }
    return {
        TokenType::Identifier,
        input.substr(start, position - start)
    };
}
Token Tokenizer::readNumber(
    const  string& input,
    size_t& position
){
    size_t start = position;
    bool decimalPointFound = false;
    while (position < input.length()) {
        char current = input[position];
        if ( isdigit(static_cast<unsigned char>(current))) {
            position++;
        }
        else if (current == '.' && !decimalPointFound) {
            decimalPointFound = true;
            position++;
        }
        else {
            break;
        }
    }
    return {
        TokenType::Number,
        input.substr(start, position - start)
    };
}
Token Tokenizer::readString(
    const  string& input,
    size_t& position
){
    char quote = input[position];
    position++;
    size_t start = position;
    while (position < input.length() && input[position] != quote) {
        position++;
    }
    if (position >= input.length()) {
        throw  runtime_error("Unterminated string literal.");
    }
     string value = input.substr(start, position - start);
    position++;
    return {
        TokenType::String,
        value
    };
}
Token Tokenizer::readOperator(
    const  string& input,
    size_t& position
) {
    char current = input[position];
    if (position + 1 < input.length()) {
         string twoCharacters =
            input.substr(position, 2);
        if (
            twoCharacters == ">=" ||
            twoCharacters == "<=" ||
            twoCharacters == "!=" ||
            twoCharacters == "<>"
        ) {
            position += 2;

            return {
                TokenType::Operator,
                twoCharacters
            };
        }
    }
    if(
        current == '=' ||
        current == '>' ||
        current == '<' ||
        current == '+'
    ){
        position++;
        return {
            TokenType::Operator,
             string(1, current)
        };
    }
    throw  runtime_error(
         string("Unknown operator: ") + current
    );
}
 vector<Token> Tokenizer::tokenize(
    const  string& input
){
     vector<Token> tokens;
    size_t position = 0;
    while (position < input.length()) {
        char current = input[position];
        if ( isspace(static_cast<unsigned char>(current))) {
            position++;
            continue;
        }
        if (isIdentifierStart(current)) {
            tokens.push_back(
                readIdentifier(input, position)
            );
            continue;
        }
        if (isNumberStart(current)) {
            tokens.push_back(
                readNumber(input, position)
            );
            continue;
        }
        if (current == '"' || current == '\'') {
            tokens.push_back(
                readString(input, position)
            );
            continue;
        }
        if (
            current == '=' ||
            current == '>' ||
            current == '<' ||
            current == '+'
        ) {
            tokens.push_back(
                readOperator(input, position)
            );
            continue;
        }
        if (
            current == ';' ||
            current == ',' ||
            current == '(' ||
            current == ')' ||
            current == '*'
        ) {
            tokens.push_back({
                TokenType::Symbol,
                 string(1, current)
            });
            position++;
            continue;
        }
        throw  runtime_error(
            string("Unexpected character: ") + current
        );
    }
    tokens.push_back({
        TokenType::EndOfFile,
        ""
    });
    return tokens;
}