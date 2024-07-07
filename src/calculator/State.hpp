#pragma once

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "evaluator/Evaluator.hpp"
#include "parser/Parser.hpp"

namespace Calculator {

// TODO: Derive from an interface since it will facilitate the creating of new tests using
// mocked interfaces and dependency injection into the Runner class

/**
 * @brief Manages the state of the calculator:
 * - maintains the order of operations
 * - stores the results of evaluated expressions
 * - tracks dependencies between operands
 */
class State
{
public:
    /**
     * @brief Class' default constructor
     */
    State() = default;

    /**
     * @brief Updates the operation order with the given operand.
     *
     * @param[in] operand The operand to update in the operation order.
     */
    void updateOperationOrder(const std::string& operand);

    /**
     * @brief Stores the value of a given operand and recursively resolves
     * any dependencies that can be fulfilled with the new value
     *
     * @param[in] operand Operand whose value is to be stored
     * @param[in] value Value of the operand
     *
     * @return Operands and their respective values that were affected by setting the new value
     */
    std::vector<std::pair<std::string, int>>
          storeExpressionValue(const std::string& operand, const int value);

    /**
     * @brief Stores the dependencies of an expression
     *
     * As a safeguard, cyclic dependencies are checked before storing new dependencies
     *
     * @param[in] operand Operand whose dependencies are to be stored
     * @param[in] expressionAST AST of the expression associated with the operand
     * @param[in] dependencies Set of operands that the given operand depends on
     *
     * @return True if the dependencies were stored successfully
     * @return False if a cyclic dependency was found
     */
    [[nodiscard]] bool storeExpressionDependencies(const std::string& operand,
                                                   std::shared_ptr<Parser::ASTofRSH> expressionAST,
                                                   const Evaluator::Dependencies& dependencies);

    /**
     * @brief Retrieves the map of operand values for lookup
     *
     * @return A const reference to the map containing operand values
     */
    [[nodiscard]] const std::unordered_map<std::string, int>& getOperandValueMap() const;

    /**
     * @brief Retrieves the result of the last fulfilled operation
     *
     * @return Operand and value pair relative to the last fulfilled operation
     */
    [[nodiscard]] std::pair<std::string, int> getLastFulfilledOperation() const;

    /**
     * @brief Undoes the specified number of operations
     * (by removing their entries from the operation history and the operand values registry)
     *
     * @param[in] undoCount Number of operations to undo
     *
     * @return Operands of the undone operations
     */
    [[nodiscard]] std::vector<std::string> undoLastRegisteredOperations(const int undoCount);

private:
    /// LIFO stack to keep track of the order of operations by tracking operands of each expression
    std::stack<std::string> mOperandOrderStack;

    /// Map holding the operands with their current values
    std::unordered_map<std::string, int> mOperandValuesMap;

    /// Multimap to track dependencies between operands (one to many relationship).
    std::unordered_multimap<std::string, std::string> mOperandDependenciesMap;

    /// Map to track arithmetic expressions that depend on the values of other operands
    std::unordered_map<std::string, std::shared_ptr<Parser::ASTofRSH>> mExpressionsWithDependenciesMap;
};

} // namespace Calculator
