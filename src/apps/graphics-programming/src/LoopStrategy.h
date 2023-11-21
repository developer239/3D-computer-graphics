#pragma once

#include "Textures.h"
#include "core/AssetStore.h"
#include "core/IStrategy.h"
#include "core/Window.h"
#include "gui/Controls.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "logger/Logger.h"
#include "rendering/Painter.h"
#include "rendering/WavefrontLoader.h"

class LoopStrategy : public Core::IStrategy {
 public:
  Light light;
  Camera camera;
  Painter painter = Painter(800, 600);
  WavefrontLoader wavefrontLoader;
  Mesh mesh;
  std::unique_ptr<uint32_t[]> redbrickTexture;
  Controls debug;

  bool shouldRotate = false;
  bool shouldCull = true;
  bool shouldRenderTexture = true;
  bool shouldRenderWireframe = false;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    // wavefrontLoader.LoadObjFile("assets/models/f22.obj", mesh);
    mesh = Mesh::CreateCube();

    // Calculate the size of REDBRICK_TEXTURE in terms of uint32_t elements
    constexpr size_t textureSize = sizeof(REDBRICK_TEXTURE) / sizeof(uint32_t);

    // Allocate memory for uint32_t array
    auto tempTexture = std::make_unique<uint32_t[]>(textureSize);

    // Convert every 4 bytes from REDBRICK_TEXTURE to one uint32_t
    for (size_t i = 0; i < textureSize; ++i) {
      size_t index = i * 4;  // Index in the uint8_t array
      tempTexture[i] = static_cast<uint32_t>(REDBRICK_TEXTURE[index]) |
                       static_cast<uint32_t>(REDBRICK_TEXTURE[index + 1]) << 8 |
                       static_cast<uint32_t>(REDBRICK_TEXTURE[index + 2])
                           << 16 |
                       static_cast<uint32_t>(REDBRICK_TEXTURE[index + 3]) << 24;
    }

    // Move the converted texture to the const qualified smart pointer
    redbrickTexture = std::move(tempTexture);
  }

  void HandleEvent(SDL_Event& event) override {}

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {
    if (shouldRotate) {
      mesh.rotate();
    }

    painter.colorBuffer.ClearColorBuffer();
    painter.BackgroundGrid();
    painter.Mesh(mesh, shouldCull, shouldRenderTexture, shouldRenderWireframe, camera, light, redbrickTexture);
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    painter.colorBuffer.RenderColorBuffer(renderer);
    debug.Render(shouldRotate, shouldCull, mesh, shouldRenderTexture, shouldRenderWireframe);
  }
};
