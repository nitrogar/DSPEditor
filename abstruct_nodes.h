#ifndef ABSTRUCT_HEADER
#define ABSTRUCT_HEADER

#include "node.h"
namespace app{

class runable_node : public node {
public:
  runable_node(node_editor *parent) : node(parent) {}
  void virtual run() {}

  void virtual draw_properites() override {
    node::draw_properites();
    if (ImGui::Button("RUN")) {
      run();
    }
    ImGui::SameLine();
    if (ImGui::Button("STOP")) {
      //... my_code
    }
  }
};
class sink_node : public runable_node {
public:
  sink_node(node_editor *parent) : runable_node(parent) {
    add_pin("source", node_editor::pin_type::INPUT);
  }
  void run() override { log_info(std::format("value = {}", v<int>("source"))); }
};
}
#endif
