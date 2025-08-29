#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>

typedef uint16_t PathLenSize_t;
typedef uint64_t PackHeaderLength_t;
typedef uint64_t FileOffset_t;
typedef uint64_t FileSize_t;
typedef uint32_t FileCount_t;

template <typename T>
T readBinary(std::ifstream &in)
{
    T value;
    in.read(reinterpret_cast<char *>(&value), sizeof(T));
    return value;
}

struct FileEntry
{
    std::string path;
    FileOffset_t offset;
    FileSize_t size;
};

typedef std::unordered_map<std::string, FileEntry> Header_t;
struct PackHeader
{
    PackHeaderLength_t HeaderLength;
    Header_t HeaderMap;
};

int Pack(const std::string &folderPath, const std::string &outPath)
{
    std::vector<FileEntry> header;
    std::vector<char> blob;
    FileOffset_t currentOffset = 0;

    for (const auto &p : std::filesystem::recursive_directory_iterator(folderPath))
    {
        if (!std::filesystem::is_directory(p))
        {
            std::string path = p.path().string().substr(folderPath.size() + 1);

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

            std::cout << path << "\n";
        }
    }

    std::ofstream outFile(outPath, std::ios::binary);

    FileCount_t fileCount = (FileCount_t)header.size();
    outFile.write(reinterpret_cast<char *>(&fileCount), sizeof(fileCount));

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

PackHeader LoadPackIndex(const std::string &inPath)
{
    std::ifstream inFile(inPath, std::ios::binary);
    if (!inFile)
        throw std::runtime_error("Failed to open packfile");

    PackHeader packHeader;

    FileCount_t fileCount = readBinary<FileCount_t>(inFile);

    for (FileCount_t i = 0; i < fileCount; ++i)
    {
        PathLenSize_t pathLen = readBinary<PathLenSize_t>(inFile);

        std::string path(pathLen, '\0');
        inFile.read(path.data(), pathLen);

        FileOffset_t offset = readBinary<FileOffset_t>(inFile);
        FileSize_t size = readBinary<FileSize_t>(inFile);

        packHeader.HeaderMap[path] = FileEntry{path, offset, size};
    }
    packHeader.HeaderLength = inFile.tellg();

    return packHeader;
}

std::vector<char> ExtractFile(const std::string &packPath, const FileEntry &entry, uint64_t headerEndOffset)
{
    std::ifstream in(packPath, std::ios::binary);
    if (!in)
        throw std::runtime_error("Failed to reopen packfile.");

    // actual blob offset = headerEndOffset + entry.offset
    in.seekg(headerEndOffset + entry.offset, std::ios::beg);

    std::vector<char> buffer(entry.size);
    in.read(buffer.data(), entry.size);

    return buffer;
}

std::vector<char> Unpack(const std::string &inPath, const std::string &filePath, PackHeader packHeader)
{
    FileEntry meta = packHeader.HeaderMap[filePath];
    std::ifstream in(inPath, std::ios::binary);
    if (!in)
        throw std::runtime_error("Failed to open packfile");

    in.seekg(packHeader.HeaderLength + meta.offset, std::ios::beg);

    std::vector<char> buffer(meta.size);
    in.read(buffer.data(), meta.size);
    return buffer;
}

void UnpackAll(const std::string &inPath, const std::string &outFolder)
{
    PackHeader packHeader = LoadPackIndex(inPath);

    std::ifstream in(inPath, std::ios::binary);
    if (!in)
        throw std::runtime_error("Failed to open packfile");

    for (auto &e : packHeader.HeaderMap)
    {
        auto buffer = Unpack(inPath, e.first, packHeader);
        auto outPath = std::filesystem::path(outFolder) / e.first;
        std::filesystem::create_directories(outPath.parent_path());
        std::ofstream out(std::format("{}/{}", outFolder, e.first), std::ios::binary);
        out.write(buffer.data(), buffer.size());
    }
}

int main()
{
    Pack("Data", "Data.pak");
    std::cout << "Packed correctly" << std::endl;
    UnpackAll("Data.pak", "OutData");
    std::cout << "Unpacked correctly" << std::endl;
    return 0;
}