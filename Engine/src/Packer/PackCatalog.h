#ifndef HDEF_PACKCATALOG
#define HDEF_PACKCATALOG

#include <vector>
#include <memory>
#include <string>

namespace Mochi::FS
{
    class IFileLoader;

    /// @brief The pack catalog is a class that creates and owns file loaders and acts as a combined catalog for all of them at once.
    /// This means that opening pack A and pack B allows you to use GetFile with files that are in any or both of them.
    /// As it searches in reverse order of pack opening, files of the last opened packs are prioritized when using GetFile and HasFile, which
    /// allows you to "override" assets of a previous pack, allowing a simple but effective way of modding.
    /// Each file system inside the catalog starts at the root, so it is needed the same folder structure in two file systems to be able to override.
    /// Example:
    /// Pack A
    /// - Fonts
    ///     - Arial.ttf
    /// - Sprites
    ///     - Square.png
    /// Pack B
    /// - Fonts
    ///     - Times.ttf
    /// - Sprites
    ///     - Square.png
    /// Pack C
    /// - Images
    ///     - Square.png
    /// For this example, if we load packs in this order: A --> B --> C we will have this catalog:
    /// Catalog
    /// - Fonts
    ///     - Arial.ttf (A)
    ///     - Times.ttf (B)
    /// - Sprites
    ///     - Square.png (B)
    /// - Images
    ///     - Square.png (C)
    ///
    /// Lifecycle:
    /// - A default catalog is created in the Engine. If in debug, it creates a SystemFileLoader, and if in release, it creates a PackFile.
    /// - New catalogs can be created if you have different file loader type needs, like using always a SystemFileLoader. Otherwise, the default
    /// catalog can be always used.

    class PackCatalog
    {

    public:
        enum FileLoaderType
        {
            FileSystem = 0,
            Packfile = 1
        };
        /// @brief Constructs the file loader with a loader type
        /// @param
        PackCatalog(FileLoaderType);
        ~PackCatalog();
        void OpenPack(const std::string &);
        bool ClosePack(const std::string &);
        bool IsPackOpen(const std::string &);
        std::vector<char> GetFile(const std::string &) const;
        bool HasFile(const std::string &) const;
        std::vector<std::string> GetAvailableFiles() const;

    private:
        struct Pack
        {
            std::string packId;
            std::shared_ptr<IFileLoader> fileLoader;
        };
        std::vector<Pack> mPacks;
        FileLoaderType mLoaderType;
    };

}

#endif