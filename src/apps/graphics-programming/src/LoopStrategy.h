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

  Mesh mesh;
  bool shouldRotate = false;
  bool shouldCull = false;

  // TODO: pass width and height from window instance
  Painter painter = Painter(800, 600);

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    mesh = Mesh();

    WavefrontLoader::LoadObjFile("assets/models/cube.obj", mesh);
  }

  void HandleEvent(SDL_Event& event) override {}

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {
    if (shouldRotate) {
      mesh.rotate();
    }

    painter.ClearColorBuffer();
    painter.Mesh(mesh, shouldCull);
    painter.BackgroundGrid();
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    painter.RenderColorBuffer(renderer);
    debug.Render(shouldRotate, shouldCull, painter.projector.fovFactor, mesh);
  }
};
