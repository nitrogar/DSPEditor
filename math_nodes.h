#ifndef NODE_MATH_HEADER
#define NODE_MATH_HEADER

#include "node.h"

namespace app {
class isum_node : public node {
public:
  isum_node(node_editor *parent) : node(parent) {
    add_pin("sum", node_editor::pin_type::OUTPUT);
    add_pin("x", node_editor::pin_type::INPUT);
    add_pin("y", node_editor::pin_type::INPUT);
  }
  std::any x(std::string &name) override {
    if (name == "sum")
      return v<int>("x") + v<int>("y");
    err_pin_not_found(name);
  }

  void draw_properites() override {
    node::draw_properites();
    ImGui::Text("SUM");
  }
};
}
#endif
