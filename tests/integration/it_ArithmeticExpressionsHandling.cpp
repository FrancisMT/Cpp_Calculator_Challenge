#include "gtest/gtest.h"

#include "calculator/Runner.hpp"
#include "utils/Methods.hpp"

/**
 * @brief Tests that the calculator correctly performs a series of arithmetic operations
 * and handles commands like "undo" and "result".
 */
TEST(CalculatorIntegrationTest, calculatorCorrectlyPerformsSupportedOperations)
{
    Calculator::Runner calculator;

    // Iterate over a list of arithmetic expressions and the corresponding expected results
    for (const auto& [arithmeticExpression, expectedResults] :
         std::initializer_list<std::pair<std::string, std::vector<std::string>>>{
               {"a=2+3", {"a = 5"}},                   // Basic addition
               {"b=e-2", {}},                          // Subtraction with unresolved dependency
               {"c=1+2", {"c = 3"}},                   // Basic addition
               {"d=e/3", {}},                          // Division with unresolved dependency
               {"e=a+c", {"e = 8", "b = 6", "d = 2"}}, // Addition with resolved dependencies with
                                                       // resolution of dependent expressions
               {"f=3+4", {"f = 7"}},                   // Basic addition
               {"undo 2", {"delete f", "delete e"}},   // Undo the last two operations
               {"e=2+2", {"e = 4", "b = 2", "d = 1"}}, // Redefinition of 'e' with
                                                       // resolution of dependent expressions
               {"f=g*7", {}},                          // Multiplication with unresolved dependency
               {"result", {"return e = 4"}},           // Request result of the last fulfilled expression
               {"g=3*2", {"g = 6", "f = 42"}}          // Multiplication and with
                                                       // resolution of dependent expressions
         }) {

        // Check if the calculator results match the expected ones
        const auto operationResults = calculator.processInstruction(arithmeticExpression);
        ASSERT_EQ(operationResults, expectedResults);
    }
}
