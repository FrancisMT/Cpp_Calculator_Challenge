#include "Evaluator.hpp"

#include "utils/Constants.hpp"

namespace {
/**
 * @brief Performs an arithmetic operation
 *
 * @param[in] operation Binary operation type
 * @param[in] leftOperand Left Operand
 * @param[in] rightOperand Right Operand
 *
 * @return Operation result
 */
constexpr auto performArithmeticOperation(const char operation,
                                          const float leftOperand,
                                          const float rightOperand)
{
    using namespace Utils::Constants;
    switch (operation) {
    case cAddOp:
        return leftOperand + rightOperand;
    case cSubOp:
        return leftOperand - rightOperand;
    case cMultOp:
        return leftOperand * rightOperand;
    case cDivOp:
        return leftOperand / rightOperand;
    default:
        return 1.f;
    }
}
} // namespace

Evaluator::Evaluator(const std::unique_ptr<AST::Node>& astRootNode,
                     const std::unordered_map<std::string, int>& operandLookupMap)
    : mAstRootNode{astRootNode}
    , mDependenciesLookupMap{operandLookupMap}
{
}

Evaluator::Result Evaluator::execute()
{
    if (!mAstRootNode) {
        std::cerr << "Empty AST";
        return {};
    }

    const auto expressionValue = static_cast<int32_t>(analyseAndTraverseASTNode(mAstRootNode));

    if (!mDependencies.empty()) {
        return mDependencies;
    }

    return expressionValue;
}

float Evaluator::analyseAndTraverseASTNode(const std::unique_ptr<AST::Node>& node)
{
    const auto nodeValue = node->getNodeValue();

    if (std::isdigit(nodeValue)) {
        return static_cast<float>(nodeValue - '0');
    } else if (std::isalpha(nodeValue)) {

        const auto nodeValueString = {nodeValue};

        // If the variable exists in the lookup map, return the corresponding value
        if (mDependenciesLookupMap.contains(nodeValueString)) {
            return static_cast<float>(mDependenciesLookupMap.at(nodeValueString));
        }

        // Otherwise, add it as a dependencies
        mDependencies.insert(nodeValueString);

    } else {
        const auto leftNodeValue = analyseAndTraverseASTNode(node->getReferenceToLeftNodePointer());
        const auto rightNodeValue
              = analyseAndTraverseASTNode(node->getReferenceToRightNodePointer());

        return performArithmeticOperation(nodeValue, leftNodeValue, rightNodeValue);
    }

    return 0.f;
}
