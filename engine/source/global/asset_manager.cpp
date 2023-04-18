#include "source/global/asset_manager.h"
#include "source/global/log_system.h"

namespace JMEngine
{
    std::vector<char> AssetManager::ReadFile(const std::string &filename)
    {

        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            LOG_ERROR("failed to load file: " + filename);
            return std::vector<char>();
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }
}