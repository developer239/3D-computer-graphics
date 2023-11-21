#pragma once

#include "core/AssetStore.h"
#include "core/IStrategy.h"
#include "core/Window.h"
#include "gui/Controls.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "logger/Logger.h"
#include "rendering/Painter.h"
#include "rendering/Texture.h"
#include "rendering/WavefrontLoader.h"

class LoopStrategy : public Core::IStrategy {
 public:
  Light light;
  Camera camera;
  Painter painter = Painter(800, 600);
  WavefrontLoader wavefrontLoader;
  Mesh mesh;
  Texture texture;
  Controls debug;

  bool shouldRotate = false;
  bool shouldCull = true;
  bool shouldRenderTexture = false;
  bool shouldRenderWireframe = true;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    wavefrontLoader.LoadObjFile("assets/models/cube.obj", mesh);
    // mesh = Mesh::CreateCube();
    texture = Texture("assets/textures/cube.png");
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
    painter.Mesh(
        mesh,
        shouldCull,
        shouldRenderTexture,
        shouldRenderWireframe,
        camera,
        light,
        texture
    );
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    painter.colorBuffer.RenderColorBuffer(renderer);
    debug.Render(
        shouldRotate,
        shouldCull,
        mesh,
        shouldRenderTexture,
        shouldRenderWireframe
    );
  }
};
