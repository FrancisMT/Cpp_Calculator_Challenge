#include "Runner.hpp"

#include "evaluator/Evaluator.hpp"
#include "parser/Parser.hpp"
#include "utils/Constants.hpp"
#include "utils/Methods.hpp"

namespace {
/// Supported string for the undo command
constexpr auto cUndoCommand{"undo"};
/// Supported string for the result command
constexpr auto cResultCommand{"result"};

/**
 * @brief Enum representing operations supported by the calculator
 */
enum class SupportedOperation : uint8_t {

    RESULT = 0, // Present result of last fulfilled operation
    UNDO = 1,   // Undo a certain amount of operation
    OTHER = 2   // Most probably an arithmetic expression (needs further evaluation)
};

/**
 * @brief Parses an input string to determine the type of operation that is being requested
 *
 * @param[in] input The input string to parse
 *
 * @return A pair consisting of the type of operation and an optional integer argument
 */
std::pair<SupportedOperation, std::optional<int>> getOperationRequest(const std::string& input)
{
    const auto inputStringTokens
          = Utils::Methods::splitString(input, Utils::Constants::cWhiteSpace);

    if (inputStringTokens.size() == 1 && inputStringTokens.back() == cResultCommand) {
        return {SupportedOperation::RESULT, {}};
    } else if (inputStringTokens.size() == 2 && inputStringTokens.front() == cUndoCommand) {

        int result{};
        try {
            result = std::stoi(inputStringTokens.back());
        }
        catch (const std::exception& e) {
            result = -1;
        }

        return {SupportedOperation::UNDO, result};
    }

    return {SupportedOperation::OTHER, {}};
}

} // namespace

namespace Calculator {

std::vector<std::string> Runner::processInstruction(const std::string& input)
{
    std::vector<std::string> results;

    // Handle situations where the user provided a supported instructions
    // instead of an arithmetic expression.
    {
        const auto operationRequest = getOperationRequest(input);

        switch (operationRequest.first) {
        case SupportedOperation::RESULT: {
            const auto lastOperation = mState.getLastFulfilledOperation();

            if (lastOperation == decltype(lastOperation)()) {
                std::cerr << "There is no result available yet\n";
            } else {
                results.emplace_back("return " + lastOperation.first + " = "
                                     + std::to_string(lastOperation.second));
            }

            return results;
        }
        case SupportedOperation::UNDO: {
            const auto undoneOperations = mState.undoLastRegisteredOperations(
                  operationRequest.second.value_or(/*default*/ 0));

            if (undoneOperations.empty()) {
                std::cout << "No operations were undone\n";
            } else {
                for (const auto& undoneOperation : undoneOperations) {
                    results.emplace_back("delete " + undoneOperation);
                }
            }

            return results;
        }
        case SupportedOperation::OTHER:
        default:
            break;
        }
    }

    // Try to parse the provided arithmetic expression
    Parser expressionParser(input);
    if (!expressionParser.execute()) {
        std::cout << "\nInvalid arithmetic expression provided.";
        return results;
    }

    // Retrieve the LHS of the parsed arithmetic expression (an operand).
    const auto expressionOperand = expressionParser.getOperandOfLHS();
    // Retrieve the RHS of the parsed arithmetic expression (an AST).
    const auto expressionAST = expressionParser.getASTOfRHS();

    // Try to evaluate the AST to check if we can obtain
    // either a valid result or a list of unmet dependencies
    Evaluator astEvaluator(expressionAST->top(),
                           // the map with the current values of each operand is provided for
                           // dependency lookup when evaluation the AST
                           mState.getOperandValueMap());

    // Get the result of the evaluation and process it according to its type
    const auto evaluationResult = astEvaluator.execute();
    std::visit(
          [&](auto&& variantValue) {
              // Expected types: int or unordered_set<std::string>
              using VariantType = std::decay_t<decltype(variantValue)>;

              // Did we get a value after the expression was evaluated?
              if constexpr (std::is_same_v<VariantType, int>) {

                  // Then, store it
                  for (const auto& [operand, value] :
                       mState.storeExpressionValue(expressionOperand, variantValue)) {

                      results.emplace_back(operand + " = " + std::to_string(value));
                  }

                  mState.updateOperationOrder(expressionOperand);
              }
              // Or did we get a list of unmet dependencies instead?
              else if constexpr (std::is_same_v<VariantType, Evaluator::Dependencies>) {

                  if (!variantValue.empty()) {

                      // Then, update the state of the dependencies
                      if (!mState.storeExpressionDependencies(
                                expressionOperand, expressionAST, variantValue)) {

                          std::cerr << "Cyclic dependency found: \'" << expressionOperand
                                    << "\' is already a dependency in another expression\n";
                      } else {
                          mState.updateOperationOrder(expressionOperand);
                      }
                  }
              } else {
                  std::cerr << "Unknown result type returned\n";
              }
          },
          evaluationResult);

    return results;
}

} // namespace Calculator
