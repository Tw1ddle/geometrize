#pragma once

#include <string>
#include <vector>

namespace geometrize
{

namespace util
{

/**
 * @brief readStringVector Reads the JSON file containing the vector of strings at the given filepath. Will attempt to overwrite any existing file when it does this.
 * @param filePath The path to the file to read.
 * @return The vector of strings read from the file.
 */
std::vector<std::string> readStringVector(const std::string& filePath);

/**
 * @brief writeStringVector Writes the string vector to the given filepath as JSON. Will attempt to overwrite any existing file.
 * @param text The vector of strings to write.
 * @param filePath The path of the file to write to.
 */
void writeStringVector(const std::vector<std::string>& vec, const std::string& filePath);

}

}
