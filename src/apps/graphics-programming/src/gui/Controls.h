#pragma once

#include "imgui.h"

#include "../geometry/Mesh.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

class Controls {
 public:
  void Render(
      bool& shouldRotate, bool& shouldCull, Mesh& mesh
  ) {
    if (ImGui::Begin("Controls")) {
      ImGui::Checkbox("Should rotate", &shouldRotate);
      ImGui::Checkbox("Should cull", &shouldCull);

      ImGui::SliderFloat("Rotation X", &mesh.rotation.x, 0, 360);
      ImGui::SliderFloat("Rotation Y", &mesh.rotation.y, 0, 360);
      ImGui::SliderFloat("Rotation Z", &mesh.rotation.z, 0, 360);

      if (ImGui::CollapsingHeader("Scale")) {
        ImGui::SliderFloat("Scale X", &mesh.scale.x, 0, 10);
        ImGui::SliderFloat("Scale Y", &mesh.scale.y, 0, 10);
        ImGui::SliderFloat("Scale Z", &mesh.scale.z, 0, 10);
      }

      ImGui::End();
    }
  };
};
