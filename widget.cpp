#include "widget.h"
#include "logger.h"
namespace app {
widget::widget(window *parent) { this->parent = parent; }
widget::~widget() {}
void widget::draw() {}
} // namespace app
