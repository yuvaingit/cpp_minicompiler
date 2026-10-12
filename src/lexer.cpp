#include "lexer.hpp"
#include <cctype>
#include <stdexcept>
#include <utility>

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

char Lexer::current() const {
    return position_ < source_.size() ? source_[position_] : '\0';
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(current()))) {
        ++position_;
    }
}

Token Lexer::number() {
    const std::size_t start = position_;
    while (std::isdigit(static_cast<unsigned char>(current()))) {
        ++position_;
    }
    const std::string text = source_.substr(start, position_ - start);
    try {
        return {TokenType::Number, text, std::stoi(text), start};
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Integer literal is too large at position " +
                                 std::to_string(start));
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (current() != '\0') {
        skipWhitespace();
        if (current() == '\0') break;

        const std::size_t start = position_;
        const char ch = current();

        if (std::isdigit(static_cast<unsigned char>(ch))) {
            tokens.push_back(number());
            continue;
        }

        TokenType type;
        switch (ch) {
            case '+': type = TokenType::Plus; break;
            case '-': type = TokenType::Minus; break;
            case '*': type = TokenType::Star; break;
            case '/': type = TokenType::Slash; break;
            case '(': type = TokenType::LeftParen; break;
            case ')': type = TokenType::RightParen; break;
            default:
                throw std::runtime_error("Unexpected character '" +
                                         std::string(1, ch) + "' at position " +
                                         std::to_string(position_));
        }
        tokens.push_back({type, std::string(1, ch), 0, start});
        ++position_;
    }

    tokens.push_back({TokenType::End, "", 0, source_.size()});
    return tokens;
}
