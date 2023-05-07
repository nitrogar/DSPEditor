#ifndef WINDOW_H
#define WINDOW_H

#include "SDL2/SDL.h"
#include "imgui_impl_sdl.h"
#include "widget.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <vector>
namespace app {
class window {
private:
  int width;
  int height;
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_render;
  ImGuiIO *pio;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  enum State { NEED_EXIT, RUN, IDEL };
  std::vector<widget *> widgets;
  State state = IDEL;
  void init_widgets();
  void imgui_style_editor();
  void implot_style_editor();
  void ShowDockingDisabledMessage();
  void HelpMarker(const char *desc);
  void ShowExampleAppDockSpace(bool *p_open);

public:
  window();
  ~window();
  void draw();
  void handle_event(SDL_Event &e);
  [[nodiscard]] SDL_Window *get_native_window();
  [[nodiscard]] SDL_Renderer *get_native_renderer();
};
} // namespace app

#endif // !
