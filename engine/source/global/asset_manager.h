#pragma once

#include <fstream>
#include <vector>

namespace JMEngine
{
    class AssetManager final
    {
    private:
        AssetManager();
        ~AssetManager();
        AssetManager(const AssetManager &assetManager) = delete;
        AssetManager &operator=(const AssetManager &assetManager) = delete;

    public:
        static std::vector<char> ReadFile(const std::string &filename);
        // [[nodiscard]] inline static AssetManager &GetInstance()
        // {
        //     static AssetManager assetManager;
        //     return assetManager;
        // }
    };

}