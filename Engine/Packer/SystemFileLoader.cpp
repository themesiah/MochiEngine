#include "SystemFileLoader.h"

#include <filesystem>
#include <format>
#include <fstream>

#include "../Utils/StringUtils.h"

namespace Mochi::FS
{

    SystemFileLoader::SystemFileLoader(const std::string &directoryPath) : mDirectoryPath(directoryPath)
    {
        if (!IsValid())
        {
            throw std::runtime_error(std::format("{} is not a directory", directoryPath));
        }
    }

    SystemFileLoader::~SystemFileLoader()
    {
    }

    bool SystemFileLoader::IsValid() const
    {
        return std::filesystem::is_directory(mDirectoryPath);
    }

    std::vector<char> SystemFileLoader::GetFile(const std::string &path)
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        if (!HasFile(normalizedPath))
        {
            throw std::runtime_error(std::format("{} is not a file", path));
        }
        std::filesystem::path filepath(normalizedPath);
        filepath = mDirectoryPath / filepath;
        std::ifstream file(filepath, std::ios::binary);

        std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        return data;
    }

    bool SystemFileLoader::HasFile(const std::string &path) const
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        if (!IsValid())
        {
            throw std::runtime_error(std::format("{} is not a directory", mDirectoryPath.string()));
        }
        std::filesystem::path filepath(normalizedPath);
        filepath = mDirectoryPath / filepath;
        return std::filesystem::exists(filepath);
    }

    std::vector<std::string> SystemFileLoader::GetAvailableFiles() const
    {
        std::vector<std::string> list;
        for (const auto &p : std::filesystem::recursive_directory_iterator(mDirectoryPath))
        {
            if (!std::filesystem::is_directory(p))
            {
                list.push_back(p.path().string());
            }
        }
        return list;
    }
}
