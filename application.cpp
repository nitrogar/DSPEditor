#include "application.h"
#include "imgui.h"
#include "logger.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#
namespace app {
application::application() {
  uint32_t sdl_flags =
      SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
  if (SDL_Init(sdl_flags))
    logger::log_error("could not inilize sdl");
  state = RUN;
  this->window = std::make_unique<app::window>();
}
application::~application() { SDL_Quit(); }
int application::run() {
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      window->handle_event(event);
      if (event.type == SDL_QUIT)
        this->state = NEED_EXIT;
    }

    switch (state) {
    case RUN:
      // Start the Dear ImGui frame
      window->draw();
      break;
    case NEED_EXIT:
      return 0;
      break;
    default:
      break;
    }
  }
}
} // namespace app
