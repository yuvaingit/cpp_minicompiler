#pragma once
#include <string>

enum class TokenType {
    Number, Plus, Minus, Star, Slash, LeftParen, RightParen, End
};

struct Token {
    TokenType type;
    std::string text;
    int value = 0;
    std::size_t position = 0;
};
