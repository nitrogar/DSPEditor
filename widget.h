#ifndef WIDGET_H
#define WIDGET_H
#include "imgui_internal.h"
#include <string>
namespace app {
class window;
class widget {

protected:
  std::string name;
  int width;
  int height;

  window *parent;

public:
  widget(window *parent);
  ~widget();
  virtual void draw();
};

} // namespace app
  //
#endif // !WIDGET_H
