#include "PackCatalog.h"

#include <memory>
#include <vector>
#include <string>
#include <format>
#include <algorithm>

#include "IFileLoader.h"
#include "PackFile.h"
#include "SystemFileLoader.h"
#include "../Utils/StringUtils.h"
#include "../Logger.h"

PackCatalog::PackCatalog(FileLoaderType fileLoaderType) : mPacks(), mLoaderType(fileLoaderType)
{
}

PackCatalog::~PackCatalog()
{
}

bool PackCatalog::OpenPack(const std::string &packPath)
{
    std::shared_ptr<IFileLoader> fileLoader;
    switch (mLoaderType)
    {
    case FileLoaderType::FileSystem:
        // Create instance of SystemFileLoader
        fileLoader = std::make_shared<SystemFileLoader>(packPath);
        break;
    case FileLoaderType::Packfile:
        // Create instance of PackFile
        fileLoader = std::make_shared<PackFile>(std::format("{}.{}", packPath, PACKFILE_EXTENSION));
        break;
    default:
        return false;
    }

    if (fileLoader)
    {
        std::string pathNormalized = NormalizePath(packPath);
        Pack pack;
        pack.packId = pathNormalized;
        pack.fileLoader = fileLoader;
        mPacks.insert(mPacks.begin(), pack);

        return true;
    }
    else
    {
        LOG_ERROR(std::format("Pack in {} with loading mode {} was not found, or could not be opened.", packPath, mLoaderType));
        return false;
    }
}

bool PackCatalog::ClosePack(const std::string &packId)
{
    std::string idNormalized = NormalizePath(packId);
    auto it = std::find_if(mPacks.begin(), mPacks.end(),
                           [&](const Pack &pack)
                           {
                               return pack.packId == idNormalized;
                           });

    if (it != mPacks.end())
    {
        mPacks.erase(it); // removes the pack
        return true;
    }

    return false; // not found
}

bool PackCatalog::IsPackOpen(const std::string &packId)
{
    std::string idNormalized = NormalizePath(packId);
    auto it = std::find_if(mPacks.begin(), mPacks.end(),
                           [&](const Pack &pack)
                           {
                               return pack.packId == idNormalized;
                           });

    if (it != mPacks.end())
    {
        return true;
    }

    return false; // not found
}

std::vector<char> PackCatalog::GetFile(const std::string &filePath) const
{
    std::string normalizedPath = NormalizePath(filePath);
    for (size_t i = 0; i < mPacks.size(); ++i)
    {
        if (mPacks[i].fileLoader->HasFile(normalizedPath))
        {
            return mPacks[i].fileLoader->GetFile(normalizedPath);
        }
    }
    LOG_ERROR(std::format("File on path {} was not found on any mounted file loaders", filePath));
    throw std::runtime_error("File not found");
}

bool PackCatalog::HasFile(const std::string &filePath) const
{
    std::string normalizedPath = NormalizePath(filePath);
    for (size_t i = 0; i < mPacks.size(); ++i)
    {
        if (mPacks[i].fileLoader->HasFile(normalizedPath))
            return true;
    }
    return false;
}

std::vector<std::string> PackCatalog::GetAvailableFiles() const
{
    std::vector<std::string> list;
    for (size_t i = 0; i < mPacks.size(); ++i)
    {
        auto files = mPacks[i].fileLoader->GetAvailableFiles();
        list.insert(list.end(), files.begin(), files.end());
    }
    return list;
}
