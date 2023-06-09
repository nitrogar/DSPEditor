#include "window.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "implot.h"
#include "logger.h"
#include "node_editor.h"
#include "plot_widget.h"
#include "w_framerate.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>
#include <sys/types.h>
namespace app {
window::window() {
  uint32_t sdl_window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
  uint32_t sdl_window_postion = SDL_WINDOWPOS_CENTERED;
  uint32_t sdl_render_flags =
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

  std::string title = "Inspector";
  /*
  this->sdl_window =
      SDL_CreateWindow(title.c_str(), sdl_window_postion, sdl_window_postion,
                       this->width, this->height, sdl_window_flags);
  this->sdl_render = SDL_CreateRenderer(this->sdl_window, -1, sdl_render_flags);
  */
  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  sdl_window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                1280, 720, window_flags);
  sdl_render = SDL_CreateRenderer(
      sdl_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  if (!this->sdl_render) {
    logger::log_warning(SDL_GetError());
    logger::log_error("error creating sdl rendere");
  }
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  this->pio = &ImGui::GetIO();

  ImGuiIO io = *pio;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  logger::log_info("before imgui renderer");
  ImGui_ImplSDL2_InitForSDLRenderer(sdl_window, sdl_render);
  ImGui_ImplSDLRenderer_Init(sdl_render);
  // add default widgets
  init_widgets();
}
window::~window() {
  ImPlot::DestroyContext();
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyWindow(this->sdl_window);
  SDL_DestroyRenderer(this->sdl_render);
}
SDL_Window *window::get_native_window() { return this->sdl_window; }
SDL_Renderer *window::get_native_renderer() { return this->sdl_render; }
void window::draw() {
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();

  ImGui::NewFrame();

  bool t = true;
  ShowExampleAppDockSpace(&t);
  //  draw widgets
  for (auto w : widgets) {
    w->draw();
  }
  //imgui_style_editor(); imgui editor settings customizer
  //implot_style_editor();
  

  // render the scene
  //
  //

  ImGui::Render();
  SDL_RenderSetScale(sdl_render, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);

  SDL_SetRenderDrawColor(
      sdl_render, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255),
      (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
  SDL_RenderClear(sdl_render);
  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
  SDL_RenderPresent(sdl_render);
}
void window::handle_event(SDL_Event &e) {
  auto win_id = SDL_GetWindowID(sdl_window);
  ImGui_ImplSDL2_ProcessEvent(&e);
  if (e.type == SDL_WINDOWEVENT && e.window.windowID == win_id) {
    switch (e.window.event) {
    case SDL_WINDOWEVENT_CLOSE:
      state = NEED_EXIT;
    }
  }
}
void window::init_widgets() {
  auto framerate = new framerate_widget(this);
  auto plot = new plot_widget(this);
  auto n_editor = new node_editor(this);
  this->widgets.push_back(framerate);
  this->widgets.push_back(n_editor);
  // this->widgets.push_back(plot);
}
void window::imgui_style_editor() {
  ImGui::Begin("imgui style"); // Create a window called "Hello, world!"

  ImGui::ShowStyleEditor(); // use a format strings too)
  ImGui::End();
}

void window::implot_style_editor() {
  ImGui::Begin("implot style"); // Create a window called "Hello, world!"
  ImPlot::ShowStyleEditor();
  ImGui::End();
}

void window::ShowExampleAppDockSpace(bool *p_open) {
  // If you strip some features of, this demo is pretty much equivalent to
  // calling DockSpaceOverViewport()! In most cases you should be able to just
  // call DockSpaceOverViewport() and ignore all the code below! In this
  // specific demo, we are not using DockSpaceOverViewport() because:
  // - we allow the host window to be floating/moveable instead of filling the
  // viewport (when opt_fullscreen == false)
  // - we allow the host window to have padding (when opt_padding == true)
  // - we have a local menu bar in the host window (vs. you could use
  // BeginMainMenuBar() + DockSpaceOverViewport() in your code!) TL;DR; this
  // demo is more complicated than what you would normally use. If we removed
  // all the options we are showcasing, this demo would become:
  //     void ShowExampleAppDockSpace()
  //     {
  //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  //     }

  static bool opt_fullscreen = true;
  static bool opt_padding = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;
  } else {
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  if (!opt_padding)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", p_open, window_flags);
  if (!opt_padding)
    ImGui::PopStyleVar();

  if (opt_fullscreen)
    ImGui::PopStyleVar(2);

  // Submit the DockSpace
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  } else {
    ShowDockingDisabledMessage();
  }

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Options")) {
      // Disabling fullscreen would allow the window to be moved to the front of
      // other windows, which we can't undo at the moment without finer window
      // depth/z control.
      ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
      ImGui::MenuItem("Padding", NULL, &opt_padding);
      ImGui::Separator();

      if (ImGui::MenuItem("Flag: NoSplit", "",
                          (dockspace_flags & ImGuiDockNodeFlags_NoSplit) !=
                              0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
      }
      if (ImGui::MenuItem("Flag: NoResize", "",
                          (dockspace_flags & ImGuiDockNodeFlags_NoResize) !=
                              0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
      }
      if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
                          (dockspace_flags &
                           ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
      }
      if (ImGui::MenuItem(
              "Flag: AutoHideTabBar", "",
              (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
      }
      if (ImGui::MenuItem(
              "Flag: PassthruCentralNode", "",
              (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0,
              opt_fullscreen)) {
        dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
      }
      ImGui::Separator();

      if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
        *p_open = false;
      ImGui::EndMenu();
    }
    HelpMarker(
        "When docking is enabled, you can ALWAYS dock MOST window into "
        "another! Try it now!"
        "\n"
        "- Drag from window title bar or their tab to dock/undock."
        "\n"
        "- Drag from window menu button (upper-left button) to undock an "
        "entire node (all windows)."
        "\n"
        "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == "
        "false, default)"
        "\n"
        "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)"
        "\n"
        "This demo app has nothing to do with enabling docking!"
        "\n\n"
        "This demo app only demonstrate the use of ImGui::DockSpace() which "
        "allows you to manually create a docking node _within_ another window."
        "\n\n"
        "Read comments in ShowExampleAppDockSpace() for more details.");

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

void window::HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered(ImGuiHoveredFlags_None)) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void window::ShowDockingDisabledMessage() {
  ImGuiIO &io = ImGui::GetIO();
  ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
  ImGui::Text(
      "Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
  ImGui::SameLine(0.0f, 0.0f);
  if (ImGui::SmallButton("click here"))
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}
} // namespace app
