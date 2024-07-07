#include "gtest/gtest.h"

#include "evaluator/Evaluator.hpp"

/**
 * @brief Tests that the Evaluator correctly calculates the integer result
 * from ASTs representing arithmetic expressions without dependencies
 */
TEST(EvaluatorUnitTest, evaluatorOutputsCorrectResultWithoutDependencies)
{
    // Constructing a valid AST for the arithmetic expression: "4+5+7/2"
    using namespace AST;
    const auto rootNode = std::make_unique<Node>(
          // First Level: root node representing '+'
          '+',
          // Second Level: left child (4 + 5)
          std::make_unique<Node>('+',
                                 // Third Level: leaf nodes '4' and '5'
                                 std::make_unique<Node>('4'),
                                 std::make_unique<Node>('5')),
          // Second Level: right child (7 / 2)
          std::make_unique<Node>('/',
                                 // Third Level: leaf nodes '7' and '2'
                                 std::make_unique<Node>('7'),
                                 std::make_unique<Node>('2')));

    // Create an Evaluator with the AST and an empty operand lookup map
    Evaluator evaluator(rootNode, {});
    const auto result = evaluator.execute();

    // Verify that the evaluation resulted in the expected integer value
    ASSERT_TRUE(std::holds_alternative<int>(result));
    constexpr auto expectedValue{/* 4 + 5 + 7 / 2 = 4 + 5 + 3 = 12 */ 12};
    ASSERT_EQ(std::get<int>(result), expectedValue);
}

/**
 * @brief Tests that the Evaluator correctly calculates the integer result
 * from ASTs representing arithmetic expressions with dependencies
 * when the appropriate dependencies lookup map is provided
 */
TEST(EvaluatorUnitTest, evaluatorOutputsCorrectResultWithDependencies)
{
    // Constructing a valid AST for the arithmetic expression: "4+a+7/b"
    using namespace AST;
    const auto rootNode = std::make_unique<Node>(
          // First Level: root node representing '+'
          '+',
          // Second Level: left child (4 + a)
          std::make_unique<Node>('+',
                                 // Third Level: leaf nodes '4' and 'a'
                                 std::make_unique<Node>('4'),
                                 std::make_unique<Node>('a')),
          // Second Level: right child (7 / b)
          std::make_unique<Node>('/',
                                 // Third Level: leaf nodes '7' and 'b'
                                 std::make_unique<Node>('7'),
                                 std::make_unique<Node>('b')));

    // Setup the dependencies lookup map
    const std::unordered_map<std::string, int> dependenciesLookupMap{{"a", 5}, {"b", 2}};

    // Create an Evaluator with the AST and the operand lookup map
    Evaluator evaluator(rootNode, dependenciesLookupMap);
    const auto result = evaluator.execute();

    // Verify that the evaluation resulted in the expected integer value
    ASSERT_TRUE(std::holds_alternative<int>(result));
    constexpr auto expectedValue{/* 4 + 5 + 7 / 2 = 4 + 5 + 3 = 12 */ 12};
    ASSERT_EQ(std::get<int>(result), expectedValue);
}

/**
 * @brief Tests that the Evaluator cannot calculate a result from ASTs
 * representing arithmetic expressions with dependencies when no dependency map is provided
 * and instead returns the list of the corresponding unmet dependencies
 */
TEST(EvaluatorUnitTest, evaluatorOutputsDependenciesInsteadOfResult)
{
    // Constructing a valid AST for the arithmetic expression: "4+a+7/b"
    using namespace AST;
    const auto rootNode = std::make_unique<Node>(
          // First Level: root node representing '+'
          '+',
          // Second Level: left child (4 + a)
          std::make_unique<Node>('+',
                                 // Third Level: leaf nodes '4' and 'a'
                                 std::make_unique<Node>('4'),
                                 std::make_unique<Node>('a')),
          // Second Level: right child (7 / b)
          std::make_unique<Node>('/',
                                 // Third Level: leaf nodes '7' and 'b'
                                 std::make_unique<Node>('7'),
                                 std::make_unique<Node>('b')));

    // Create an Evaluator with the AST and an empty operand lookup map
    Evaluator evaluator(rootNode, {});
    const auto result = evaluator.execute();

    // Verify that the evaluation resulted in the expected dependencies
    ASSERT_TRUE(std::holds_alternative<Evaluator::Dependencies>(result));
    const Evaluator::Dependencies expectedDependencies{"a", "b"};
    ASSERT_EQ(std::get<Evaluator::Dependencies>(result), expectedDependencies);
}
