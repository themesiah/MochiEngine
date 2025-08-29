#include "SystemFileLoader.h"

#include <filesystem>
#include <format>
#include <fstream>

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

std::vector<char> SystemFileLoader::GetFile(std::string path)
{
    if (!HasFile(path))
    {
        throw std::runtime_error(std::format("{} is not a file", path));
    }
    std::filesystem::path filepath(path);
    filepath = mDirectoryPath / filepath;
    std::ifstream file(filepath, std::ios::binary);

    std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return data;
}

bool SystemFileLoader::HasFile(std::string path) const
{
    if (!IsValid())
    {
        throw std::runtime_error(std::format("{} is not a directory", mDirectoryPath.string()));
    }
    std::filesystem::path filepath(path);
    filepath = mDirectoryPath / filepath;
    return std::filesystem::exists(filepath);
}
