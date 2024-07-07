#pragma once

#include <iostream>

namespace AST {

/**
 * @brief Implementation of an AST Node
 */
class Node
{
public:
    /**
     * @brief Class constructor
     *
     * @param[in] nodeValue char representing the value that the node will hold
     * @param[in] leftNode Left child node
     * @param[in] rightNode Right child node
     */
    explicit Node(char nodeValue,
                  std::unique_ptr<Node> leftNode = nullptr,
                  std::unique_ptr<Node> rightNode = nullptr)
        : mNodeValue{nodeValue}
        , mLeftNode{std::move(leftNode)}
        , mRightNode{std::move(rightNode)}
    {
    }

    /**
     * @brief Getter for the value currently being held by the node
     *
     * @return Node value
     */
    [[nodiscard]] char getNodeValue() const
    {
        return mNodeValue;
    }

    /**
     * @brief Getter for the left child node
     *
     * @return Reference to the left child node pointer
     */
    [[nodiscard]] const std::unique_ptr<Node>& getReferenceToLeftNodePointer() const
    {
        return mLeftNode;
    }

    /**
     * @brief Getter for the right child node
     *
     * @return Reference to the right child node pointer
     */
    [[nodiscard]] const std::unique_ptr<Node>& getReferenceToRightNodePointer() const
    {
        return mRightNode;
    }

private:
    /// Value being held by the node
    char mNodeValue{};
    /// Left child node
    std::unique_ptr<Node> mLeftNode{nullptr};
    /// Right child node
    std::unique_ptr<Node> mRightNode{nullptr};
};

/**
 * @brief Helper method used to print (horizontally) the contents of an AST
 *
 * Recursive calls are made in order to print the content of every node's value.
 *
 * Preorder traversal is being used:
 * 1. Visit root node;
 * 2. Traverse left node (maintaining preorder traversal);
 * 2. Traverse right node (maintaining preorder traversal);
 *
 * @param[in] rootNode Reference to the root node of the AST
 * @param[in] prefix Helper string used to beautify the outputted data
 */
inline void printAST(const std::unique_ptr<Node>& rootNode, std::string&& prefix = "")
{
    if (rootNode) {
        std::cout << prefix << rootNode->getNodeValue() << "\n";
        printAST(rootNode->getReferenceToLeftNodePointer(), prefix + "    ");
        printAST(rootNode->getReferenceToRightNodePointer(), prefix + "    ");
    }
}

} // namespace AST
