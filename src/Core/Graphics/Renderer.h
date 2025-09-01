#ifndef HDEF_RENDERER
#define HDEF_RENDERER

#include <memory>
#include <vector>

#include "RenderCommand.h"

class Camera;
struct SDL_Renderer;
struct SDL_Window;
class Renderer
{
private:
    std::shared_ptr<SDL_Renderer> mRenderer;
    std::shared_ptr<SDL_Window> mWindow;

public:
    Renderer();
    ~Renderer();
    bool Init(const char *appName, const char *appVersion, const char *appId, const char *windowName);
    std::shared_ptr<SDL_Renderer> GetRenderer() const { return mRenderer; }
    std::shared_ptr<SDL_Window> GetWindow() const { return mWindow; }
    void StartFrameRendering() const;
    void Render(std::vector<RenderCommand> renderQueue, std::shared_ptr<Camera> camera) const;
    void FinishRendering() const;
};

#endif