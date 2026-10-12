#include "compiler.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const bool showBytecode = argc > 1 && std::string(argv[1]) == "--bytecode";
    std::cout << "C++ Mini Compiler (integers and arithmetic)\n"
              << "Enter an expression, or type 'exit' to quit.\n";

    std::string line;
    while (std::cout << "\n> " && std::getline(std::cin, line)) {
        if (line == "exit") break;
        if (line.empty()) continue;

        try {
            Lexer lexer(line);
            Parser parser(lexer.tokenize());
            auto syntaxTree = parser.parse();

            Compiler compiler;
            auto bytecode = compiler.compile(*syntaxTree);
            if (showBytecode) {
                std::cout << "Bytecode:\n" << Compiler::format(bytecode);
            }
            std::cout << "Result: " << Compiler::execute(bytecode) << '\n';
        } catch (const std::exception& error) {
            std::cout << "Compile/runtime error: " << error.what() << '\n';
        }
    }
    return 0;
}
