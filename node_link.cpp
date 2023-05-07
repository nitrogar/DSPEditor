#include "node_link.h"

namespace app {

node_link::node_link(node_editor *parent) {
  this->parent = parent;
  this->id = parent->get_link_id();
}

void node_link::draw() {}
} // namespace app
