#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

struct FileEntry
{
    std::string path;
    uint64_t offset;
    uint64_t size;
};

int Pack(const std::string &folderPath, const std::string &outPath)
{
    std::vector<FileEntry> header;
    std::vector<char> blob;
    uint64_t currentOffset = 0;

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

    std::ofstream outFile(std::format(outPath, folderPath), std::ios::binary);

    uint32_t fileCount = (uint32_t)header.size();
    outFile.write(reinterpret_cast<char *>(&fileCount), sizeof(fileCount));

    for (auto &e : header)
    {
        uint16_t pathLen = (uint16_t)e.path.size();
        outFile.write(reinterpret_cast<char *>(&pathLen), sizeof(pathLen));
        outFile.write(e.path.data(), pathLen);
        outFile.write(reinterpret_cast<char *>(&e.offset), sizeof(e.offset));
        outFile.write(reinterpret_cast<char *>(&e.size), sizeof(e.size));
    }
    outFile.write(blob.data(), blob.size());
    outFile.close();
    return blob.size();
}

int Unpack(const std::string &inPath, const std::string &outPath)
{
}

int main()
{
    Pack("Data", "Data.pak");
    return 0;
}