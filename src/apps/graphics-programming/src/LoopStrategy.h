#pragma once

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
  Controls debug;
  Camera camera;
  // TODO: pass width and height from window instance
  Painter painter = Painter(800, 600);

  WavefrontLoader wavefrontLoader;
  Mesh mesh;
  bool shouldRotate = false;
  bool shouldCull = false;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    wavefrontLoader.LoadObjFile("assets/models/f22.obj", mesh);
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
    painter.Mesh(mesh, shouldCull, camera);
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    painter.colorBuffer.RenderColorBuffer(renderer);
    debug.Render(shouldRotate, shouldCull, camera.fovFactor, mesh);
  }
};
