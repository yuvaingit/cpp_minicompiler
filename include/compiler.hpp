#pragma once
#include "ast.hpp"
#include <string>
#include <vector>

struct Instruction {
    std::string opcode;
    int operand = 0;
};

class Compiler {
public:
    std::vector<Instruction> compile(const Expr& root);
    static int execute(const std::vector<Instruction>& program);
    static std::string format(const std::vector<Instruction>& program);

private:
    void emit(const Expr& node, std::vector<Instruction>& output);
};
