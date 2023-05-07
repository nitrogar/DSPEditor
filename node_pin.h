#ifndef NODE_PIN_H
#define NODE_PIN_H
#include "imgui.h"
#include "node_editor.h"
namespace app {

class node_pin {
public:
  node_editor::pin_type type;
  node_link *link;
  node *parent;
  ImU32 color = ImColor(94, 95, 191);
  std::string name;
  imnode::PinId id;
  enum pin_state { FLOATING, WIRED, ERROR, UNCOMPATIBLE };
  pin_state state;

public:
  node_pin(node *parent, std::string &name, node_editor::pin_type t);
  ~node_pin(){};

  virtual void Begin();
  virtual void End();
  //  bool is_connected(){};
  void connect_to(node_link *l);
  void unconnect(){};
  void draw();
  // node_out eval(){};
 ImRect ImGui_GetItemRect() {
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }
};

} // namespace app
  //
#endif
