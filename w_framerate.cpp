#include "w_framerate.h"
#include "imgui.h"
#include "logger.h"
namespace app {
framerate_widget::framerate_widget(window *parent) : widget(parent) {
  this->name = "framerate widget";
}
framerate_widget::~framerate_widget(){};
void framerate_widget::draw() {
  auto io = ImGui::GetIO();

  static float f = 0.0f;
  static int counter = 0;

  ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"

  ImGui::Text("This is some useful text."); // Display some text (you can
                                            // use a format strings too)
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / io.Framerate, io.Framerate);
  ImGui::End();
};
} // namespace app
