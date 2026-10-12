#pragma once
#include "token.hpp"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source_;
    std::size_t position_ = 0;

    char current() const;
    void skipWhitespace();
    Token number();
};
