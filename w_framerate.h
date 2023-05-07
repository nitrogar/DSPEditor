#ifndef FRAMERATE_WIDGET_H
#define FRAMERATE_WIDGET_H

#include "widget.h"

namespace app {

class framerate_widget : public widget {
public:
  framerate_widget(window *parent);
  ~framerate_widget();
  void draw() override;
};

} // namespace app
  //
#endif
