#pragma once
#include "ast.hpp"
#include "token.hpp"
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<Expr> parse();

private:
    std::vector<Token> tokens_;
    std::size_t position_ = 0;

    const Token& current() const;
    bool match(TokenType type);
    const Token& consume(TokenType type, const char* message);
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
};
