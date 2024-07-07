#include "gtest/gtest.h"

#include "parser/Parser.hpp"

using namespace ::testing;

/**
 * @brief Test fixture for the Parser class
 */
class ParserUnitTest : public Test
{
protected:
    /**
     * @brief Validates whether the parser is successful or not
     *
     * @param[in] isSuccessScenario True if a success scenario is to be tested (false otherwise)
     */
    void testInputs(const bool isSuccessScenario)
    {
        for (const auto& inputString : mTestInputs) {
            Parser parser(inputString);
            ASSERT_EQ(isSuccessScenario, parser.execute());
        }
    }

    /**
     * @brief Compares two ASTs to determine if they are identical
     *
     * @param[in] rootNodeA Reference to the root node of the first AST
     * @param[in] rootNodeB Reference to the root node of the second AST
     *
     * @return True if the ASTs are identical (false otherwise)
     */
    [[nodiscard]] bool areASTsIdentical(const std::unique_ptr<AST::Node>& rootNodeA,
                                        const std::unique_ptr<AST::Node>& rootNodeB)
    {
        return (!rootNodeA && !rootNodeB)
               || ((rootNodeA && rootNodeB)
                   && (rootNodeA->getNodeValue() == rootNodeB->getNodeValue())
                   && areASTsIdentical(rootNodeA->getReferenceToLeftNodePointer(),
                                       rootNodeB->getReferenceToLeftNodePointer())
                   && areASTsIdentical(rootNodeA->getReferenceToRightNodePointer(),
                                       rootNodeB->getReferenceToRightNodePointer()));
    }

    /**
     * @brief Counts the total number of nodes inside an AST
     *
     * @param[in] rootNode Reference to the root node of the AST
     *
     * @return Number of nodes of the AST
     */
    [[nodiscard]] uint32_t getNumberOfNodes(const std::unique_ptr<AST::Node>& rootNode)
    {
        return !rootNode ? 0
                         : 1 + getNumberOfNodes(rootNode->getReferenceToLeftNodePointer())
                                 + getNumberOfNodes(rootNode->getReferenceToRightNodePointer());
    }

protected:
    /// List of inputs to be tested
    std::vector<std::string> mTestInputs;
};

/**
 * @brief Tests that the Parser fails when the input string is empty
 */
TEST_F(ParserUnitTest, parserFailsWhenInputIsEmpty)
{
    mTestInputs = {""};
    testInputs(false);
}

/**
 * @brief Tests that the Parser fails when the input has negative numbers
 */
TEST_F(ParserUnitTest, parserFailsWhenNegativeNumbersAreProvided)
{
    mTestInputs = {"a = -1", "b = -22", "c = 2+(-3*2)", "d = (-5)", "e = -(2*3)"};
    testInputs(false);
}

/**
 * @brief Tests that the Parser fails when the input has integers with more than one digit
 */
TEST_F(ParserUnitTest, parserFailsWhenLiteralsAreTooLarge)
{
    mTestInputs = {"a = 42", "b = 1337", "c = 11*11+3-(20)", "d = 10  +  1"};
    testInputs(false);
}

/**
 * @brief Tests that the Parser fails when the input has unpaired parentheses
 */
TEST_F(ParserUnitTest, parserFailsWhenParenthesesAreNotPaired)
{
    mTestInputs = {"a = (1+2))", "b = (3*  3", "c = 4+2*(5/7))"};
    testInputs(false);
}

/**
 * @brief Tests that the Parser fails when the input has incorrectly used parentheses
 */
TEST_F(ParserUnitTest, parserFailsWhenParenthesesAreIncorrectlyUsed)
{
    mTestInputs = {"a = )1+2(", "b = (3*()3)", "c = 4+2)(*(5/7)"};
    testInputs(false);
}

/**
 * @brief Tests that the Parser fails when the input has missing operators
 */
TEST_F(ParserUnitTest, parserFailsWhenOperatorsAreMissing)
{
    mTestInputs = {"a = (4 + 5 (7 - 3)) - 2", "b = 2+(5*3)7"};
    testInputs(false);
}

/**
 * @brief Tests that the Parser fails when the input has incorrectly used operators
 */
TEST_F(ParserUnitTest, parserFailsWhenOperatorsAreIncorrectlyUsed)
{
    mTestInputs = {" a = (4 +* 5 - (7 - 3)) - 2",
                   "b = 2+(/5*3)7",
                   "c = *(4+5)-3",
                   "d = +-42",
                   "e = 1-3+3/7+"};
    testInputs(false);
}

/**
 * @brief Tests that the Parser succeeds when the input has the correct syntax
 */
TEST_F(ParserUnitTest, parserSucceedsWhenValidInputIsProvided)
{
    mTestInputs = {"a = (4 + 5 * (7 - 3)) - 2",
                   "b = 4+5+7/2",
                   "c = 2+3*(1-2)",
                   "d = 5+(1*2)",
                   "e = 2+3* 1 - 2",
                   "f = 7+3*(1/(2/(3+1)-1))",
                   "g = (2*(3+6/2)/4)"};
    testInputs(true);
}

/**
 * @brief Tests that the Parser is able to retrieve a correct LHS operand
 * and constructs a valid RHS AST from the input
 */
TEST_F(ParserUnitTest, parserRetrievesCorrectOperandAndAST)
{
    constexpr auto validArithmeticExpression{"a = 5+(1*2)"};
    constexpr auto expectedOperand{"a"};
    const auto expectedAST = std::make_unique<AST::Node>(
          '+', // First Level
          std::make_unique<AST::Node>('5'),
          std::make_unique<AST::Node>('*', // Second Level
                                      std::make_unique<AST::Node>('1'),
                                      std::make_unique<AST::Node>('2')));
    constexpr auto expectedASTNodeCount{5};

    Parser parser(validArithmeticExpression);
    ASSERT_TRUE(parser.execute());
    ASSERT_EQ(parser.getOperandOfLHS(), expectedOperand);

    const auto retrievedAST = parser.getASTOfRHS();

    ASSERT_NE(retrievedAST, nullptr);
    ASSERT_FALSE(retrievedAST->empty());

    const auto& astRootNode = retrievedAST->top();
    ASSERT_EQ(getNumberOfNodes(astRootNode), expectedASTNodeCount);
    ASSERT_TRUE(areASTsIdentical(astRootNode, expectedAST));
}
