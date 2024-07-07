#include "State.hpp"

#include <functional>

#include "utils/Methods.hpp"

namespace Calculator {

void State::updateOperationOrder(const std::string& operand)
{
    mOperandOrderStack.push(operand);
}

std::vector<std::pair<std::string, int>>
      State::storeExpressionValue(const std::string& operand, const int value)
{
    std::vector<std::pair<std::string, int>> affectedValues;

    // Function used to handle the recursive logic of storing values and resolving dependencies
    std::function<void(const std::string&, int)> storeValueAndCheckDependencies =

          [&](const std::string& newOperand, int newValue) {
              // Update the values map with the new value of the operand
              mOperandValuesMap.insert_or_assign(newOperand, newValue);
              affectedValues.emplace_back(newOperand, newValue);

              // Check if there are any expressions that depend on the provided operand
              // (whose value is now known) and if so, try to resolve them
              const auto operandDependencyRange = mOperandDependenciesMap.equal_range(newOperand);

              for (auto itr = operandDependencyRange.first; itr != operandDependencyRange.second;
                   ++itr) {

                  const auto dependantOperand = itr->second;

                  // If the dependent operand has an associated expression, evaluate it
                  if (mExpressionsWithDependenciesMap.contains(dependantOperand)) {

                      Evaluator evaluator(
                            mExpressionsWithDependenciesMap.at(dependantOperand)->top(),
                            mOperandValuesMap);
                      const auto evaluatorResult = evaluator.execute();

                      // If the evaluation results in an integer value,
                      // store it and check its dependencies
                      if (const int* dependantOperandResult = std::get_if<int>(&evaluatorResult)) {
                          storeValueAndCheckDependencies(dependantOperand, *dependantOperandResult);
                      }
                  }
              }
          };

    // Bootstrap the the recursive dependencies checking
    storeValueAndCheckDependencies(operand, value);

    return affectedValues;
}

bool State::storeExpressionDependencies(const std::string& operand,
                                        std::shared_ptr<Parser::ASTofRSH> expressionAST,
                                        const Evaluator::Dependencies& dependencies)
{

    // Check for cyclic dependencies (e.g.: a = c, b = a, c = b).
    const auto operandDependencyRange = mOperandDependenciesMap.equal_range(operand);

    for (auto itr = operandDependencyRange.first; itr != operandDependencyRange.second; ++itr) {

        const auto dependantOperand = itr->second;

        if (dependencies.contains(dependantOperand)) {
            // Cyclic dependency found.
            return false;
        }
    }

    // Store the expression's AST of the provided operand
    // since it might be resolved later if the dependencies are met.
    mExpressionsWithDependenciesMap.insert_or_assign(operand, std::move(expressionAST));

    // Add the new dependencies to the operand dependencies map
    for (const auto& dependency : dependencies) {
        mOperandDependenciesMap.emplace(dependency, operand);
    }

    return true;
}

const std::unordered_map<std::string, int>& State::getOperandValueMap() const
{
    return mOperandValuesMap;
}

std::pair<std::string, int> State::getLastFulfilledOperation() const
{
    // Go through the stack of operations history and check
    // which operand already has a value available
    auto operandOrderStack = mOperandOrderStack;
    while (!operandOrderStack.empty()) {

        if (mOperandValuesMap.contains(operandOrderStack.top())) {

            return {operandOrderStack.top(), mOperandValuesMap.at(operandOrderStack.top())};
        }

        operandOrderStack.pop();
    }

    return {};
}

std::vector<std::string> State::undoLastRegisteredOperations(const int undoCount)
{
    std::vector<std::string> deletedOperations;

    // Check for either an invalid count value or if there are enough operations to undo
    if (undoCount <= 0 || static_cast<int>(mOperandOrderStack.size()) < undoCount) {
        return deletedOperations;
    }

    for (int deleteCounter = 0; deleteCounter < undoCount; ++deleteCounter) {

        // Get the operand from the top of the stack
        const auto operand = mOperandOrderStack.top();

        // Try to remove the operand from the operand values map
        if (mOperandValuesMap.contains(operand)) {
            mOperandValuesMap.erase(operand);
        }

        // Tey to remove the operand from the expressions with dependencies map
        if (mExpressionsWithDependenciesMap.contains(operand)) {
            mExpressionsWithDependenciesMap.erase(operand);
        }

        // Remove the operand from the operation order stack
        mOperandOrderStack.pop();

        deletedOperations.push_back(operand);
    }

    return deletedOperations;
}

} // namespace Calculator
