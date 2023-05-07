#ifndef PLOT_WIDGET_H
#define PLOT_WIDGET_H

#include "widget.h"

namespace app {

class plot_widget : public widget {
public:
  plot_widget(window *parent);
  ~plot_widget();
  void draw() override;
  void show_style_editor();
};

} // namespace app
  //
#endif
