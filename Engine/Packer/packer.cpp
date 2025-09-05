#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <format>

#include "PackFile.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        throw std::runtime_error("This program need arguments to run. Use -h to show help.");
    }

    std::string arg1(argv[1]);
    if (arg1 == "-h")
    {
        // Show help
        std::cout << "***** HELP *****\n\n-pack \"FolderName\" \"OutFile.pak\"\n";
        std::cout << "-unpack \"InFile.pak\" \"OutFolder\"" << std::endl;
    }
    else if (arg1 == "-pack" && argc >= 3)
    {
        try
        {
            std::string arg2(argv[2]);
            std::string arg3(argv[3]);
            Mochi::FS::PackFile::Pack(arg2, arg3);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\nSecond and third arguments must be paths. Check -h for help." << std::endl;
        }
    }
    else if (arg1 == "-unpack" && argc >= 3)
    {
        try
        {
            std::string arg2(argv[2]);
            std::string arg3(argv[3]);
            Mochi::FS::PackFile file(arg2);
            file.Unpack(arg3);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << "\nSecond and third arguments must be paths. Check -h for help." << std::endl;
        }
    }
    else
    {
        throw std::runtime_error("No valid arguments provided. Use -h to show help.");
    }

    /*size_t bytes = PackFile::Pack("Data", "Data.pak");
    std::cout << "Packed correctly " << bytes << " bytes" << std::endl;
    PackFile file("Data.pak");
    file.Unpack("OutData");
    std::cout << "Unpacked correctly" << std::endl;*/
    return 0;
}