#pragma once

#include <memory>
#include <stack>
#include <string>

#include "ast/Node.hpp"

/**
 * @brief Class responsible for parsing arithmetic expressions and generating Abstract Syntax Trees
 */
class Parser
{
public:
    /// Alias representing a whole AST (stack of AST node unique pointers)
    using ASTofRSH = std::stack<std::unique_ptr<AST::Node>>;

    /**
     * @brief Class constructor
     *
     * @param[in] inputToParse String containing the arithmetic expression to parse
     */
    explicit Parser(const std::string& inputToParse);

    /**
     * @brief Checks the input for a valid arithmetic expression and generates the appropriate AST
     *
     * @return True if parsing and AST generation were successful (false otherwise)
     */
    [[nodiscard]] bool execute();

    /**
     * @brief Retrieves the operand of the LHS (Left Hand Side) expression
     *
     * @return String containing the operand of the LHS
     */
    [[nodiscard]] std::string getOperandOfLHS() const;

    /**
     * @brief Getter for the generated AST of the RHS (Right Hand Side) expression
     *
     * @return Shared pointer to a stack containing all the ATS nodes
     */
    [[nodiscard]] std::shared_ptr<ASTofRSH> getASTOfRHS() const;

private:
    /**
     * @brief Parses the LHS of the arithmetic expression
     *
     * @return True if parsing the LHS was successful (false otherwise)
     */
    [[nodiscard]] bool parseLHS();

    /**
     * @brief Parses the RHS (Right Hand Side) of the arithmetic expression
     *
     * @return True if parsing the RHS was successful (false otherwise)
     */
    [[nodiscard]] bool parseRHS();

    /**
     * @brief Validates the RHS of the arithmetic expression
     *
     * @return True if the RHS is a valid expression, false otherwise
     */
    [[nodiscard]] bool validateRHS();

    /**
     * @brief Creates an AST for the RHS of the arithmetic expression
     *
     * Uses the Shunting Yard algorithm to convert the RHS expression into an AST
     *
     * @return True if AST creation was successful (false otherwise)
     */
    [[nodiscard]] bool createASTforRHS();

private:
    ///  String representation of the LHS operand
    std::string mLHSString;

    /// String representation of the RHS expression
    std::string mRHSString;

    /// Input string to parse
    std::string mInputString;

    /// Stack to manage the operators of the RHS arithmetic expression during RHS expression parsing
    std::stack<char> mRHSOperatorStack;

    /// Shared ownership pointer holding a stack of AST nodes that represent the RHS expression
    std::shared_ptr<ASTofRSH> mRHSValueStack;
};
