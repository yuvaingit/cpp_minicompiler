#include "parser.hpp"
#include <stdexcept>
#include <string>
#include <utility>

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

const Token& Parser::current() const {
    return tokens_.at(position_);
}

bool Parser::match(TokenType type) {
    if (current().type != type) return false;
    ++position_;
    return true;
}

const Token& Parser::consume(TokenType type, const char* message) {
    if (current().type != type) {
        throw std::runtime_error(std::string(message) + " at position " +
                                 std::to_string(current().position));
    }
    return tokens_[position_++];
}

std::unique_ptr<Expr> Parser::parse() {
    auto result = expression();
    consume(TokenType::End, "Unexpected token after expression");
    return result;
}

// expression -> term (('+' | '-') term)*
std::unique_ptr<Expr> Parser::expression() {
    auto node = term();
    while (current().type == TokenType::Plus ||
           current().type == TokenType::Minus) {
        char op = current().type == TokenType::Plus ? '+' : '-';
        ++position_;
        node = std::make_unique<Expr>(op, std::move(node), term());
    }
    return node;
}

// term -> unary (('*' | '/') unary)*
std::unique_ptr<Expr> Parser::term() {
    auto node = unary();
    while (current().type == TokenType::Star ||
           current().type == TokenType::Slash) {
        char op = current().type == TokenType::Star ? '*' : '/';
        ++position_;
        node = std::make_unique<Expr>(op, std::move(node), unary());
    }
    return node;
}

// unary -> ('+' | '-') unary | primary
std::unique_ptr<Expr> Parser::unary() {
    if (match(TokenType::Minus)) {
        return std::make_unique<Expr>('-', unary());
    }
    if (match(TokenType::Plus)) {
        return unary();
    }
    return primary();
}

// primary -> NUMBER | '(' expression ')'
std::unique_ptr<Expr> Parser::primary() {
    if (current().type == TokenType::Number) {
        int value = current().value;
        ++position_;
        return std::make_unique<Expr>(value);
    }
    if (match(TokenType::LeftParen)) {
        auto node = expression();
        consume(TokenType::RightParen, "Expected ')'");
        return node;
    }
    throw std::runtime_error("Expected a number or '(' at position " +
                             std::to_string(current().position));
}
