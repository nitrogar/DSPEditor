#ifndef NODE_EDITOR_H
#define NODE_EDITOR_H

#include "imgui.h"
#include "imgui_node_editor.h"
#include "logger.h"
#include "widget.h"
#include "window.h"
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

namespace imnode = ax::NodeEditor;

namespace app {
class node;
class node_link;
class node_pin;

class node_editor : public widget {

private:
  std::unordered_map<uintptr_t, node_link *> mlinks;
  std::unordered_map<uintptr_t, node *> mnodes;
  std::unordered_map<uintptr_t, node_pin *> pins;
  std::vector<node *> nodes;
  std::vector<node_link *> links;
  std::vector<node_pin *> vpins;
  node *start;
  node *end;
  std::string name;
  imnode::EditorContext *ctx;
  uint32_t current_id = 100;

public:
  node_editor(window *parent);
  ~node_editor();
  void add_node(node *new_node){};
  void delete_node(node *del_node){};
  void draw() override;
  void add_pin(node_pin *);
  imnode::PinId get_pin_id();
  imnode::LinkId get_link_id();
  imnode::NodeId get_node_id();
  void ShowStyleEditor(bool *show = nullptr);

      enum pin_type {
        INPUT,
        OUTPUT,
        FLOW
      };
};

} // namespace app

#endif
