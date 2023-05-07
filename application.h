

#include "imgui_internal.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <memory>
namespace app {
class application {
public:
  application();
  ~application();
  int run();
  void stop();

private:
  enum State { NEED_EXIT, RUN };
  State state;
  std::unique_ptr<app::window> window;
};

} // namespace app
