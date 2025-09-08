#include "PackFile.h"

#include <format>
#include <filesystem>
#include <iostream>
#include <algorithm>

#include "../Utils/StringUtils.h"
#include "../Exception.hpp"

namespace Mochi::FS
{

    PackFile::PackFile(const std::string &packfilePath)
    {
        mHandler = std::ifstream(packfilePath, std::ios::binary);
        if (!mHandler)
            throw std::runtime_error(std::format("Failed to open packfile {}", packfilePath));

        LoadPackIndex();
    }

    PackFile::~PackFile()
    {
        if (mHandler)
        {
            mHandler.close();
        }
    }

    bool PackFile::IsValid() const
    {
        return mHandler.is_open();
    }

    template <typename T>
    T PackFile::ReadBinary(std::ifstream &in)
    {
        T value;
        in.read(reinterpret_cast<char *>(&value), sizeof(T));
        return value;
    }

    size_t PackFile::Pack(const std::string &folderPath, const std::string &outPath)
    {
        std::vector<FileEntry> header;
        std::vector<char> blob;
        FileOffset_t currentOffset = 0;

        for (const auto &p : std::filesystem::recursive_directory_iterator(folderPath))
        {
            if (!std::filesystem::is_directory(p))
            {
                std::string path = p.path().string().substr(folderPath.size() + 1);
                path = Utils::NormalizePath(path);

                std::ifstream f(p.path(), std::ios::binary);
                std::vector<char> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                f.close();

                FileEntry fe;
                fe.path = path;
                fe.offset = currentOffset;
                fe.size = data.size();

                header.push_back(fe);

                blob.insert(blob.end(), data.begin(), data.end());
                currentOffset += fe.size;

                std::cout << "Packing " << path << "\n";
            }
        }

        std::ofstream outFile(outPath, std::ios::binary);

        FileCount_t fileCount = (FileCount_t)header.size();
        size_t blobSize = blob.size();
        outFile.write(reinterpret_cast<char *>(&fileCount), sizeof(fileCount));
        outFile.write(reinterpret_cast<char *>(&blobSize), sizeof(blobSize));

        for (auto &e : header)
        {
            PathLenSize_t pathLen = (PathLenSize_t)e.path.size();
            outFile.write(reinterpret_cast<char *>(&pathLen), sizeof(pathLen));
            outFile.write(e.path.data(), pathLen);
            outFile.write(reinterpret_cast<char *>(&e.offset), sizeof(e.offset));
            outFile.write(reinterpret_cast<char *>(&e.size), sizeof(e.size));
        }
        outFile.write(blob.data(), blob.size());
        outFile.close();
        return blob.size();
    }

    std::vector<char> PackFile::UnpackFile(const std::string &filePath)
    {
        FileEntry meta = mHeader.HeaderMap[filePath];

        mHandler.seekg(mHeader.HeaderLength + meta.offset, std::ios::beg);

        std::vector<char> buffer(meta.size);
        mHandler.read(buffer.data(), meta.size);
        return buffer;
    }

    size_t PackFile::Unpack(const std::string &outFolder)
    {
        if (!mHandler)
            throw EngineError("Packfile is not opened");

        for (auto &e : mHeader.HeaderMap)
        {
            auto buffer = UnpackFile(e.first);
            auto outPath = std::filesystem::path(outFolder) / e.first;
            std::filesystem::create_directories(outPath.parent_path());
            std::ofstream out(std::format("{}/{}", outFolder, e.first), std::ios::binary);
            out.write(buffer.data(), buffer.size());
        }
        return mHeader.BlobSize;
    }

    void PackFile::LoadPackIndex()
    {
        PackHeader packHeader;

        FileCount_t fileCount = ReadBinary<FileCount_t>(mHandler);
        size_t blobSize = ReadBinary<size_t>(mHandler);

        for (FileCount_t i = 0; i < fileCount; ++i)
        {
            PathLenSize_t pathLen = ReadBinary<PathLenSize_t>(mHandler);

            std::string path(pathLen, '\0');
            mHandler.read(path.data(), pathLen);

            FileOffset_t offset = ReadBinary<FileOffset_t>(mHandler);
            FileSize_t size = ReadBinary<FileSize_t>(mHandler);

            packHeader.HeaderMap[path] = FileEntry{path, offset, size};
        }
        packHeader.HeaderLength = mHandler.tellg();
        packHeader.BlobSize = blobSize;

        mHeader = packHeader;
    }

    std::vector<char> PackFile::GetFile(const std::string &path)
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        FileEntry meta = mHeader.HeaderMap[normalizedPath];
        if (!mHandler)
            throw EngineError("Packfile is not opened");

        mHandler.seekg(mHeader.HeaderLength + meta.offset, std::ios::beg);

        std::vector<char> buffer(meta.size);
        mHandler.read(buffer.data(), meta.size);

        return buffer;
    }

    bool PackFile::HasFile(const std::string &path) const
    {
        std::string normalizedPath = Utils::NormalizePath(path);
        return mHeader.HeaderMap.find(normalizedPath) != mHeader.HeaderMap.end();
    }

    std::vector<std::string> PackFile::GetAvailableFiles() const
    {
        std::vector<std::string> list;
        for (auto &file : mHeader.HeaderMap)
        {
            list.push_back(file.first);
        }
        return list;
    }
}
