#include "compiler.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>

void Compiler::emit(const Expr& node, std::vector<Instruction>& output) {
    if (node.kind == Expr::Kind::Number) {
        output.push_back({"PUSH", node.value});
        return;
    }

    if (node.kind == Expr::Kind::Unary) {
        emit(*node.right, output);
        if (node.op == '-') {
            output.push_back({"NEG", 0});
        }
        return;
    }

    emit(*node.left, output);
    emit(*node.right, output);

    switch (node.op) {
        case '+': output.push_back({"ADD", 0}); break;
        case '-': output.push_back({"SUB", 0}); break;
        case '*': output.push_back({"MUL", 0}); break;
        case '/': output.push_back({"DIV", 0}); break;
        default: throw std::runtime_error("Unknown operator in syntax tree");
    }
}

std::vector<Instruction> Compiler::compile(const Expr& root) {
    std::vector<Instruction> program;
    emit(root, program);
    return program;
}

int Compiler::execute(const std::vector<Instruction>& program) {
    std::vector<int> stack;

    for (const auto& instruction : program) {
        if (instruction.opcode == "PUSH") {
            stack.push_back(instruction.operand);
            continue;
        }
        if (instruction.opcode == "NEG") {
            if (stack.empty()) throw std::runtime_error("Invalid bytecode stack");
            stack.back() = -stack.back();
            continue;
        }

        if (stack.size() < 2) throw std::runtime_error("Invalid bytecode stack");
        const int rhs = stack.back(); stack.pop_back();
        const int lhs = stack.back(); stack.pop_back();

        if (instruction.opcode == "ADD") stack.push_back(lhs + rhs);
        else if (instruction.opcode == "SUB") stack.push_back(lhs - rhs);
        else if (instruction.opcode == "MUL") stack.push_back(lhs * rhs);
        else if (instruction.opcode == "DIV") {
            if (rhs == 0) throw std::runtime_error("Division by zero");
            stack.push_back(lhs / rhs);
        } else {
            throw std::runtime_error("Unknown instruction: " + instruction.opcode);
        }
    }

    if (stack.size() != 1) throw std::runtime_error("Invalid bytecode result");
    return stack.back();
}

std::string Compiler::format(const std::vector<Instruction>& program) {
    std::ostringstream output;
    for (std::size_t i = 0; i < program.size(); ++i) {
        output << i << ": " << program[i].opcode;
        if (program[i].opcode == "PUSH") output << ' ' << program[i].operand;
        output << '\n';
    }
    return output.str();
}
