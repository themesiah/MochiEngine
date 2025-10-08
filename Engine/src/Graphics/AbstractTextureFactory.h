#ifndef HDEF_ABSTRACTTEXTUREFACTORY
#define HDEF_ABSTRACTTEXTUREFACTORY

#include <memory>
#include <string>
#include <unordered_map>

namespace Mochi::Graphics
{
    class ITexture;
    class AbstractTextureFactory
    {
    protected:
        std::unordered_map<std::string, std::shared_ptr<ITexture>> mTexturesMap;
        virtual std::shared_ptr<ITexture> LoadTexture(const std::string &texturePath) = 0;

    public:
        AbstractTextureFactory();
        virtual ~AbstractTextureFactory();
        std::shared_ptr<ITexture> GetTexture(const std::string &texturePath);
        void DeleteCache();
        void DeleteTexture(const std::string &texturePath);
    };
}

#endif