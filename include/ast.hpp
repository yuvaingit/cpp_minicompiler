#pragma once
#include <memory>

struct Expr {
    enum class Kind { Number, Unary, Binary };
    Kind kind;
    int value = 0;
    char op = '\0';
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    explicit Expr(int number) : kind(Kind::Number), value(number) {}
    Expr(char operation, std::unique_ptr<Expr> operand)
        : kind(Kind::Unary), op(operation), right(std::move(operand)) {}
    Expr(char operation, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
        : kind(Kind::Binary), op(operation), left(std::move(lhs)),
          right(std::move(rhs)) {}
};
