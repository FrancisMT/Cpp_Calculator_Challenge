#pragma once

#include <memory>
#include <string>
#include <variant>
#include <unordered_map>
#include <unordered_set>

#include "ast/Node.hpp"

/**
 * @brief Class responsible for evaluating arithmetic expressions contained in an AST
 *
 * Evaluation is only performed on integers
 */
class Evaluator
{
public:
    /// Alias representing a set of operands that are dependencies of an expression
    using Dependencies = std::unordered_set<std::string>;
    /// Alias representing the result of the evaluation: an value or a set of dependencies
    using Result = std::variant<int, Dependencies>;

    /**
     * @brief Class constructor
     *
     * @param[in] astRootNode Reference to the root node of an (AST)
     * @param[in] dependenciesLookupMap Map of operand names to their corresponding integer values
     */
    explicit Evaluator(const std::unique_ptr<AST::Node>& astRootNode,
                       const std::unordered_map<std::string, int>& dependenciesLookupMap);

    /**
     * @brief Evaluates an AST holding an arithmetic expression and outputs a result
     *
     * During the evaluation, if dependencies are detected within the AST,
     * the provided dependencies map is used for value lookup
     *
     * If the evaluation is successful, the result will be the value o the expression
     *
     * However, if there are unresolved dependencies (variables) in the expression,
     * the result will be those dependencies
     *
     * @return Result of the arithmetic expression
     */
    [[nodiscard]] Result execute();

private:
    /**
     * @brief Helper method used to recursively traverse the AST and evaluate each node's content
     *
     * @param[in] node Reference to an AST node to analyse
     *
     * @return Final value of the node
     */
    [[nodiscard]] float analyseAndTraverseASTNode(const std::unique_ptr<AST::Node>& node);

private:
    /// Reference to the AST root node
    const std::unique_ptr<AST::Node>& mAstRootNode;

    /// Map used to lookup the value of specific operands
    ///( used to resolve dependencies when analysing an AST)
    const std::unordered_map<std::string, int>& mDependenciesLookupMap;

    /// Set of dependencies encountered during AST evaluation
    /// (operands not found on the dependencies lookup map)
    Dependencies mDependencies;
};
