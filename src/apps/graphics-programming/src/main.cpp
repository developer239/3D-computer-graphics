#include "LoopStrategy.h"
#include "core-imgui/Strategy.h"
#include "core/Loop.h"

int main() {
  CoreImGui::Strategy gui;
  LoopStrategy strategy;

  auto loop = Core::Loop({&gui, &strategy});
  loop.Run();
}
