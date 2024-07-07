
#include <iostream>

#include "calculator/Runner.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Calculator::Runner calculator;

    const auto getUserInputString = [] {
        std::cout << "\nInput Arithmetic expression to evaluate: ";
        std::string input;
        std::getline(std::cin, input);

        return input;
    };

    while (true) {
        const auto operationResults = calculator.processInstruction(getUserInputString());

        for (auto itr = operationResults.cbegin(); itr != operationResults.cend(); ++itr) {
            std::cout << *itr << (std::next(itr) != operationResults.cend() ? ", " : "\n");
        }
    }

    return 0;
}
