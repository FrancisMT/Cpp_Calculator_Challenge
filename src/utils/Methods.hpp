#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace Utils::Methods {

/**
 * @brief Splits a string into substrings based on the specified delimiter
 *
 * @param[in] stringToSplit String to be split into substrings
 * @param[in] delimiter Delimiter used to split the string
 *
 * @return Vector containing the resulting substrings
 */
[[nodiscard]] inline std::vector<std::string> splitString(const std::string& stringToSplit, const char delimiter)
{
    std::vector<std::string> output;
    for (const auto& splitValue : stringToSplit | std::ranges::views::split(delimiter)) {
        output.emplace_back(splitValue.begin(), splitValue.end());
    }

    return output;
}

/**
 * @brief Removes all whitespace characters from the provided string
 *
 * @param[in,out] stringToTrim String to remove whitespaces from
 */
inline constexpr void removeWhiteSpacesFromString(std::string& stringToTrim)
{
    stringToTrim.erase(
          std::remove_if(stringToTrim.begin(),
                         stringToTrim.end(),
                         [](unsigned char stringChar) { return std::isspace(stringChar); }),
          stringToTrim.end());
}

} // namespace Utils::Methods
