#ifndef NODE_H
#define NODE_H

#include "imgui.h"
#include "node_editor.h"
#include <iostream>
#include <vector>
namespace app {

class node {
protected:
  std::vector<node_pin *> in_pins;
  std::vector<node_pin *> out_pins;
  imnode::NodeId id;
  ImU32 header_color = ImColor(59, 59, 209);
  node_editor *parent;
  std::string name;

public:
  node(node_editor *parent);
  ~node(){};
  void add_pin(){};
  void delete_pin(){};
  void virtual draw();
  void virtual draw_header();
  void virtual draw_pins();
  virtual void process(){};
  void add_pin(std::string name, node_editor::pin_type type);
  virtual void evaluate_pin(node_pin *pin){};
  imnode::PinId get_pin_id();
};
class start_node : public node {
public:
  start_node(node_editor *parent) : node(parent) {
    add_pin("in1", node_editor::pin_type::INPUT);
    add_pin("in1", node_editor::pin_type::INPUT);
    add_pin("out1", node_editor::pin_type::OUTPUT);
  }
  // void draw() override {
  //  logger::log_info("drawing the start node");
  //  std::cout << "n_id: " << n_id.Get() << " pi_id:" << pi_id.Get()
  //            << " po_id:" << po_id.Get() << std::endl;
  //  imnode::BeginNode(n_id);
  //  ImGui::Text("Node A");
  //  imnode::BeginPin(pi_id, imnode::PinKind::Input);
  //  ImGui::Text("-> In");
  //  imnode::EndPin();
  //  ImGui::SameLine();
  //  imnode::BeginPin(po_id, imnode::PinKind::Output);
  //  ImGui::Text("Out ->");
  //  imnode::EndPin();
  //  imnode::EndNode();
  //}

private:
  imnode::NodeId n_id;
  imnode::PinId pi_id;
  imnode::PinId po_id;
};

} // namespace app

#endif // DEBUG
