#pragma once

#include <string>
#include <vector>

#include "State.hpp"

namespace Calculator {

/**
 * @brief Class responsible for processing instructions and managing the state of the calculator
 *
 * This class handles various calculator operations including:
 * - evaluating arithmetic expressions;
 * - undoing previous operations;
 * - fetching the result of the last completed operation;
 */
class Runner
{
public:
    /**
     * @brief Class' default constructor
     */
    Runner() = default;

    /**
     * @brief Processes a given instruction and returns the corresponding results
     *
     * Supported instructions are an arithmetic expression or commands like "undo 2" or "result"
     *
     * @param[in] input Instruction to process
     *
     * @return A vector of strings containing the results of the instruction after being processed
     */
    std::vector<std::string> processInstruction(const std::string& input);

private:
    /// State of the calculator (operand values and existing dependencies)
    State mState;
};

} // namespace Calculator
