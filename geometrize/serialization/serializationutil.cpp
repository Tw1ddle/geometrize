#include "serializationutil.h"

#include <cassert>
#include <fstream>

#include <QDir>
#include <QFileInfo>

#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"

#include "common/util.h"

namespace geometrize
{

namespace util
{

std::vector<std::string> readStringVector(const std::string& filePath)
{
    if(!geometrize::util::stringEndsWith(filePath, ".json")) {
        assert(0 && "Failed to read string vector, check file extension");
    }

    std::vector<std::string> result;
    std::ifstream file(filePath);
    if(!file) {
        return result;
    }

    cereal::JSONInputArchive archive(file);
    archive(result);
    return result;
}

void writeStringVector(const std::vector<std::string>& vec, const std::string& filePath)
{
    if(!geometrize::util::stringEndsWith(filePath, ".json")) {
        assert(0 && "Failed to write string vector, check file extension");
    }

    // Create folders up to the path if necessary
    const QFileInfo fileInfo(QString::fromStdString(filePath));
    if(!fileInfo.exists()) {
        const bool createdDir{QDir().mkpath(fileInfo.absolutePath())};
        if(!createdDir) {
            assert(0 && "Failed to create directory for string vector");
            return;
        }
    }

    std::ofstream file(filePath);
    cereal::JSONOutputArchive archive(file);
    archive(cereal::make_nvp("stringVector", vec));
}

}

}
