#pragma once

#include "imgui.h"

#include "../geometry/Cube.h"
#include "../rendering/Projector.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

class Controls {
 public:
  void Render(bool& shouldRotate, bool& shouldCull, float& fovFactor, Mesh& mesh) {
    if (ImGui::Begin("Controls")) {
      ImGui::Checkbox("Should rotate", &shouldRotate);
      ImGui::Checkbox("Should cull", &shouldCull);

      ImGui::SliderFloat(
          "FoV",
          &fovFactor,
          128,
          1280
      );

      ImGui::SliderFloat(
          "Rotation X",
          &mesh.rotation.x,
          0,
          360
      );
      ImGui::SliderFloat(
          "Rotation Y",
          &mesh.rotation.y,
          0,
          360
      );
      ImGui::SliderFloat(
          "Rotation Z",
          &mesh.rotation.z,
          0,
          360
      );

      ImGui::End();
    }
  };
};
