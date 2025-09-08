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
#include "../Utils/Logger.h"
#include "../Exception.hpp"

namespace Mochi::FS
{
    PackCatalog::PackCatalog(FileLoaderType fileLoaderType) : mPacks(), mLoaderType(fileLoaderType)
    {
    }

    PackCatalog::~PackCatalog()
    {
    }

    void PackCatalog::OpenPack(const std::string &packPath)
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
            throw EngineError("Invalid loader for catalog");
        }

        if (fileLoader)
        {
            std::string pathNormalized = Utils::NormalizePath(packPath);
            Pack pack;
            pack.packId = pathNormalized;
            pack.fileLoader = fileLoader;
            mPacks.insert(mPacks.begin(), pack);
        }
        else
        {
            std::string str = std::format("Pack in {} with loading mode {}", packPath, mLoaderType == FileLoaderType::FileSystem ? "File System" : "Pack File");
            throw ResourceNotFoundError(str);
        }
    }

    bool PackCatalog::ClosePack(const std::string &packId)
    {
        std::string idNormalized = Utils::NormalizePath(packId);
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
        std::string idNormalized = Utils::NormalizePath(packId);
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
        std::string normalizedPath = Utils::NormalizePath(filePath);
        for (size_t i = 0; i < mPacks.size(); ++i)
        {
            if (mPacks[i].fileLoader->HasFile(normalizedPath))
            {
                return mPacks[i].fileLoader->GetFile(normalizedPath);
            }
        }
        throw ResourceNotFoundError(filePath);
    }

    bool PackCatalog::HasFile(const std::string &filePath) const
    {
        std::string normalizedPath = Utils::NormalizePath(filePath);
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
}
