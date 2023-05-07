#ifndef NODE_LINK_H
#define NODE_LINK_H

#include "node_editor.h"

#include <string>

namespace app {
class node_link {
public:
  std::string name;
  node_pin *from;
  node_pin *to;
  imnode::LinkId id;
  node_editor *parent;
  ImVec4 color = ImVec4(1, 1, 1, 1);
  float thickness = 1.0f;

public:
  node_link(node_editor *parent);
  ~node_link();
  void draw();
};

} // namespace app

#endif
