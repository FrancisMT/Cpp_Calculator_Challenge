#include "Parser.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>
#include <unordered_set>

#include "ast/Node.hpp"
#include "utils/Constants.hpp"
#include "utils/Methods.hpp"

namespace {
using namespace Utils::Constants;

/**
 * @brief Checks if the provided character is a parenthesis
 *
 * @param[in] character Character to evaluate
 *
 * @return True if is a supported parenthesis (false otherwise)
 */
constexpr bool isParenthesis(const char character)
{
    switch (character) {
    case cLeftParenthesis:
    case cRightParenthesis:
        return true;
    default:
        return false;
    }
}

/**
 * @brief Checks if the provided character is a valid binary operator
 *
 * @param[in] character Character to evaluate
 *
 * @return True if is a supported operator (false otherwise)
 */
constexpr bool isOperator(const char character)
{
    switch (character) {
    case cAddOp:
    case cSubOp:
    case cMultOp:
    case cDivOp:
        return true;
    default:
        return false;
    }
}

/**
 * @brief Checks if the provided character is a single digit integer
 *
 * @param[in] previousCharacter Adjacent character to evaluate (cannot be a digit)
 * @param[in] character Character to evaluate
 *
 * @return True if is a single digit character (false otherwise)
 */
constexpr bool isSingleDigitInteger(const char previousCharacter, const char character)
{
    return !std::isdigit(previousCharacter) && std::isdigit(character);
}

/**
 * @brief Checks if the provided character is a unary minus
 *
 * @param[in] previousCharacter Adjacent character to evaluate (cannot be an operator, space or '(')
 * @param[in] character Character to evaluate
 *
 * @return True if character is an unary minus (false otherwise)
 */
constexpr bool isUnaryMinus(const char previousCharacter, const char character)
{
    return character == cSubOp
           && (isOperator(previousCharacter) || std::isspace(previousCharacter)
               || previousCharacter == cLeftParenthesis);
}

/**
 * @brief Determines the precedence level of an operator based on its type
 *
 * When constructing the AST using the Shunting Yard algorithm,
 * each operator being processed causes its preceding operators to "execute"
 * (new nodes in the AST are created) only if it has a higher precedence value
 *
 * Precedence levels:
 * - '('        : 1 (lowest precedence)
 * - '+', '-'   : 2
 * - '*', '/'   : 3
 * - ')'        : 4 (highest precedence)
 *
 * @param[in] op Operator whose precedence is to be determined
 *
 * @return The precedence level of the operator
 */
constexpr uint8_t operatorPrecedence(const char op)
{
    switch (op) {
    case cRightParenthesis:
        return 4;
    case cMultOp:
    case cDivOp:
        return 3;
    case cAddOp:
    case cSubOp:
        return 2;
    case cLeftParenthesis:
        return 1;
    default:
        return 0;
    }
}
} // namespace

Parser::Parser(const std::string& inputToParse)
    : mInputString{inputToParse}
    , mRHSValueStack{std::make_unique<ASTofRSH>()}
{
}

bool Parser::execute()
{
    Utils::Methods::removeWhiteSpacesFromString(mInputString);

    const auto inputStringTokens
          = Utils::Methods::splitString(mInputString, Utils::Constants::cAssignOp);

    if (inputStringTokens.size() != 2) {
        return false;
    }

    mLHSString = inputStringTokens.front();
    mRHSString = inputStringTokens.back();

    return parseLHS() && parseRHS();
}

std::string Parser::getOperandOfLHS() const
{
    return mLHSString;
}

std::shared_ptr<Parser::ASTofRSH> Parser::getASTOfRHS() const
{
    return mRHSValueStack;
}

bool Parser::parseLHS()
{
    // TODO[FM]: Add support for a more complex parsing.
    // Ideally we would also create an AST for the LHS but for now,
    // we only support LHS values with a single letter operands
    // (e.g. 'x' from in "x=2+2")
    if (mLHSString.size() != 1) {
        return false;
    }

    return true;
}

bool Parser::parseRHS()
{
    return validateRHS() && createASTforRHS();
}

bool Parser::validateRHS()
{
    if (mRHSString.empty()) {
        std::cerr << "Empty expression provided" << "\n";
        return false;
    }

    // String will be wrapped around parenthesis for easier parsing
    std::string validatedString{cLeftParenthesis};
    validatedString.reserve(mRHSString.size());

    uint32_t leftParenthesisCounter{0};
    uint32_t rightParenthesisCounter{0};

    for (const auto& character : mRHSString) {

        const auto& previousValidCharacter = validatedString.back();

        // Check digit validity
        // TODO: Add support for expressions with integers with more than one digit
        if (isSingleDigitInteger(previousValidCharacter, character)) {

            // Right parenthesis should not be followed by a digit (e.g. ")2" )
            if (previousValidCharacter == cRightParenthesis) {
                std::cerr << "Invalid expression provided" << "\n";
                return false;
            }
        }
        // Check operator validity
        else if (isOperator(character)) {

            // Check if the operator is used as a unary minus
            // TODO: Add support for expressions with negative integers (e.g. "-2*3")
            if (isUnaryMinus(previousValidCharacter, character)) {
                std::cout << "Negative values are not currently supported" << "\n";
                return false;
            }

            // Check if the previous character was not and operator or a left parenthesis
            // (e.g. "++2" or ")+2")
            if (isOperator(previousValidCharacter) || previousValidCharacter == cLeftParenthesis) {
                std::cerr << "Invalid expression provided" << "\n";
                return false;
            }
        }
        // Check parenthesis validity
        else if (isParenthesis(character)) {

            // Keep tabs on the amount of parenthesis pairs
            if (character == cLeftParenthesis) {
                ++leftParenthesisCounter;
            } else if (character == cRightParenthesis) {
                ++rightParenthesisCounter;
            }

            // Left parenthesis should not be preceded by a digit (e.g. "2("))
            // TODO: Add support for expressions with implicit multiplication
            if (character == cLeftParenthesis && std::isdigit(previousValidCharacter)) {
                std::cerr << "Invalid expression provided" << "\n";
                return false;
            }
        }
        // Account for single character variables
        else if (!std::isalpha(character)) {
            std::cerr << "Invalid expression provided" << "\n";
            return false;
        }

        validatedString.push_back(character);
    }

    // Validate the amount of parenthesis pairs
    if (leftParenthesisCounter != rightParenthesisCounter) {
        std::cerr << "Parenthesis do not match" << "\n";
        return false;
    }

    // Validate that the expression does not end with an operator
    if (isOperator(validatedString.back())) {
        std::cerr << "Invalid expression provided" << "\n";
        return false;
    }

    // Finalize the string wrapping by adding a right parenthesis at the end
    validatedString.append(1, cRightParenthesis);
    mRHSString.swap(validatedString);
    mRHSString.shrink_to_fit();

    return true;
}

bool Parser::createASTforRHS()
{
    if (mRHSString.empty()) {
        std::cerr << "Empty expression provided" << "\n";
        return false;
    }

    // Helper lambda used to add new nodes to the AST
    const auto generateNewNode = [this]() {
        if (!mRHSOperatorStack.empty() && !mRHSValueStack->empty()) {

            const auto operation = mRHSOperatorStack.top();
            mRHSOperatorStack.pop();

            auto rightValue = std::move(mRHSValueStack->top());
            mRHSValueStack->pop();

            auto leftValue = std::move(mRHSValueStack->top());
            mRHSValueStack->pop();

            mRHSValueStack->emplace(std::make_unique<AST::Node>(
                  operation, std::move(leftValue), std::move(rightValue)));
        }
    };

    // Analyse input
    for (const auto& character : mRHSString) {

        // Account for the possibility that we might have either a number or a variable in the
        // provided string
        if (std::isdigit(character) || std::isalpha(character)) {
            mRHSValueStack->emplace(std::make_unique<AST::Node>(character));

        } else if (isOperator(character)) {

            // Generate new nodes until an operator with a lower precedence
            // than the new one is found on the top of the operator stack
            while (!mRHSOperatorStack.empty()
                   && operatorPrecedence(mRHSOperatorStack.top())
                            >= operatorPrecedence(character)) {
                generateNewNode();
            }

            mRHSOperatorStack.push(character);

        } else if (character == cLeftParenthesis) {
            mRHSOperatorStack.push(character);

        } else if (character == cRightParenthesis) {

            // Generate new nodes until we reach the closest left parenthesis
            while (!mRHSOperatorStack.empty() && mRHSOperatorStack.top() != cLeftParenthesis) {
                generateNewNode();
            }

            // Pop left parenthesis
            if (!mRHSOperatorStack.empty()) {
                mRHSOperatorStack.pop();
            }
        }
    }

    // Generate new nodes until the operator stack is empty
    while (!mRHSOperatorStack.empty()) {
        generateNewNode();
    }

#ifdef DEBUG_BUILD
    if (!mRHSValueStack->empty()) {
        std::cout << "Generated Abstract Syntax Tree:\n";
        AST::printAST(mRHSValueStack->top());
    }
#endif

    return true;
}
